#include <cstdint>

enum class GpioLevel : uint8_t { G_LOW = 0, G_HIGH = 1, G_UNDEF = 2};

struct IGpio {
    virtual ~IGpio() = default;

    virtual void gpio_write(GpioLevel level) = 0;

    virtual GpioLevel gpio_read() = 0;
};
