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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(200);
  spi.ESPSpi_init();

  BaudRate baud500k = BaudRate::kBaud500K;
  mcp2515.begin(baud500k);

  CAN_Bus drive_bus(mcp2515);

  CAN_Signal_UINT32 test_sig_1 = MakeSignal(uint32_t, 0, 8, 0, 0);
  CAN_Signal_UINT8 test_sig_2 = MakeSignal(uint8_t, 8, 8, 0, 0);
  CAN_Signal_UINT8 test_sig_3 = MakeSignal(uint8_t, 16, 8, 0, 0);
  CAN_Signal_UINT8 test_sig_4 = MakeSignal(uint8_t, 24, 8, 0, 0);

  MakeRXCanMessage(4) test_msg(drive_bus, 0x001, false, 4, test_sig_1, test_sig_2, test_sig_3, test_sig_4);

  test_sig_1->set(5);
  auto val = test_sig_1->get();

}


void loop() {
  // put your main code here, to run repeatedly:


}


