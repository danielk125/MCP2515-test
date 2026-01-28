#include "IClock.hpp"
#include "IGpio.hpp"
#include "ISpi.hpp"
#include "ICAN.hpp"

#include <string>

struct bitRateConfig {
    uint8_t cnf1;
    uint8_t cnf2;
    uint8_t cnf3;
};

enum class BaudRate {
    kBaud1M = 1000000,
    kBaud500K = 500000,
    kBaud250K = 250000,
    kBaud125k = 125000
};

class MCP2515 : ICAN {
private:
    ISpi& _spi;
    IGpio& _cs;
    IClock& _clock;
    uint32_t missCounter;
    uint32_t recvCount;
public:
    MCP2515(ISpi& spi, IGpio& cs, IClock& clock) : _spi(spi), _cs(cs), _clock(clock) {}

    bool begin(const BaudRate baud);

    bool send(const CAN_Frame& msg) override;

    bool recv(CAN_Frame& msg) override;

    uint32_t time_ms() override;

    // debug
    bool probe(std::string& error);
    bool updateMissCounter();
    float getMissCounter();

private:
    // MCP2515 Instructions
    static constexpr uint8_t CMD_RESET     = 0xC0;
    static constexpr uint8_t CMD_READ      = 0x03;
    static constexpr uint8_t CMD_WRITE     = 0x02;
    static constexpr uint8_t CMD_BITMOD    = 0x05;
    static constexpr uint8_t CMD_READSTAT  = 0xA0;
    static constexpr uint8_t CMD_RXSTAT    = 0xB0;
    static constexpr uint8_t CMD_RTS_TX0   = 0x81;

    // Registers (subset)
    static constexpr uint8_t REG_CANSTAT   = 0x0E;
    static constexpr uint8_t REG_CANCTRL   = 0x0F;
    static constexpr uint8_t REG_CNF3      = 0x28;
    static constexpr uint8_t REG_CNF2      = 0x29;
    static constexpr uint8_t REG_CNF1      = 0x2A;
    static constexpr uint8_t REG_CANINTE   = 0x2B;
    static constexpr uint8_t REG_CANINTF   = 0x2C;

    static constexpr uint8_t REG_TXB0CTRL  = 0x30;
    static constexpr uint8_t REG_TXB0SIDH  = 0x31;
    static constexpr uint8_t REG_TXB0SIDL  = 0x32;
    static constexpr uint8_t REG_TXB0EID8  = 0x33;
    static constexpr uint8_t REG_TXB0EID0  = 0x34;
    static constexpr uint8_t REG_TXB0DLC   = 0x35;
    static constexpr uint8_t REG_TXB0D0    = 0x36;

    static constexpr uint8_t REG_RXB0CTRL  = 0x60;
    static constexpr uint8_t REG_RXB0SIDH  = 0x61;
    static constexpr uint8_t REG_RXB0SIDL  = 0x62;
    static constexpr uint8_t REG_RXB0EID8  = 0x63;
    static constexpr uint8_t REG_RXB0EID0  = 0x64;
    static constexpr uint8_t REG_RXB0DLC   = 0x65;
    static constexpr uint8_t REG_RXB0D0    = 0x66;
    static constexpr uint8_t REG_EFLG      = 0x2D;
    // Bits/masks
    static constexpr uint8_t RXB0SIDL_IDE   = 0x08; // SIDL bit3
    static constexpr uint8_t RXB0DLC_RTR    = 0x40; // DLC bit6
    static constexpr uint8_t RXB0DLC_LENMSK = 0x0F; // DLC bits3:0
    static constexpr uint8_t CANCTRL_REQOP_MASK = 0xE0;
    static constexpr uint8_t CANCTRL_MODE_CONFIG = 0x80; // 100xxxxx
    static constexpr uint8_t CANCTRL_MODE_NORMAL = 0x00; // 000xxxxx

    static constexpr uint8_t CANINTF_RX0IF = 0x01;

    static constexpr uint8_t TXBCTRL_TXREQ = 0x08; // bit3

    void select(bool en);
    bool reset();
    bool baudRateToCNF(BaudRate baud, bitRateConfig& out);

    bool writeRegister(uint8_t addr, uint8_t val);
    bool readRegister(uint8_t addr, uint8_t& out);
    bool bitModify(uint8_t addr, uint8_t mask, uint8_t data);
    bool writeRegisters(uint8_t startAddr, const uint8_t* data, uint8_t len);
    bool readRegisters(uint8_t startAddr, uint8_t* data, uint8_t len);

    static void packId(uint32_t id, bool extended, uint8_t& sidh, uint8_t& sidl, uint8_t& eid8, uint8_t& eid0);
    static void unpackId(uint8_t sidh, uint8_t sidl, uint8_t eid8, uint8_t eid0, uint32_t& id, bool& extended);
};
