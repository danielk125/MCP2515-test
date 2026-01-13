#ifndef ISPI_HPP
#define ISPI_HPP

#include "../../ISpi.hpp"
#include <SPI.h>

class ESPSpi final : public ISpi {
    SPISettings _settings;

public:
    ESPSpi(uint32_t clock_hz, uint8_t bitOrder = MSBFIRST, uint8_t dataMode = SPI_MODE0)
        : _settings(clock_hz, bitOrder, dataMode) {}

    // ESP Arduino platform ONLY
    void ESPSpi_init(uint32_t sck = SCK, uint32_t miso = MISO, uint32_t mosi = MOSI) { 
        SPI.begin(sck, miso, mosi); 
    }

    bool ISpi_transfer(const uint8_t* tx, uint8_t* rx, size_t len) override {
        if (!tx || !rx || len == 0) return false;

        SPI.beginTransaction(settings_);
        for (size_t i = 0; i < len; i++) {
            rx[i] = SPI.transfer(tx[i]);
        }
        SPI.endTransaction();
        return true;
    }

    bool ISpi_write(const uint8_t* tx, size_t len) override {
        if (!tx || len == 0) return false;

        SPI.beginTransaction(settings_);
        for (size_t i = 0; i < len; i++) SPI.transfer(tx[i]);

        SPI.endTransaction();
        return true;
    }
};
