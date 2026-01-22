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
  Frame fr;
  mcp2515.recv(fr);

  mcp2515.updateMissCounter();


  uint8_t buf[8] = { 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD };
  Frame fr_send;
  fr_send.id = 0;
  fr_send.extended = false;
  fr_send.rtr = false;
  fr_send.dlc = 8;
  std::memcpy(fr_send.data, buf, 8);

  mcp2515.send(fr_send);

  delay(1);
}


