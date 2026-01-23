#include <Arduino.h>
#include <cstring>
#include <virtualTimer.h>
#include "../include/MCP2515.hpp"
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

  bitRateConfig cfg = { 0x00, 0x91, 0x01 };
  mcp2515.begin(cfg);
  debugTimerGroup.AddTimer(10000, printMissCounter);

}


void loop() {
  // put your main code here, to run repeatedly:
  debugTimerGroup.Tick(millis());
  CAN_Message msg;
  mcp2515.recv(msg);

  mcp2515.updateMissCounter();


  std::array<uint8_t, 8> buf = { 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD };
  CAN_Message msg_send(0, 8, buf, false);

  mcp2515.send(msg_send);

  delay(1);
}


