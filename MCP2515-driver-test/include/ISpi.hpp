#include <cstddef>
#include <cstdint>

class ISpi {
    virtual ~ISpi() = default;

    virtual bool ISpi_transfer(const uint8_t* tx, uint8_t* rx, size_t len) = 0;

    virtual bool ISpi_write(const uint8_t* tx, size_t len) = 0;
};