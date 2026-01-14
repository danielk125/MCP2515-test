#include "../include/MCP2515.hpp"
#include <cstring>

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

bool MCP2515::probe(){
    select(false);
    if (!reset()) return false;


    if (!bitModify(REG_CANCTRL, CANCTRL_REQOP_MASK, CANCTRL_MODE_CONFIG)) return false;

    uint8_t out;
    if (!readRegister(REG_CANSTAT, out)) return false;

    return (out & CANCTRL_REQOP_MASK) == CANCTRL_MODE_CONFIG;
}
