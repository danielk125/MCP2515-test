MakeSignalSigned(float, 0, 16, 0.01, 0.0, True) gen_amps;
MakeSignalSigned(float, 16, 8, 0.01, 0.0, True) front_fan_amps;
MakeSignalSigned(float, 24, 8, 0.01, 0.0, True) rear_fan_amps;
MakeSignalSigned(float, 32, 8, 0.01, 0.0, True) front_pump_amps;
MakeSignalSigned(float, 40, 8, 0.01, 0.0, True) rear_pump_amps;
CAN_MESSAGE<5> {drive_bus, 0x2A1, Gen_Amps, Front_Fan_Amps, Rear_Fan_Amps, Front_Pump_Amps, Rear_Pump_Amps}};

MakeSignalSigned(float, 0, 16, 0.01, 0.0, True) bat_volt;
MakeSignalSigned(bool, 16, 8, 1.0, 0.0, True) bat_volt_warning;
CAN_MESSAGE<2> {drive_bus, 0x2A2, Bat_Volt, Bat_Volt_Warning}};

MakeSignalSigned(int16_t, 0, 16, 1.0, 0.0, True) rpm;
MakeSignalSigned(int16_t, 16, 16, 0.1, 0.0, True) motor_current;
MakeSignalSigned(int16_t, 32, 16, 0.1, 0.0, True) dc_voltage;
MakeSignalSigned(int16_t, 48, 16, 0.1, 0.0, True) dc_current;
CAN_MESSAGE<4> {drive_bus, 0x281, RPM, Motor_Current, DC_Voltage, DC_Current}};

MakeSignalSigned(int16_t, 0, 16, 0.1, 0.0, True) igbt_temp;
MakeSignalSigned(int16_t, 16, 16, 0.1, 0.0, True) motor_temp;
CAN_MESSAGE<2> {drive_bus, 0x282, IGBT_Temp, Motor_Temp}};

MakeSignal(uint32_t, 32, 32, 0.0001, 0.0) ah_charged;
CAN_MESSAGE<1> {drive_bus, nan, Ah_Charged}};

MakeSignal(uint32_t, 0, 32, 0.001, 0.0) wh_drawn;
MakeSignal(uint32_t, 32, 32, 0.001, 0.0) wh_charged;
CAN_MESSAGE<2> {drive_bus, 0x284, Wh_Drawn, Wh_Charged}};

MakeSignal(uint8_t, 0, 8, 1.0, 0.0) fault_code;
CAN_MESSAGE<1> {drive_bus, 0x280, Fault_Code}};

MakeSignalSigned(int32_t, 0, 32, 0.001, 0.0, True) set_current;
CAN_MESSAGE<1> {drive_bus, 0x200, Set_Current}};

MakeSignalSigned(int32_t, 0, 32, 0.001, 0.0, True) set_current_brake;
CAN_MESSAGE<1> {drive_bus, 0x201, Set_Current_Brake}};

MakeSignalSigned(int16_t, 0, 16, 1.0, 0.0, True) apps1_throttle;
MakeSignalSigned(int16_t, 16, 16, 1.0, 0.0, True) apps2_throttle;
CAN_MESSAGE<2> {drive_bus, 0x202, APPS1_Throttle, APPS2_Throttle}};

MakeSignalSigned(int16_t, 0, 16, 1.0, 0.0, True) front_brake_pressure;
MakeSignalSigned(int16_t, 16, 16, 1.0, 0.0, True) rear_brake_pressure;
MakeSignalSigned(bool, 32, 8, 1.0, 0.0, True) brake_pressed;
CAN_MESSAGE<3> {drive_bus, 0x203, Front_Brake_Pressure, Rear_Brake_Pressure, Brake_Pressed}};

MakeSignalSigned(bool, 0, 8, 1.0, 0.0, True) implausibility_present;
MakeSignalSigned(bool, 8, 8, 1.0, 0.0, True) appss_disagreement_imp;
MakeSignalSigned(bool, 16, 8, 1.0, 0.0, True) bppc_imp;
MakeSignalSigned(bool, 24, 8, 1.0, 0.0, True) brake_invalid_imp;
MakeSignalSigned(bool, 32, 8, 1.0, 0.0, True) appss_invalid_imp;
CAN_MESSAGE<5> {drive_bus, 0x204, Implausibility_Present, APPSs_Disagreement_Imp, BPPC_Imp, Brake_Invalid_Imp, APPSs_Invalid_Imp}};

MakeSignal(uint8_t, 0, 8, 1.0, 0.0) bms_command;
CAN_MESSAGE<1> {drive_bus, 0x205, BMS_Command}};

MakeSignal(uint8_t, 0, 8, 1.0, 0.0) drive_state;
CAN_MESSAGE<1> {drive_bus, 0x206, Drive_State}};

MakeSignalSigned(bool, 0, 1, 1.0, 0.0, True) active_aero_state;
MakeSignalSigned(int16_t, 1, 16, 1.0, 0.0, True) active_aero_position;
CAN_MESSAGE<2> {drive_bus, 0x208, Active_Aero_State, Active_Aero_Position}};

MakeSignal(uint8_t, 0, 8, 1.0, 0.0) front_pump_duty_cycle;
MakeSignal(uint8_t, 8, 8, 1.0, 0.0) rear_pump_duty_cycle;
MakeSignal(uint8_t, 16, 8, 1.0, 0.0) front_fan_duty_cycle;
MakeSignal(uint8_t, 24, 8, 1.0, 0.0) rear_pump_duty_cycle;
CAN_MESSAGE<4> {drive_bus, 0x209, Front_Pump_Duty_Cycle, Rear_Pump_Duty_Cycle, Front_Fan_Duty_Cycle, Rear_Pump_Duty_Cycle}};

MakeSignal(uint8_t, 0, 8, 1.0, 0.0) accel_lut_id_response;
CAN_MESSAGE<1> {drive_bus, 0x20A, Accel_LUT_Id_Response}};

MakeSignalSigned(bool, 0, 1, 1.0, 0.0, True) igbt_temp_limiting;
MakeSignalSigned(bool, 1, 1, 1.0, 0.0, True) battery_temp_limiting;
MakeSignalSigned(bool, 2, 1, 1.0, 0.0, True) motor_temp_limiting;
CAN_MESSAGE<3> {drive_bus, 0x20B, IGBT_Temp_Limiting, Battery_Temp_Limiting, Motor_Temp_Limiting}};

MakeSignal(uint8_t, 0, 8, 1.0, 0.0) torque_status;
CAN_MESSAGE<1> {drive_bus, 0x20C, Torque_Status}};

MakeSignal(uint8_t, 0, 8, 1.0, 0.0) ecu_enable_response;
MakeSignal(uint8_t, 8, 8, 1.0, 0.0) bms_enable_response;
MakeSignal(uint8_t, 16, 8, 1.0, 0.0) pdm_enable_response;
CAN_MESSAGE<3> {drive_bus, 0x490, ECU_Enable_Response, BMS_Enable_Response, PDM_Enable_Response}};

MakeSignal(uint8_t, 0, 8, 1.0, 0.0) dynamics_enable_response;
MakeSignal(uint8_t, 8, 8, 1.0, 0.0) front_enable_response;
MakeSignal(uint8_t, 16, 8, 1.0, 0.0) telemetry_enable_response;
CAN_MESSAGE<3> {drive_bus, 0x491, Dynamics_Enable_Response, Front_Enable_Response, Telemetry_Enable_Response}};

MakeSignal(uint8_t, 0, 8, 1.0, 0.0) bl_enable_response;
MakeSignal(uint8_t, 8, 8, 1.0, 0.0) br_enable_response;
MakeSignal(uint8_t, 16, 8, 1.0, 0.0) fl_enable_response;
MakeSignal(uint8_t, 24, 8, 1.0, 0.0) fr_enable_response;
CAN_MESSAGE<4> {drive_bus, 0x492, BL_Enable_Response, BR_Enable_Response, FL_Enable_Response, FR_Enable_Response}};

MakeSignal(uint64_t, 0, 64, 1.0, 0.0) counter;
CAN_MESSAGE<1> {drive_bus, 0x590, Counter}};

MakeSignal(uint64_t, 0, 64, 1.0, 0.0) counter;
CAN_MESSAGE<1> {drive_bus, 0x591, Counter}};

MakeSignal(uint64_t, 0, 64, 1.0, 0.0) counter;
CAN_MESSAGE<1> {drive_bus, 0x592, Counter}};

MakeSignal(uint64_t, 0, 64, 1.0, 0.0) counter;
CAN_MESSAGE<1> {data_bus, 0x593, Counter}};

MakeSignal(uint64_t, 0, 64, 1.0, 0.0) counter;
CAN_MESSAGE<1> {data_bus, 0x594, Counter}};

MakeSignal(uint64_t, 0, 64, 1.0, 0.0) counter;
CAN_MESSAGE<1> {data_bus, 0x595, Counter}};

MakeSignal(uint64_t, 0, 64, 1.0, 0.0) counter;
CAN_MESSAGE<1> {drive_bus, 0x596, Counter}};

MakeSignal(uint64_t, 0, 64, 1.0, 0.0) counter;
CAN_MESSAGE<1> {drive_bus, 0x597, Counter}};

MakeSignal(uint64_t, 0, 64, 1.0, 0.0) counter;
CAN_MESSAGE<1> {drive_bus, 0x598, Counter}};

MakeSignal(uint64_t, 0, 64, 1.0, 0.0) counter;
CAN_MESSAGE<1> {drive_bus, 0x599, Counter}};

MakeSignal(uint8_t, 0, 8, 1.0, 0.0) ecu_status;
MakeSignal(uint8_t, 8, 8, 1.0, 0.0) bms_status;
MakeSignal(uint8_t, 16, 8, 1.0, 0.0) pdm_status;
CAN_MESSAGE<3> {data_bus, 0x192, ECU_Status, BMS_Status, PDM_Status}};

MakeSignal(uint8_t, 0, 8, 1.0, 0.0) dynamics_status;
MakeSignal(uint8_t, 8, 8, 1.0, 0.0) front_status;
MakeSignal(uint8_t, 16, 8, 1.0, 0.0) telemetry_status;
CAN_MESSAGE<3> {data_bus, 0x193, Dynamics_Status, Front_Status, Telemetry_Status}};

MakeSignal(uint8_t, 0, 8, 1.0, 0.0) bl_status;
MakeSignal(uint8_t, 8, 8, 1.0, 0.0) br_status;
MakeSignal(uint8_t, 16, 8, 1.0, 0.0) fl_status;
MakeSignal(uint8_t, 24, 8, 1.0, 0.0) fr_status;
CAN_MESSAGE<4> {data_bus, 0x194, BL_Status, BR_Status, FL_Status, FR_Status}};

MakeSignal(unsigned_float, 0, 12, 0.1, 0.0) max_discharge_current;
MakeSignal(unsigned_float, 12, 12, 0.1, 0.0) max_regen_current;
MakeSignal(unsigned_float, 24, 16, 0.01, 0.0) battery_voltage;
MakeSignal(unsigned_float, 40, 8, 1.0, -40.0) battery_temperature;
MakeSignal(unsigned_float, 48, 16, 0.01, -100.0) battery_current;
CAN_MESSAGE<5> {drive_bus, 0x150, Max_Discharge_Current, Max_Regen_Current, Battery_Voltage, Battery_Temperature, Battery_Current}};

MakeSignalSigned(bool, 0, 1, 1.0, 0.0, True) fault_summary;
MakeSignalSigned(bool, 1, 1, 1.0, 0.0, True) undervoltage_fault;
MakeSignalSigned(bool, 2, 1, 1.0, 0.0, True) overvoltage_fault;
MakeSignalSigned(bool, 3, 1, 1.0, 0.0, True) undertemperature_fault;
MakeSignalSigned(bool, 4, 1, 1.0, 0.0, True) overtemperature_fault;
MakeSignalSigned(bool, 5, 1, 1.0, 0.0, True) overcurrent_fault;
MakeSignalSigned(bool, 6, 1, 1.0, 0.0, True) external_kill_fault;
MakeSignalSigned(bool, 7, 1, 1.0, 0.0, True) open_wire_fault;
MakeSignalSigned(bool, 8, 1, 1.0, 0.0, True) open_wire_temp_fault;
MakeSignalSigned(bool, 9, 1, 1.0, 0.0, True) pec_fault;
MakeSignal(uint8_t, 10, 8, 1.0, 0.0) total_pec_failures;
CAN_MESSAGE<11> {drive_bus, 0x151, Fault_Summary, Undervoltage_Fault, Overvoltage_Fault, Undertemperature_Fault, Overtemperature_Fault, Overcurrent_Fault, External_Kill_Fault, Open_Wire_Fault, Open_Wire_Temp_Fault, Pec_Fault, Total_PEC_Failures}};

MakeSignal(uint8_t, 0, 8, 1.0, 0.0) bms_state;
MakeSignal(uint8_t, 8, 8, 1.0, 0.0) imd_state;
MakeSignal(unsigned_float, 16, 8, 1.0, -40.0) max_cell_temp;
MakeSignal(unsigned_float, 24, 8, 1.0, -40.0) min_cell_temp;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) max_cell_voltage;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) min_cell_voltage;
MakeSignal(unsigned_float, 48, 8, 0.004, 0.0) bms_soc;
CAN_MESSAGE<7> {drive_bus, 0x152, BMS_State, IMD_State, Max_Cell_Temp, Min_Cell_Temp, Max_Cell_Voltage, Min_Cell_Voltage, BMS_SOC}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_0;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_1;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_2;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_3;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_4;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_5;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_6;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_0;
CAN_MESSAGE<8> {drive_bus, 0x153, Cell_V_0, Cell_V_1, Cell_V_2, Cell_V_3, Cell_V_4, Cell_V_5, Cell_V_6, Cell_OCV_Offset_0}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_7;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_8;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_9;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_10;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_11;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_12;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_13;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_1;
CAN_MESSAGE<8> {drive_bus, 0x154, Cell_V_7, Cell_V_8, Cell_V_9, Cell_V_10, Cell_V_11, Cell_V_12, Cell_V_13, Cell_OCV_Offset_1}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_14;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_15;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_16;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_17;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_18;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_19;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_20;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_2;
CAN_MESSAGE<8> {drive_bus, 0x155, Cell_V_14, Cell_V_15, Cell_V_16, Cell_V_17, Cell_V_18, Cell_V_19, Cell_V_20, Cell_OCV_Offset_2}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_21;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_22;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_23;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_24;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_25;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_26;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_27;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_3;
CAN_MESSAGE<8> {drive_bus, 0x156, Cell_V_21, Cell_V_22, Cell_V_23, Cell_V_24, Cell_V_25, Cell_V_26, Cell_V_27, Cell_OCV_Offset_3}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_28;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_29;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_30;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_31;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_32;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_33;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_34;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_4;
CAN_MESSAGE<8> {drive_bus, 0x157, Cell_V_28, Cell_V_29, Cell_V_30, Cell_V_31, Cell_V_32, Cell_V_33, Cell_V_34, Cell_OCV_Offset_4}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_35;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_36;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_37;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_38;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_39;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_40;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_41;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_5;
CAN_MESSAGE<8> {drive_bus, 0x158, Cell_V_35, Cell_V_36, Cell_V_37, Cell_V_38, Cell_V_39, Cell_V_40, Cell_V_41, Cell_OCV_Offset_5}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_42;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_43;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_44;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_45;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_46;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_47;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_48;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_6;
CAN_MESSAGE<8> {drive_bus, 0x159, Cell_V_42, Cell_V_43, Cell_V_44, Cell_V_45, Cell_V_46, Cell_V_47, Cell_V_48, Cell_OCV_Offset_6}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_49;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_50;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_51;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_52;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_53;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_54;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_55;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_7;
CAN_MESSAGE<8> {drive_bus, 0x15A, Cell_V_49, Cell_V_50, Cell_V_51, Cell_V_52, Cell_V_53, Cell_V_54, Cell_V_55, Cell_OCV_Offset_7}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_56;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_57;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_58;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_59;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_60;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_61;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_62;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_8;
CAN_MESSAGE<8> {drive_bus, 0x15B, Cell_V_56, Cell_V_57, Cell_V_58, Cell_V_59, Cell_V_60, Cell_V_61, Cell_V_62, Cell_OCV_Offset_8}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_63;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_64;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_65;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_66;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_67;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_68;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_69;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_9;
CAN_MESSAGE<8> {drive_bus, 0x15C, Cell_V_63, Cell_V_64, Cell_V_65, Cell_V_66, Cell_V_67, Cell_V_68, Cell_V_69, Cell_OCV_Offset_9}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_70;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_71;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_72;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_73;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_74;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_75;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_76;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_10;
CAN_MESSAGE<8> {drive_bus, 0x15D, Cell_V_70, Cell_V_71, Cell_V_72, Cell_V_73, Cell_V_74, Cell_V_75, Cell_V_76, Cell_OCV_Offset_10}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_77;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_78;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_79;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_80;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_81;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_82;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_83;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_11;
CAN_MESSAGE<8> {drive_bus, 0x15E, Cell_V_77, Cell_V_78, Cell_V_79, Cell_V_80, Cell_V_81, Cell_V_82, Cell_V_83, Cell_OCV_Offset_11}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_84;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_85;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_86;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_87;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_88;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_89;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_90;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_12;
CAN_MESSAGE<8> {drive_bus, 0x15F, Cell_V_84, Cell_V_85, Cell_V_86, Cell_V_87, Cell_V_88, Cell_V_89, Cell_V_90, Cell_OCV_Offset_12}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_91;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_92;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_93;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_94;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_95;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_96;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_97;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_13;
CAN_MESSAGE<8> {drive_bus, 0x160, Cell_V_91, Cell_V_92, Cell_V_93, Cell_V_94, Cell_V_95, Cell_V_96, Cell_V_97, Cell_OCV_Offset_13}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_98;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_99;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_100;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_101;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_102;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_103;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_104;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_14;
CAN_MESSAGE<8> {drive_bus, 0x161, Cell_V_98, Cell_V_99, Cell_V_100, Cell_V_101, Cell_V_102, Cell_V_103, Cell_V_104, Cell_OCV_Offset_14}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_105;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_106;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_107;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_108;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_109;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_110;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_111;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_15;
CAN_MESSAGE<8> {drive_bus, 0x162, Cell_V_105, Cell_V_106, Cell_V_107, Cell_V_108, Cell_V_109, Cell_V_110, Cell_V_111, Cell_OCV_Offset_15}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_112;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_113;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_114;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_115;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_116;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_117;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_118;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_16;
CAN_MESSAGE<8> {drive_bus, 0x163, Cell_V_112, Cell_V_113, Cell_V_114, Cell_V_115, Cell_V_116, Cell_V_117, Cell_V_118, Cell_OCV_Offset_16}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_119;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_120;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_121;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_122;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_123;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_124;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_125;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_17;
CAN_MESSAGE<8> {drive_bus, 0x164, Cell_V_119, Cell_V_120, Cell_V_121, Cell_V_122, Cell_V_123, Cell_V_124, Cell_V_125, Cell_OCV_Offset_17}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_126;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_127;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_128;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_129;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_130;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_131;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_132;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_18;
CAN_MESSAGE<8> {drive_bus, 0x165, Cell_V_126, Cell_V_127, Cell_V_128, Cell_V_129, Cell_V_130, Cell_V_131, Cell_V_132, Cell_OCV_Offset_18}};

MakeSignal(unsigned_float, 0, 8, 0.012, 2.0) cell_v_133;
MakeSignal(unsigned_float, 8, 8, 0.012, 2.0) cell_v_134;
MakeSignal(unsigned_float, 16, 8, 0.012, 2.0) cell_v_135;
MakeSignal(unsigned_float, 24, 8, 0.012, 2.0) cell_v_136;
MakeSignal(unsigned_float, 32, 8, 0.012, 2.0) cell_v_137;
MakeSignal(unsigned_float, 40, 8, 0.012, 2.0) cell_v_138;
MakeSignal(unsigned_float, 48, 8, 0.012, 2.0) cell_v_139;
MakeSignal(unsigned_float, 56, 8, 0.004, 0.0) cell_ocv_offset_19;
CAN_MESSAGE<8> {drive_bus, 0x166, Cell_V_133, Cell_V_134, Cell_V_135, Cell_V_136, Cell_V_137, Cell_V_138, Cell_V_139, Cell_OCV_Offset_19}};

MakeSignal(unsigned_float, 0, 8, 1.0, -40.0) cell_t_0;
MakeSignal(unsigned_float, 8, 8, 1.0, -40.0) cell_t_1;
MakeSignal(unsigned_float, 16, 8, 1.0, -40.0) cell_t_2;
MakeSignal(unsigned_float, 24, 8, 1.0, -40.0) cell_t_3;
MakeSignal(unsigned_float, 32, 8, 1.0, -40.0) cell_t_4;
MakeSignal(unsigned_float, 40, 8, 1.0, -40.0) cell_t_5;
MakeSignal(unsigned_float, 48, 8, 1.0, -40.0) cell_t_6;
MakeSignal(unsigned_float, 56, 8, 1.0, -40.0) cell_t_7;
CAN_MESSAGE<8> {drive_bus, 0x167, Cell_T_0, Cell_T_1, Cell_T_2, Cell_T_3, Cell_T_4, Cell_T_5, Cell_T_6, Cell_T_7}};

MakeSignal(unsigned_float, 0, 8, 1.0, -40.0) cell_t_8;
MakeSignal(unsigned_float, 8, 8, 1.0, -40.0) cell_t_9;
MakeSignal(unsigned_float, 16, 8, 1.0, -40.0) cell_t_10;
MakeSignal(unsigned_float, 24, 8, 1.0, -40.0) cell_t_11;
MakeSignal(unsigned_float, 32, 8, 1.0, -40.0) cell_t_12;
MakeSignal(unsigned_float, 40, 8, 1.0, -40.0) cell_t_13;
MakeSignal(unsigned_float, 48, 8, 1.0, -40.0) cell_t_14;
MakeSignal(unsigned_float, 56, 8, 1.0, -40.0) cell_t_15;
CAN_MESSAGE<8> {drive_bus, 0x168, Cell_T_8, Cell_T_9, Cell_T_10, Cell_T_11, Cell_T_12, Cell_T_13, Cell_T_14, Cell_T_15}};

MakeSignal(unsigned_float, 0, 8, 1.0, -40.0) cell_t_16;
MakeSignal(unsigned_float, 8, 8, 1.0, -40.0) cell_t_17;
MakeSignal(unsigned_float, 16, 8, 1.0, -40.0) cell_t_18;
MakeSignal(unsigned_float, 24, 8, 1.0, -40.0) cell_t_19;
MakeSignal(unsigned_float, 32, 8, 1.0, -40.0) cell_t_20;
MakeSignal(unsigned_float, 40, 8, 1.0, -40.0) cell_t_21;
MakeSignal(unsigned_float, 48, 8, 1.0, -40.0) cell_t_22;
MakeSignal(unsigned_float, 56, 8, 1.0, -40.0) cell_t_23;
CAN_MESSAGE<8> {drive_bus, 0x169, Cell_T_16, Cell_T_17, Cell_T_18, Cell_T_19, Cell_T_20, Cell_T_21, Cell_T_22, Cell_T_23}};

MakeSignal(unsigned_float, 0, 8, 1.0, -40.0) cell_t_24;
MakeSignal(unsigned_float, 8, 8, 1.0, -40.0) cell_t_25;
MakeSignal(unsigned_float, 16, 8, 1.0, -40.0) cell_t_26;
MakeSignal(unsigned_float, 24, 8, 1.0, -40.0) cell_t_27;
MakeSignal(unsigned_float, 32, 8, 1.0, -40.0) cell_t_28;
MakeSignal(unsigned_float, 40, 8, 1.0, -40.0) cell_t_29;
MakeSignal(unsigned_float, 48, 8, 1.0, -40.0) cell_t_30;
MakeSignal(unsigned_float, 56, 8, 1.0, -40.0) cell_t_31;
CAN_MESSAGE<8> {drive_bus, 0x16A, Cell_T_24, Cell_T_25, Cell_T_26, Cell_T_27, Cell_T_28, Cell_T_29, Cell_T_30, Cell_T_31}};

MakeSignal(unsigned_float, 0, 8, 1.0, -40.0) cell_t_32;
MakeSignal(unsigned_float, 8, 8, 1.0, -40.0) cell_t_33;
MakeSignal(unsigned_float, 16, 8, 1.0, -40.0) cell_t_34;
MakeSignal(unsigned_float, 24, 8, 1.0, -40.0) cell_t_35;
MakeSignal(unsigned_float, 32, 8, 1.0, -40.0) cell_t_36;
MakeSignal(unsigned_float, 40, 8, 1.0, -40.0) cell_t_37;
MakeSignal(unsigned_float, 48, 8, 1.0, -40.0) cell_t_38;
MakeSignal(unsigned_float, 56, 8, 1.0, -40.0) cell_t_39;
CAN_MESSAGE<8> {drive_bus, 0x16B, Cell_T_32, Cell_T_33, Cell_T_34, Cell_T_35, Cell_T_36, Cell_T_37, Cell_T_38, Cell_T_39}};

MakeSignal(unsigned_float, 0, 8, 1.0, -40.0) cell_t_40;
MakeSignal(unsigned_float, 8, 8, 1.0, -40.0) cell_t_41;
MakeSignal(unsigned_float, 16, 8, 1.0, -40.0) cell_t_42;
MakeSignal(unsigned_float, 24, 8, 1.0, -40.0) cell_t_43;
MakeSignal(unsigned_float, 32, 8, 1.0, -40.0) cell_t_44;
MakeSignal(unsigned_float, 40, 8, 1.0, -40.0) cell_t_45;
MakeSignal(unsigned_float, 48, 8, 1.0, -40.0) cell_t_46;
MakeSignal(unsigned_float, 56, 8, 1.0, -40.0) cell_t_47;
CAN_MESSAGE<8> {drive_bus, 0x16C, Cell_T_40, Cell_T_41, Cell_T_42, Cell_T_43, Cell_T_44, Cell_T_45, Cell_T_46, Cell_T_47}};

MakeSignal(unsigned_float, 0, 8, 1.0, -40.0) cell_t_48;
MakeSignal(unsigned_float, 8, 8, 1.0, -40.0) cell_t_49;
MakeSignal(unsigned_float, 16, 8, 1.0, -40.0) cell_t_50;
MakeSignal(unsigned_float, 24, 8, 1.0, -40.0) cell_t_51;
MakeSignal(unsigned_float, 32, 8, 1.0, -40.0) cell_t_52;
MakeSignal(unsigned_float, 40, 8, 1.0, -40.0) cell_t_53;
MakeSignal(unsigned_float, 48, 8, 1.0, -40.0) cell_t_54;
MakeSignal(unsigned_float, 56, 8, 1.0, -40.0) cell_t_55;
CAN_MESSAGE<8> {drive_bus, 0x16D, Cell_T_48, Cell_T_49, Cell_T_50, Cell_T_51, Cell_T_52, Cell_T_53, Cell_T_54, Cell_T_55}};

MakeSignal(unsigned_float, 0, 8, 1.0, -40.0) cell_t_56;
MakeSignal(unsigned_float, 8, 8, 1.0, -40.0) cell_t_57;
MakeSignal(unsigned_float, 16, 8, 1.0, -40.0) cell_t_58;
MakeSignal(unsigned_float, 24, 8, 1.0, -40.0) cell_t_59;
MakeSignal(unsigned_float, 32, 8, 1.0, -40.0) cell_t_60;
MakeSignal(unsigned_float, 40, 8, 1.0, -40.0) cell_t_61;
MakeSignal(unsigned_float, 48, 8, 1.0, -40.0) cell_t_62;
MakeSignal(unsigned_float, 56, 8, 1.0, -40.0) cell_t_63;
CAN_MESSAGE<8> {drive_bus, 0x16E, Cell_T_56, Cell_T_57, Cell_T_58, Cell_T_59, Cell_T_60, Cell_T_61, Cell_T_62, Cell_T_63}};

MakeSignal(unsigned_float, 0, 8, 1.0, -40.0) cell_t_64;
MakeSignal(unsigned_float, 8, 8, 1.0, -40.0) cell_t_65;
MakeSignal(unsigned_float, 16, 8, 1.0, -40.0) cell_t_66;
MakeSignal(unsigned_float, 24, 8, 1.0, -40.0) cell_t_67;
MakeSignal(unsigned_float, 32, 8, 1.0, -40.0) cell_t_68;
MakeSignal(unsigned_float, 40, 8, 1.0, -40.0) cell_t_69;
MakeSignal(unsigned_float, 48, 8, 1.0, -40.0) cell_t_70;
MakeSignal(unsigned_float, 56, 8, 1.0, -40.0) cell_t_71;
CAN_MESSAGE<8> {drive_bus, 0x16F, Cell_T_64, Cell_T_65, Cell_T_66, Cell_T_67, Cell_T_68, Cell_T_69, Cell_T_70, Cell_T_71}};

MakeSignal(unsigned_float, 0, 8, 1.0, -40.0) cell_t_72;
MakeSignal(unsigned_float, 8, 8, 1.0, -40.0) cell_t_73;
MakeSignal(unsigned_float, 16, 8, 1.0, -40.0) cell_t_74;
MakeSignal(unsigned_float, 24, 8, 1.0, -40.0) cell_t_75;
MakeSignal(unsigned_float, 32, 8, 1.0, -40.0) cell_t_76;
MakeSignal(unsigned_float, 40, 8, 1.0, -40.0) cell_t_77;
MakeSignal(unsigned_float, 48, 8, 1.0, -40.0) cell_t_78;
MakeSignal(unsigned_float, 56, 8, 1.0, -40.0) cell_t_79;
CAN_MESSAGE<8> {drive_bus, 0x170, Cell_T_72, Cell_T_73, Cell_T_74, Cell_T_75, Cell_T_76, Cell_T_77, Cell_T_78, Cell_T_79}};

MakeSignal(uint64_t, 0, 64, 1.0, 0.0) controller_error;
CAN_MESSAGE<1> {data_bus, 0x004, Controller_Error}};

MakeSignal(uint64_t, 0, 64, 1.0, 0.0) controller_error_nack;
CAN_MESSAGE<1> {data_bus, 0x024, Controller_Error_NACK}};

MakeSignal(uint64_t, 0, 64, 1.0, 0.0) controller_error_protocol_violation;
CAN_MESSAGE<1> {data_bus, 0x00C, Controller_Error_Protocol_Violation}};

