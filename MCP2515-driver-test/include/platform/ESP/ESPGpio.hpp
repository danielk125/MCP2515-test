#ifndef IGPIO_HPP
#define IGPIO_HPP

#include "../../IGpio.hpp"
#include <Arduino.h>

#endif

enum class GpioMode : uint8_t { OUTPUT = 0, INPUT = 1};

class ESPGpio final : public IGpio {
    uint32_t _pin;
    GpioMode _mode;

public:
    ESPGpio(uint32_t pin, GpioMode mode) : _pin(pin), _mode(mode) {
        pinMode(_pin, mode == GpioMode::OUTPUT ? OUTPUT : INPUT);
    }

    void gpio_write(GpioLevel level) override {
        if (_mode == GpioMode::INPUT || level == GpioLevel::UNDEF) return;

        digitalWrite(_pin, level == GpioLevel::HIGH ? HIGH : LOW);
    }

    GpioLevel gpio_read() override {
        if (_mode == GpioMode::OUTPUT) return GpioLevel::UNDEF;

        return digitalRead(_pin) ? GpioLevel::HIGH : GpioLevel::LOW;
    }
};
