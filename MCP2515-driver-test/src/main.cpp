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
CAN_Bus drive_bus(mcp2515);
VirtualTimerGroup group;

CAN_Signal_FLOAT Gen_Amps = MakeSignal(float, 0, 16, 0.01, 0);
CAN_Signal_FLOAT Front_Fan_Amps = MakeSignal(float, 16, 8, 0.01, 0);
CAN_Signal_FLOAT Rear_Fan_Amps = MakeSignal(float, 24, 8, 0.01, 0);
CAN_Signal_FLOAT Front_Pump_Amps = MakeSignal(float, 32, 8, 0.01, 0);
CAN_Signal_FLOAT Rear_Pump_Amps = MakeSignal(float, 40, 8, 0.01, 0);

CAN_Signal_INT16 Inverter_Temp = MakeSignal(int16_t, 0, 16, 0.1, 0);
CAN_Signal_INT16 Motor_Temp = MakeSignal(int16_t, 16, 16, 0.1, 0);

CAN_Signal_INT16 RPM = MakeSignal(int16_t, 0, 16, 1, 0);
CAN_Signal_INT16 Motor_Current = MakeSignal(int16_t, 16, 16, 0.1, 0);
CAN_Signal_INT16 DC_Voltage = MakeSignal(int16_t, 32, 16, 0.1, 0);
CAN_Signal_INT16 DC_Current = MakeSignal(int16_t, 48, 16, 0.1, 0);

CAN_Signal_INT16 APPS1_Throttle = MakeSignal(int16_t, 0, 16, 1, 0);
CAN_Signal_INT16 APPS2_Throttle = MakeSignal(int16_t, 16, 16, 1, 0);

void pdm_current_recv() {
  float gav = Gen_Amps->get();
  float ffv = Front_Fan_Amps->get();
  float rfv = Rear_Fan_Amps->get();
  float fpv = Front_Pump_Amps->get();
  float rpv = Rear_Pump_Amps->get();
  
  Serial.printf("PDM Current Values: %f, %f, %f, %f, %f\n", gav, ffv, rfv, fpv, rpv);
}

void motor_status_recv() {
  APPS1_Throttle->set(APPS1_Throttle->get() + 1);

  int16_t rpm = RPM->get();
  int16_t mc = Motor_Current->get();
  int16_t dcv = DC_Voltage->get();
  int16_t dcc = DC_Current->get();

  Serial.printf("Motor Status Values: %d, %d, %d, %d\n", rpm, mc, dcv, dcc);
}

RX_CAN_Message_Callback(4) motor_status(drive_bus, 0x281, false, 8, motor_status_recv, RPM, Motor_Current, DC_Voltage, DC_Current);
RX_CAN_Message(2) temp_status(drive_bus, 0x282, false, 4, Inverter_Temp, Motor_Temp);
RX_CAN_Message_Callback(5) pdm_current(drive_bus, 0x2A1, false, 6, pdm_current_recv, Gen_Amps, Front_Fan_Amps, Rear_Fan_Amps, Front_Pump_Amps, Rear_Pump_Amps);
TX_CAN_Message(2) ECU_Throttle(drive_bus, 0x202, false, 4, 1000, group, APPS1_Throttle, APPS2_Throttle);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(200);
  spi.ESPSpi_init();

  BaudRate baud500k = BaudRate::kBaud500K;
  mcp2515.begin(baud500k);

  APPS1_Throttle->set(33);
  APPS2_Throttle->set(55);

}

void loop() {
  // put your main code here, to run repeatedly:
  group.Tick(millis());
  drive_bus.tick_bus();
}


