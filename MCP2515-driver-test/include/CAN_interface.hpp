#include "ICAN.hpp"
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

class CAN_Signal {
public:
    CAN_Signal(SignalType& signalType, uint8_t startBit, uint8_t length, uint8_t factor, uint8_t offset, bool isSigned) :
    _s(signalType), _startBit(startBit), _length(length), _factor(factor), _offset(offset), _isSigned(isSigned) {};

    SignalType s();
    uint8_t startBit();
    uint8_t length();
    uint8_t factor();
    uint8_t offset();
    bool    isSigned();  

    void decode(std::span<const uint8_t> data) const;
    void encode(std::span<uint8_t> data) const;

private:
    SignalType  _s;
    uint8_t     _startBit;
    uint8_t     _length;
    uint8_t     _factor;
    uint8_t     _offset;
    bool        _isSigned;


};

template<size_t num_signals>
class CAN_Message {
public:
    CAN_Message(uint32_t id, uint8_t length, std::array<Signal, NumSignals> signals, bool extended = false)

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
