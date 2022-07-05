#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "bme280_common/bme280_common.h"
#include "CPPSPI/cppspi.h"

namespace CPPBME280
{
    class BME280SPI : public BME280
    {
    private:
        CPPSPI::Spi *spi;

    protected:
        esp_err_t writeByteData(const uint8_t reg, const uint8_t value);
        int readByteData(const uint8_t reg);
        int readWordData(const uint8_t reg);
        esp_err_t readBlockData(const uint8_t reg, uint8_t *buf, const int length);

    public:
        esp_err_t InitSpi(CPPSPI::Spi *i_spi, const int ss);
    }; // namespace CPPBME280
} // namespace CPPBME280

//void InitSPI();