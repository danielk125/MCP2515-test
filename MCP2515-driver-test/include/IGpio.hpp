#include <cstdint>

enum class GpioLevel : uint8_t { LOW = 0, HIGH = 1, UNDEF = 2};

struct IGpio {
    virtual ~IGpio() = default;

    virtual void gpio_write(GpioLevel level) = 0;

    virtual GpioLevel gpio_read() = 0;
};
