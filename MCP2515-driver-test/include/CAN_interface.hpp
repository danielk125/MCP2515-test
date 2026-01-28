#include "ICAN.hpp"
#include <variant>
#include <span>

enum class SignalType {
    UINT8,
    UINT16,
    UINT32,
    INT8,
    INT16,
    INT32,
    FLOAT
};

enum class Endianness {
    littleEndian,
    bigEndian
}

class CAN_Signal {
public:
    CAN_Signal(SignalType signalType, uint8_t startBit, uint8_t length, double factor, double offset, bool isSigned, Endianness endian) :
    _sType(signalType), _startBit(startBit), _length(length), _factor(factor), _offset(offset), _isSigned(isSigned), _endian(endian) {};

    SignalType s();
    uint8_t startBit();
    uint8_t length();
    uint8_t factor();
    uint8_t offset();
    bool    isSigned(); 

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
    using SignalValue = std::variant<uint8_t, uint16_t, uint32_t, int8_t,  int16_t,  int32_t, float>;

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
        return uint32_t{0};
    }

    void storePhysicalIntoValue(double phys);

    SignalType  _sType;
    uint8_t     _startBit;
    uint8_t     _length;
    double     _factor;
    double     _offset;
    bool        _isSigned;
    Endianness _endian;
    SignalValue _sValue;
};

template<size_t num_signals>
class CAN_Message {
public:
    CAN_Message(uint32_t id, bool extended, uint8_t length, std::array<Signal, NumSignals> signals);

    uint32_t id();
    uint8_t length();
    bool extended();

    bool decode_from(const CAN_Frame& frame) const;

    CAN_Frame encode_to_frame() const;

private:
    std::array<CAN_Signal, num_signals> _signals;
    uint32_t _id;
    uint8_t _length;
    bool extended;
};

class CAN_Bus {
    ICAN& _can;
public:
    CAN_Bus(ICAN& can) : _can(can) {};

    void tick_bus();
};
