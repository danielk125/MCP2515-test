#include "ICAN.hpp"



class CAN_Bus {
    ICAN& _can;
public:
    CAN_Bus(ICAN& can) : _can(can) {};

    void tick_bus();
};
