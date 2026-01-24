#include "../include/MCP2515.hpp"
#include <cstring>
#include <Arduino.h>


void MCP2515::select(bool set){
    _cs.gpio_write(set ? GpioLevel::G_LOW : GpioLevel::G_HIGH);
}

bool MCP2515::reset(){
    uint8_t cmd = CMD_RESET;

    select(true);
    bool ok = _spi.ISpi_write(&cmd, 1);
    select(false);

    _clock.sleepMs(10);
    return ok;
}

bool MCP2515::writeRegister(uint8_t addr, uint8_t val) {
    uint8_t data[3]{ CMD_WRITE, addr, val };

    select(true);
    bool ok = _spi.ISpi_write(data, 3);
    select(false);

    return ok;
}

bool MCP2515::readRegister(uint8_t addr, uint8_t& out) {
    uint8_t tx[3]{ CMD_READ, addr, 0x00 };
    uint8_t rx[3]{}; // needs to be same length to account for full-duplex

    select(true);
    bool ok = _spi.ISpi_transfer(tx, rx, 3);
    select(false);

    if (!ok) return false;
    out = rx[2];
    return true;
}

bool MCP2515::bitModify(uint8_t addr, uint8_t mask, uint8_t data) {
    uint8_t buf[4]{ CMD_BITMOD, addr, mask, data };

    select(true);
    bool ok = _spi.ISpi_write(buf, 4);
    select(false);

    return ok;
}

bool MCP2515::writeRegisters(uint8_t startAddr, const uint8_t* data, uint8_t len) {
    if (!data || len == 0) return false;

    uint8_t buf[2]{ CMD_WRITE, startAddr };

    select(true);
    bool ok = _spi.ISpi_write(buf, 2) && _spi.ISpi_write(data, len);
    select(false);

    return ok;
}

bool MCP2515::readRegisters(uint8_t startAddr, uint8_t* data, uint8_t len) {
    if (len == 0) return false;

    uint8_t tx[2 + 32]{};
    uint8_t rx[2 + 32]{};

    tx[0] = CMD_READ;
    tx[1] = startAddr;

    select(true);
    bool ok = _spi.ISpi_transfer(tx, rx, static_cast<size_t>(2+len));
    select(false);
    if (!ok) return false;

    std::memcpy(data, &rx[2], len);
    return true;
}

bool MCP2515::probe(std::string& error){
    select(false);
    if (!reset()){ 
        error = "reset failure"; 
        return false;
    } 


    if (!bitModify(REG_CANCTRL, CANCTRL_REQOP_MASK, CANCTRL_MODE_CONFIG)){
        error = "bitmode failure";
        return false;
    } 

    uint8_t out;
    if (!readRegister(REG_CANSTAT, out)){
        error = "read reg failure";
        return false;
    };

    char buf[64];
    snprintf(buf, sizeof(buf), "CANSTAT=0x%02X (want OPMOD=0x80)", out);
    error = buf;

    return (out & CANCTRL_REQOP_MASK) == CANCTRL_MODE_CONFIG;
}

void MCP2515::packId(uint32_t id, bool extended, uint8_t& sidh, uint8_t& sidl, uint8_t& eid8, uint8_t& eid0){
        if (!extended) {
        // 11-bit standard
        id &= 0x7FF;
        sidh = static_cast<uint8_t>(id >> 3);
        sidl = static_cast<uint8_t>((id & 0x7) << 5); // bits 7..5
        sidl &= 0xE0;
        eid8 = 0;
        eid0 = 0;
    } else {
        // 29-bit extended
        id &= 0x1FFFFFFF;
        sidh = static_cast<uint8_t>(id >> 21);
        sidl = static_cast<uint8_t>(((id >> 18) & 0x7) << 5);
        sidl |= 0x08; // IDE bit
        sidl |= static_cast<uint8_t>((id >> 16) & 0x3); // EID17..16 into bits 1..0
        eid8 = static_cast<uint8_t>((id >> 8) & 0xFF);
        eid0 = static_cast<uint8_t>(id & 0xFF);
    }
}

void MCP2515::unpackId(uint8_t sidh, uint8_t sidl, uint8_t eid8, uint8_t eid0, uint32_t& id, bool& extended){
    extended = (sidl & 0x08) != 0;
    if (!extended) {
        id = (static_cast<uint32_t>(sidh) << 3) | (static_cast<uint32_t>(sidl) >> 5);
        id &= 0x7FF;
    } else {
        uint32_t sid = (static_cast<uint32_t>(sidh) << 3) | ((static_cast<uint32_t>(sidl) >> 5) & 0x7);
        uint32_t eid = (static_cast<uint32_t>(sidl) & 0x3) << 16;
        eid |= static_cast<uint32_t>(eid8) << 8;
        eid |= static_cast<uint32_t>(eid0);
        id = (sid << 18) | eid;
        id &= 0x1FFFFFFF;
    }
}

bool MCP2515::baudRateToCNF(BaudRate baud, bitRateConfig& out) {
    switch(baud) {
        case BaudRate::kBaud125k: out = {0x01, 0xB1, 0x05}; return true;
        case BaudRate::kBaud250K: out = {0x00, 0xB1, 0x05}; return true;
        case BaudRate::kBaud500K: out = {0x00, 0x91, 0x01}; return true;
        case BaudRate::kBaud1M  : out = {0x00, 0x80, 0x00}; return true;
        default: return false;
    }
}

bool MCP2515::begin(const BaudRate baud) {
    bitRateConfig cfg;
    baudRateToCNF(baud, cfg);

    // CS should idle high for MCP2515
    select(false);

    if (!reset()) return false;

    // Enter config mode
    if (!bitModify(REG_CANCTRL, CANCTRL_REQOP_MASK, CANCTRL_MODE_CONFIG)) return false;
    _clock.sleepMs(2);

    // Program bit timing (YOU fill cfg.cnf1/2/3)
    if (!writeRegister(REG_CNF1, cfg.cnf1)) return false;
    if (!writeRegister(REG_CNF2, cfg.cnf2)) return false;
    if (!writeRegister(REG_CNF3, cfg.cnf3)) return false;

    // Accept all messages into RXB0 (RXM1:RXM0 = 11)
    // RXB0CTRL bits 6..5 = 11 => 0x60
    if (!writeRegister(REG_RXB0CTRL, 0x60)) return false;

    // Enable RX0 interrupt (RX0IE = bit0)
    if (!writeRegister(REG_CANINTE, 0x01)) return false;

    // Clear RX0IF (and others)
    if (!writeRegister(REG_CANINTF, 0x00)) return false;

    // Normal mode
    if (!bitModify(REG_CANCTRL, CANCTRL_REQOP_MASK, CANCTRL_MODE_NORMAL)) return false;
    _clock.sleepMs(2);

    return true;
}

bool MCP2515::send(const CAN_Frame& msg) {
  uint8_t txctrl = 0;
  if (!readRegister(REG_TXB0CTRL, txctrl)) return false;
  if (txctrl & TXBCTRL_TXREQ) {
    // busy
    return false;
  }

  uint8_t sidh=0, sidl=0, eid8=0, eid0=0;
  packId(msg._id, msg._extendedId, sidh, sidl, eid8, eid0);

  uint8_t dlc = (msg._length <= 8) ? msg._length : 8;
  uint8_t txdlc = dlc & 0x0F;

  uint8_t hdr[5]{ sidh, sidl, eid8, eid0, txdlc };
  if (!writeRegisters(REG_TXB0SIDH, hdr, 5)) return false;

  uint8_t buf[8];
  for (int i = 0; i < 8; i++){
    buf[i] = msg._data[i];
  }

  if (dlc > 0) {
    if (!writeRegisters(REG_TXB0D0, buf, dlc)) return false;
  }

  uint8_t cmd = CMD_RTS_TX0;
  select(true);
  bool ok = _spi.ISpi_write(&cmd, 1);
  select(false);
  return ok;
}

bool MCP2515::recv(CAN_Frame& msg) {
    uint8_t intf = 0;
    if (!readRegister(REG_CANINTF, intf)) return false;

    if ((intf & CANINTF_RX0IF) == 0) {
        return false;
    }

    uint8_t hdr[5] = {0};
    if (!readRegisters(REG_RXB0SIDH, hdr, 5)) return false;

    const uint8_t sidh = hdr[0];
    const uint8_t sidl = hdr[1];
    const uint8_t eid8 = hdr[2];
    const uint8_t eid0 = hdr[3];
    const uint8_t dlc_raw = hdr[4];

    msg._extendedId = (sidl & RXB0SIDL_IDE) != 0;
    msg._length = static_cast<uint8_t>(dlc_raw & RXB0DLC_LENMSK);
    if (msg._length > 8) msg._length = 8;

    unpackId(sidh, sidl, eid8, eid0, msg._id, msg._extendedId);

    // write to buffer
    uint8_t buf[8];
    if (msg._length > 0) {
        if (!readRegisters(REG_RXB0D0, buf, msg._length)) return false;
    }

    // write to message
    for (int i = 0; i < 8; i++){
        msg._data[i] = buf[i];
    }

    // CANINTF bits are cleared by writing 0 to the bit
    if (!bitModify(REG_CANINTF, CANINTF_RX0IF, 0x00)) return false;

    recvCount++;

    return true;
}

bool MCP2515::updateMissCounter(){
    // RX10VR
    uint8_t val;
    bool passed = readRegister(REG_EFLG, val);
    if (!passed) {
        return false;
    } 

    uint8_t shifted = val >> 6;

    if (shifted > 0) {
        missCounter++;
        bitModify(REG_EFLG, 0xC0, 0x00);
    }

    return true;
}

float MCP2515::getMissCounter() {
    if (missCounter + recvCount == 0) return 0;
    return missCounter/(float)(missCounter + recvCount);
}
