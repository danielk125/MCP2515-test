#include <cstdint>

struct IClock {
  virtual ~IClock() = default;
  virtual void sleepMs(uint32_t ms) = 0;
  virtual uint32_t monotonicMs() = 0;
};
