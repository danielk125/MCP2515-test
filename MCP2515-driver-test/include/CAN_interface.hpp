#include "ICAN.hpp"

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
    SignalType& s;
    uint8_t _startBit;
    uint8_t _length;
    uint8_t _factor;
    uint8_t _offset;

    CAN_Signal(SignalType& signalType, uint8_t startBit, uint8_t length, uint8_t factor, uint8_t offset) :
    s(signalType), _startBit(startBit), _length(length), _factor(factor), _offset(offset) {};
};

class CAN_Bus {
    ICAN& _can;
public:
    CAN_Bus(ICAN& can) : _can(can) {};

    void tick_bus();
};
