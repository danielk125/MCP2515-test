#include <Arduino.h>
#include <cstring>
#include "../include/MCP2515.hpp"
#include "../include/platform/ESP/ESPClock.hpp"
#include "../include/platform/ESP/ESPGpio.hpp"
#include "../include/platform/ESP/ESPSpi.hpp"

ESPSpi spi(1000000);
ESPGpio gpio(SS, GpioMode::G_OUTPUT);
ESPClock clock_esp;
MCP2515 mcp2515(spi, gpio, clock_esp);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(200);
  spi.ESPSpi_init();

  bitRateConfig cfg = { 0x00, 0x91, 0x01 };
  mcp2515.begin(cfg);
}

void loop() {
  // put your main code here, to run repeatedly:
  Frame fr;
  if (mcp2515.recv(fr)){
    for (uint8_t i = 0; i < fr.dlc; i++) {
      Serial.printf("%02X ", fr.data[i]);
    }
    Serial.println();
  } else {
    Serial.printf("Nothing to receive.\n");
  }

  uint8_t buf[8] = { 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD };
  Frame fr_send;
  fr_send.id = 0;
  fr_send.extended = false;
  fr_send.rtr = false;
  fr_send.dlc = 8;
  std::memcpy(fr_send.data, buf, 8);

  if (mcp2515.send(fr_send)){
    Serial.printf("Sent value\n");
  } else {
    Serial.printf("Send failed\n");
  }
  delay(1000);
}


