#include <Arduino.h>
#include <cstring>
#include <virtualTimer.h>
#include "../include/MCP2515.hpp"
#include "../include/CAN_interface.hpp"
#include "../include/platform/ESP/ESPClock.hpp"
#include "../include/platform/ESP/ESPGpio.hpp"
#include "../include/platform/ESP/ESPSpi.hpp"


ESPSpi spi(1000000);
ESPGpio gpio(SS, GpioMode::G_OUTPUT);
ESPClock clock_esp;
MCP2515 mcp2515(spi, gpio, clock_esp);
VirtualTimerGroup debugTimerGroup;
uint8_t val = 0;
uint8_t new_val = 0;

void printMissCounter() {
  float missCounter = mcp2515.getMissCounter();
  Serial.printf("Missed Counter: %f\n", missCounter);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(200);
  spi.ESPSpi_init();

  BaudRate baud500k = BaudRate::kBaud500K;
  mcp2515.begin(baud500k);
  debugTimerGroup.AddTimer(10000, printMissCounter);

}


void loop() {
  // put your main code here, to run repeatedly:
  debugTimerGroup.Tick(millis());
  CAN_Frame fr;
  mcp2515.recv(fr);

  mcp2515.updateMissCounter();


  std::array<uint8_t, 8> buf = { 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD };
  CAN_Frame fr_send(0, 8, buf, false);

  mcp2515.send(fr);

  delay(1);
}


