#include <array>

struct CAN_Frame {

    uint32_t _id {};
    uint8_t _length {};
    std::array<uint8_t, 8> _data {};
    bool _extendedId {};

    CAN_Frame() {};

    CAN_Frame(uint32_t id, uint8_t length, std::array<uint8_t, 8> data, bool extended) :
        _id(id), _length(length), _data(data), _extendedId(extended) {}
};

struct ICAN {
    virtual ~ICAN() = default;

    virtual bool send(const CAN_Frame& msg) = 0;

    virtual bool recv(CAN_Frame& msg) = 0;
};
