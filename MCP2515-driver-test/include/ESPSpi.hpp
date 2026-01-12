#ifndef ISPI_HPP
#define ISPI_HPP

#include "ISpi.hpp"
#include <SPI.h>

class ESPSpi final : public ISpi {
    SPISettings settings_;
public:
    ESPSpi(uint32_t clock_hz, uint8_t bitOrder = MSBFIRST, uint8_t dataMode = SPI_MODE0)
        : settings_(clock_hz, bitOrder, dataMode) {}

    
};