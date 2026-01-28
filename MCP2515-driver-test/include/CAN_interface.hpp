#include "ICAN.hpp"
#include <variant>

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

class CAN_Bus;

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
        if (!std::holds_alternative<T>(_value)) {
            static_assert(false && "CAN_Signal: invalid get<T>() for this signal's stored type");
        }
        return std::get<T>(_value); 
    }

    void decode(std::span<const uint8_t> data);
    void encode(std::span<uint8_t> data) const;

private:
    using SignalValue = std::variant<uint8_t, uint16_t, uint32_t, int8_t, int16_t, int32_t, float>;

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

    void storePhysicalIntoValue(double phys);

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

template<size_t num_signals>
class CAN_Message {
public:
    template <class... Ts>
    CAN_Message(CAN_Bus& bus, uint32_t id, bool extended, uint8_t length, Ts&&... signals) : 
    _bus(bus), _id(id), _extended(extended), _length(length), _signals{ std::forward<Ts>(signals)... } {
        static_assert(size_of...(signals) == num_signals, "wrong number of signals");

        // add self to bus
        // how to construct itself in bus's data structure
    }

    uint32_t id() { return _id; }
    uint8_t length() { return _length; }
    bool extended() { return _extended; }

    bool decode_from(const CAN_Frame& frame) const;

    CAN_Frame encode_to_frame() const;

private:
    CAN_Bus& _bus;
    std::array<CAN_Signal, num_signals> _signals;
    uint32_t _id;
    uint8_t _length;
    bool _extended;
};

class CAN_Bus {
    ICAN& _can;
public:
    CAN_Bus(ICAN& can) : _can(can) {};

    void tick_bus();

    /*
    BUS uses underlying can implementation to pull frames
    frame ID's are used to find message
    message decodes the frame, updating each of its signals
    */
};
