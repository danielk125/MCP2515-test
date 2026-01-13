#ifndef ICLOCK_HPP
#define ICLOCK_HPP

#include "../../IClock.hpp"
#include <Arduino.h>

#endif

struct ESPClock final : public IClock {
public:
  void sleepMs(uint32_t ms) override { delay(ms); }
  uint32_t monotonicMs() override { return millis(); }
};
