#include <cstdint>

struct I_Clock {
  virtual ~I_Clock() = default;
  virtual void sleepMs(uint32_t ms) = 0;
  virtual uint32_t monotonicMs() = 0;
};
