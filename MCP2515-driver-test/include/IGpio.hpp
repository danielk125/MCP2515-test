#include <cstdint>

enum class GpioLevel : uint8_t { LOW = 0, HIGH = 1};
enum class GpioMode : uint8_t { OUTPUT = 0, INPUT = 1};

class IGpio {
    virtual ~IGpio() = default;

    virtual void gpio_set_mode(GpioMode mode) = 0;

    virtual void gpio_write(GpioLevel level) = 0;

    virtual GpioLevel gpio_read() = 0;
};