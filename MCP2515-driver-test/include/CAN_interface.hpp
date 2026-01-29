#include "ICAN.hpp"
#include "virtualTimer.h"

#include <cassert>
#include <cmath>
#include <unordered_map>
#include <memory>
#include <functional>
#include <type_traits>

// Bit helpers -- defined at the bottom
static inline uint8_t  getBit(const std::array<uint8_t, 8>& data, uint16_t bitIndex);
static inline void     setBit(std::array<uint8_t, 8>& data, uint16_t bitIndex, uint8_t v);
static inline uint64_t extractRawLE(const std::array<uint8_t, 8>& data, uint16_t startBit, uint8_t length);
static inline void     insertRawLE(std::array<uint8_t, 8>& data, uint16_t startBit, uint8_t length, uint64_t raw);
static inline uint16_t BENextBit(uint16_t currentBit);
static inline uint64_t extractRawBE(const std::array<uint8_t, 8>& data, uint16_t startBit, uint8_t length);
static inline void     insertRawBE(std::array<uint8_t, 8>& data, uint16_t startBit, uint8_t length, uint64_t raw);
static inline int64_t  signExtend(uint64_t raw, uint8_t length);
static inline uint64_t maskN(uint8_t n);

using RawSignalValue = uint64_t;

enum class Endianness {
    littleEndian,
    bigEndian
};

struct ICAN_Signal {
    virtual ~ICAN_Signal() = default;

    virtual void decode(const std::array<uint8_t, 8>& data) = 0;

    virtual void encode(std::array<uint8_t, 8>& data) const = 0;
};

template <typename T>
class CAN_Signal : public ICAN_Signal {
public:
    CAN_Signal(uint8_t startBit, uint8_t length, double factor, double offset, bool isSigned = true, Endianness endian = Endianness::littleEndian) :
    _startBit(startBit), _length(length), _factor(factor), _offset(offset), _isSigned(isSigned), _endian(endian), _sRawValue(0) {}

    uint8_t startBit() { return _startBit; }
    uint8_t length() { return _length; }
    double  factor() { return _factor; }
    double  offset() { return _offset; }
    bool    isSigned() { return _isSigned; }
    RawSignalValue getRawValue() { return _sRawValue; }

    T get() const { return _sValue; }

    void set(T val) { _sValue = val; }

    void decode(const std::array<uint8_t, 8>& data) {
        // 1) extract raw bits
        uint64_t rawU = _endian == Endianness::bigEndian ? extractRawBE(data, _startBit, _length) : extractRawLE(data, _startBit, _length);
        _sRawValue = rawU;

        // 2) interpret signed/unsigned raw
        double phys = 0.0;
        if (_isSigned) {
            int64_t rawS = signExtend(rawU, _length);
            phys = (double)rawS * _factor + _offset;
        } else {
            phys = (double)rawU * _factor + _offset;
        }

        // 3) store into variant in the correct type
        _sValue = (T)phys; 
    }
    
    void encode(std::array<uint8_t, 8>& data) const {
        // 1) read physical from _value as double
        double phys = (double)_sValue;

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
    uint8_t         _startBit;
    uint8_t         _length;
    double          _factor;
    double          _offset;
    bool            _isSigned;
    Endianness      _endian;
    T               _sValue;
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

    virtual void decode_from(const CAN_Frame& frame) = 0;

    virtual CAN_Frame encode_to_frame() const = 0;
};


class CAN_Bus {
    ICAN& _can;

    std::unordered_map<MsgKey, ICAN_Message*, MsgKeyHash> _rx_map;
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
        if (it != _rx_map.end() && it->second == &msg) {
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
            if (it != _rx_map.end() && it->second) {
                it->second->decode_from(rx_msg);
            }
        }
    }
};

template<size_t num_signals>
class CAN_Message : public ICAN_Message {
public:

    template<class>
    struct is_shared_ptr_to_ican_signal : std::false_type {};

    template<class U>
    struct is_shared_ptr_to_ican_signal<std::shared_ptr<U>>
    : std::bool_constant<std::is_base_of_v<ICAN_Signal, U>> {};

    // Constructor for RX message with no callback
    template <class... Ps>
    CAN_Message(CAN_Bus& bus, uint32_t id, bool extended, uint8_t length, Ps&&... signals) : 
        CAN_Message(bus, 
                    id, 
                    extended, 
                    length, 
                    std::function<void()>{}, // default to void
                    std::forward<Ps>(signals)...) 
    {}

    // Constructor for RX message with callback
    template <class... Ps>
    CAN_Message(CAN_Bus& bus, uint32_t id, bool extended, uint8_t length, std::function<void(void)> callback_function, Ps&&... signals) : 
        _bus(bus), 
        _id(id), 
        _extended(extended), 
        _length(length), 
        _callback_function(std::move(callback_function)), 
        _signals{ std::static_pointer_cast<ICAN_Signal>(std::forward<Ps>(signals))... },
        _isRX(true),
        _last_recv_time(0)
    {
        static_assert(sizeof...(signals) == num_signals, "wrong number of signals");
        static_assert((is_shared_ptr_to_ican_signal<std::decay_t<Ps>>::value && ...),
                  "Signals must be shared_ptr to ICAN_Signal-derived");
        _bus.register_message(*this);
    }

    // Constructor for TX message
    template <class... Ps>
    CAN_Message(CAN_Bus& bus, uint32_t id, bool extended, uint8_t length, uint32_t period, VirtualTimerGroup& timerGroup, Ps&&... signals) :
        _bus(bus), 
        _id(id), 
        _extended(extended), 
        _length(length), 
        _transmit_timer(period, [this]() { _bus.send(*this); }, VirtualTimer::Type::kRepeating),
        _signals{ std::static_pointer_cast<ICAN_Signal>(std::forward<Ps>(signals))... },
        _isRX(false)
    {
        static_assert(sizeof...(signals) == num_signals, "wrong number of signals");
        static_assert((is_shared_ptr_to_ican_signal<std::decay_t<Ps>>::value && ...),
                  "Signals must be shared_ptr to ICAN_Signal-derived");
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
    bool isRX() const override { return _isRX; }

    void decode_from(const CAN_Frame& frame){
        std::array<uint8_t, 8> data = frame._data;

        uint64_t tmp = 0;
        std::memcpy(&tmp, data.data(), sizeof(tmp));
        _raw = tmp;

        for (int i = 0; i < num_signals; i++){
            _signals.at(i)->decode(data);
        }

        if (_isRX && _callback_function) { _callback_function(); }

        _last_recv_time = _bus.get_time();
    }

    CAN_Frame encode_to_frame() const {
        CAN_Frame fr;

        fr._id = _id;
        fr._extendedId = _extended;
        fr._length = _length;

        std::array<uint8_t, 8> data {};
        for (int i = 0; i < num_signals; i++){
            _signals.at(i)->encode(data);
        }

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
    std::function<void(void)> _callback_function;
    std::array<std::shared_ptr<ICAN_Signal>, num_signals> _signals;

    bool _isRX;
    uint32_t _last_recv_time;
    uint64_t _raw;
    VirtualTimer _transmit_timer;
};

// macros for constructing signals and messages in a readable way
/*
    IMPORTANT
    Message macros do not actually enforce that you use the correct constructor!
    They are purely for enhancing readability.
    You still should use the associated constructor for RX or TX messages.
*/
#define RX_CAN_Message_Callback(num_signals) CAN_Message<num_signals>

#define RX_CAN_Message(num_signals) CAN_Message<num_signals>

#define TX_CAN_Message(num_signals) CAN_Message<num_signals>

/*
    IMPORTANT
    Endianness and signedness are not type enforced!
    The macros are just for clarity purposes.
*/
#define MakeSignal(type, startBit, length, factor, offset) \
    std::make_shared<CAN_Signal<type>>(startBit, length, factor, offset); 

#define MakeSignalSigned(type, startBit, length, factor, offset, isSigned) \
    std::make_shared<CAN_Signal<type>>(startBit, length, factor, offset, isSigned); 

#define MakeSignalEndian(type, startBit, length, factor, offset, endianness) \
    std::make_shared<CAN_Signal<type>>(startBit, length, factor, offset, false, endianness); 

#define MakeSignalSignedEndian(type, startBit, length, factor, offset, isSigned, endianness) \
    std::make_shared<CAN_Signal<type>>(startBit, length, factor, offset, isSigned, endianness); 

// type definitions for signals for improving readability
using CAN_Signal_UINT8  = std::shared_ptr<CAN_Signal<uint8_t>>;
using CAN_Signal_UINT16 = std::shared_ptr<CAN_Signal<uint16_t>>;
using CAN_Signal_UINT32 = std::shared_ptr<CAN_Signal<uint32_t>>;
using CAN_Signal_UINT64 = std::shared_ptr<CAN_Signal<uint64_t>>;
using CAN_Signal_INT8   = std::shared_ptr<CAN_Signal<int8_t>>;
using CAN_Signal_INT16  = std::shared_ptr<CAN_Signal<int16_t>>;
using CAN_Signal_INT32  = std::shared_ptr<CAN_Signal<int32_t>>;
using CAN_Signal_INT64  = std::shared_ptr<CAN_Signal<int64_t>>;
using CAN_Signal_FLOAT  = std::shared_ptr<CAN_Signal<float>>;
using CAN_Signal_BOOL   = std::shared_ptr<CAN_Signal<bool>>;


// Bit helper definitions

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