#include "ICAN.hpp"
#include "virtualTimer.h"

#include <variant>
#include <cassert>
#include <cmath>
#include <unordered_map>
#include <memory>
#include <functional>

typedef uint64_t RawSignalValue;

enum class SignalType {
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    INT8,
    INT16,
    INT32,
    INT64,
    FLOAT,
    BOOL
};

enum class Endianness {
    littleEndian,
    bigEndian
};

static inline uint8_t getBit(const std::array<uint8_t, 8>& data, uint16_t bitIndex) {
    return (data[bitIndex / 8] >> (bitIndex % 8)) & 1u;
}

static inline void setBit(std::array<uint8_t, 8>& data, uint16_t bitIndex, uint8_t v) {
    uint8_t &b = data[bitIndex / 8];
    uint8_t mask = uint8_t(1u << (bitIndex % 8));
    if (v) b |= mask;
    else   b &= uint8_t(~mask);
}

static inline uint64_t extractRawLE(const std::array<uint8_t, 8>& data, uint16_t startBit, uint8_t length)
{
    assert(length >= 1 && length <= 64);
    uint64_t raw = 0;
    for (uint8_t i = 0; i < length; ++i) {
        raw |= (uint64_t)getBit(data, startBit + i) << i;
    }
    return raw;
}

static inline void insertRawLE(std::array<uint8_t, 8>& data, uint16_t startBit, uint8_t length, uint64_t raw)
{
    assert(length >= 1 && length <= 64);
    for (uint8_t i = 0; i < length; ++i) {
        setBit(data, startBit + i, (raw >> i) & 1u);
    }
}

static inline uint16_t BENextBit(uint16_t currentBit) {
    uint16_t byte = currentBit / 8;
    uint16_t bit  = currentBit % 8;

    if (bit == 0) {
        return uint16_t((byte + 1) * 8 + 7);
    }
    return uint16_t(byte * 8 + (bit - 1));
}

static inline uint64_t extractRawBE(const std::array<uint8_t, 8>& data, uint16_t startBit, uint8_t length)
{
    assert(length >= 1 && length <= 64);
    uint64_t raw = 0;
    uint16_t p = startBit;

    for (uint8_t i = 0; i < length; ++i) {
        raw = (raw << 1) | (uint64_t)getBit(data, p);
        p = BENextBit(p);
    }
    return raw;
}

static inline void insertRawBE(std::array<uint8_t, 8>& data, uint16_t startBit, uint8_t length, uint64_t raw)
{
    assert(length >= 1 && length <= 64);
    uint16_t p = startBit;

    for (uint8_t i = 0; i < length; ++i) {
        uint8_t bit = (raw >> (length - 1 - i)) & 1u;
        setBit(data, p, bit);
        p = BENextBit(p);
    }
}

static inline int64_t signExtend(uint64_t raw, uint8_t length) {
    if (length == 64) return (int64_t)raw;
    uint64_t sign = 1ULL << (length - 1);
    if (raw & sign) {
        uint64_t mask = ~((1ULL << length) - 1ULL);
        return (int64_t)(raw | mask);
    }
    return (int64_t)raw;
}

static inline uint64_t maskN(uint8_t n) {
    return (n == 64) ? ~0ULL : ((1ULL << n) - 1ULL);
}

class CAN_Signal {
public:
    CAN_Signal(SignalType sType, uint8_t startBit, uint8_t length, double factor, double offset, bool isSigned = true, Endianness endian = Endianness::littleEndian) :
    _sType(sType), _startBit(startBit), _length(length), _factor(factor), _offset(offset), _isSigned(isSigned), _endian(endian) {
        _sValue = defaultValueFor(sType);
        _sRawValue = 0;
    };

    SignalType s() { return _sType; }
    uint8_t startBit() { return _startBit; }
    uint8_t length() { return _length; }
    double  factor() { return _factor; }
    double  offset() { return _offset; }
    bool    isSigned() { return _isSigned; }
    RawSignalValue getRawValue() { return _sRawValue; }

    template <class T>
    T get() const { 
        static_assert(std::is_same_v<T,uint8_t>  || std::is_same_v<T,uint16_t> || std::is_same_v<T,uint32_t> || std::is_same_v<T,uint64_t>
                    std::is_same_v<T,int8_t>   || std::is_same_v<T,int16_t>  || std::is_same_v<T,int32_t>  || std::is_same_v<T,int64_t>
                    std::is_same_v<T,float>    || std::is_same_v<T,bool>,
                    "T is not a supported SignalType");

        // runtime: ensure the signal was declared as that type
        if (_type != SignalTypeOf<T>::value) {
            assert(false && "CAN_Signal: T does not match signal's declared SignalType");
            std::abort();
        }

        // you can store _value as the matching alternative always, so this should succeed
        return std::get<T>(_value);
    }

    void decode(const std::array<uint8_t, 8>& data) {
        // 1) extract raw bits
        uint64_t rawU = _endian == Endianness::bigEndian ? extractRawBE(data, _startBit, _length) : extractRawLE(data, _startBit, _length);

        // 2) interpret signed/unsigned raw
        double phys = 0.0;
        if (_isSigned) {
            int64_t rawS = signExtend(rawU, _length);
            phys = (double)rawS * _factor + _offset;
        } else {
            phys = (double)rawU * _factor + _offset;
        }

        // 3) store into variant in the correct type
        storePhysicalIntoValue(phys);
    }
    
    void encode(std::array<uint8_t, 8>& data) const {
        // 1) read physical from _value as double
        double phys = physicalAsDouble();

        // 2) physical -> raw (inverse scaling)
        double rawD = (phys - _offset) / _factor;

        // choose rounding policy; nearbyint uses current rounding mode
        // llround is common (round half away from zero)
        int64_t rawS = (int64_t)std::llround(rawD);

        // 3) clamp/mask into N bits
        uint64_t rawU = 0;
        if (_isSigned) {
            rawU = (uint64_t)rawS & maskN(_length);
        } else {
            if (rawS < 0) rawS = 0;
            rawU = (uint64_t)rawS & maskN(_length);
        }

        // 4) insert bits
        if (_endian == Endianness::bigEndian)
            insertRawBE(data, _startBit, _length, rawU);
        else
            insertRawLE(data, _startBit, _length, rawU);
    }

private:
    using SignalValue = std::variant<uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t, float, bool>;

    static SignalValue defaultValueFor(SignalType t) {
        switch (t) {
            case SignalType::UINT8:  return uint8_t{0};
            case SignalType::UINT16: return uint16_t{0};
            case SignalType::UINT32: return uint32_t{0};
            case SignalType::INT8:   return int8_t{0};
            case SignalType::INT16:  return int16_t{0};
            case SignalType::INT32:  return int32_t{0};
            case SignalType::FLOAT:  return float{0.0f};
        }
        return uint32_t{0}; // warning supression
    }

    double physicalAsDouble() const {
        // Convert whatever is in the variant to double
        return std::visit([](auto v) -> double { return (double)v; }, _sValue);
    }

    void storePhysicalIntoValue(double phys){
        switch (_sType) {
            case SignalType::UINT8:  _sValue = (uint8_t)phys; break;
            case SignalType::UINT16: _sValue = (uint16_t)phys; break;
            case SignalType::UINT32: _sValue = (uint32_t)phys; break;
            case SignalType::INT8:   _sValue = (int8_t)phys; break;
            case SignalType::INT16:  _sValue = (int16_t)phys; break;
            case SignalType::INT32:  _sValue = (int32_t)phys; break;
            case SignalType::FLOAT:  _sValue = (float)phys; break;
        }
    }

    SignalType      _sType;
    uint8_t         _startBit;
    uint8_t         _length;
    double          _factor;
    double          _offset;
    bool            _isSigned;
    Endianness      _endian;
    SignalValue     _sValue;
    RawSignalValue  _sRawValue;
};

struct MsgKey {
    uint32_t id;
    bool extended;

    friend bool operator==(const MsgKey& a, const MsgKey& b) {
        return a.id == b.id && a.extended == b.extended;
    }
};

struct MsgKeyHash {
    size_t operator()(const MsgKey& k) const noexcept {
        return (static_cast<size_t>(k.id) * 1315423911u) ^ static_cast<size_t>(k.extended);
    }
};

struct ICAN_Message {
    virtual ~ICAN_Message() = default;
    virtual MsgKey key() const = 0;
    virtual uint8_t length() const = 0;
    virtual bool isRX() const = 0;

    virtual bool decode_from(const CAN_Frame& frame) = 0;

    virtual CAN_Frame encode_to_frame() const = 0;
};



class CAN_Bus {
    ICAN& _can;

    std::unordered_map<MsgKey, std::unique_ptr<ICAN_Message>, MsgKeyHash> _rx_map;
public:
    explicit CAN_Bus(ICAN& can) : _can(can) {}

    void register_message(ICAN_Message& msg) {
        auto k = msg.key();
        auto [it, inserted] = _rx_map.emplace(k, &msg);
        if (!inserted) {
            throw std::runtime_error("Duplicate message key registered");
        }
    }

    void unregister_message(ICAN_Message& msg) {
        auto k = msg.key();
        auto it = _rx_map.find(k);
        if (it != _rx_map.end() && it->second.get() == &msg) {
            _rx_map.erase(it);
        }
    }

    bool send(const ICAN_Message& msg) {
        return _can.send(msg.encode_to_frame());
    }

    uint32_t get_time() {
        return _can.time_ms();
    }

    void tick_bus() {
        CAN_Frame rx_msg;
        while (_can.recv(rx_msg)) {
            MsgKey k{rx_msg._id, rx_msg._extendedId};

            auto it = _rx_map.find(k);
            if (it != _rx_map.end()) {
                it->second->decode_from(rx_msg);
            }
        }
    }

    /*
    BUS uses underlying can implementation to pull frames
    frame ID's are used to find message
    message decodes the frame, updating each of its signals
    */
};

template<size_t num_signals>
class CAN_Message : public ICAN_Message {
public:

    // Constructor for RX message with no callback
    template <class... Ts>
    CAN_Message(CAN_Bus& bus, uint32_t id, bool extended, uint8_t length, Ts&&... signals) : 
        CAN_Message(bus, 
                    id, 
                    extended, 
                    length, 
                    std::function<void()>{}, // default to void
                    std::forward<Ts>(signals)...) 
    {}

    // Constructor for RX message with callback
    template <class... Ts>
    CAN_Message(CAN_Bus& bus, uint32_t id, bool extended, uint8_t length, std::function<void(void)> callback_function, Ts&&... signals) : 
        _bus(bus), 
        _id(id), 
        _extended(extended), 
        _length(length), 
        _callback_function(std::move(callback_function)), 
        _signals{ std::forward<Ts>(signals)... } 
    {
        static_assert(sizeof...(signals) == num_signals, "wrong number of signals");
        _last_recv_time = 0;
        _bus.register_message(*this);
    }

    // Constructor for TX message
    CAN_Message(CAN_Bus& bus, uint32_t id, bool extended, uint8_t length, uint32_t period, VirtualTimerGroup& timerGroup, Ts&&... signals) :
        _bus(bus), 
        _id(id), 
        _extended(extended), 
        _length(length), 
        _transmit_timer(period, [this]() { _bus.send(*this); }, VirtualTimer::Type::kRepeating),
        _signals{ std::forward<Ts>(signals)... }
    {
        timerGroup.AddTimer(_transmit_timer);
    }

    ~CAN_Message() override {
        if (_isRX) _bus.unregister_message(*this);
        else TX_disable();
    }

    MsgKey key() const override { return {_id, _extended}; }
    uint32_t id() { return _id; }
    uint8_t length() const override { return _length; }
    bool extended() { return _extended; }
    bool isRX() { return _isRX; }

    bool decode_from(const CAN_Frame& frame){
        std::array<uint8_t, 8> data = frame._data;
        _raw = *reinterpret_cast<uint64_t*>(data);

        for (int i = 0; i < num_signals; i++){
            _signals.at(i).decode(data);
        }

        if (_isRX && _callback_function) { _callback_function(); }

        _last_recv_time = _bus.get_time();
    }

    CAN_Frame encode_to_frame() const {
        CAN_Frame fr;

        fr._id = _id;
        fr._extendedId = _extended;
        fr._length = _length;
        
        std::array<uint8_t, 8> data;

        for (int i = 0; i < num_signals; i++){
            _signals.at(i).encode(data);
        }

        _raw = *reinterpret_cast<uint64_t*>(data);
        fr._data = data;

        return fr;
    }

    bool TX_enable() {
        if (!_isRX) {
            _transmit_timer.Enable();
            return true;
        }

        return false;
    }

    bool TX_disable() {
        if (!_isRX) {
            _transmit_timer.Disable();
            return true;
        }

        return false;
    }

    uint32_t getLastRecvTimeMS() {
        if (_isRX) 
            return _last_recv_time;
        else 
            return 0;
    }

private:

    CAN_Bus& _bus;
    uint32_t _id;
    uint8_t _length;
    bool _extended;
    bool _isRX;
    std::array<CAN_Signal, num_signals> _signals;
    uint64_t _raw;

    uint32_t _last_recv_time;
    std::function<void(void)> _callback_function;

    VirtualTimer _transmit_timer;
};

#define MakeRXCanMessageCallback(num_signals, can_bus, msg_id, extended, length, callback_function, signals...) \
        CAN_Message<num_signals>(can_bus, msg_id, extended, length, callback_function, signals...);
