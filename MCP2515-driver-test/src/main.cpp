#include <Arduino.h>
#include "../include/MCP2515.hpp"
#include "../include/platform/ESP/ESPClock.hpp"
#include "../include/platform/ESP/ESPGpio.hpp"
#include "../include/platform/ESP/ESPSpi.hpp"

ESPSpi spi(1000000);
ESPGpio gpio(2, GpioMode::G_OUTPUT);
ESPClock clock_esp;
MCP2515 mcp2515(spi, gpio, clock_esp);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(200);
  spi.ESPSpi_init();
  bool ok = mcp2515.probe();
  Serial.printf("MCP2515 probe: %s\n", ok ? "OK" : "FAIL");
}

void loop() {
  // put your main code here, to run repeatedly:
}


