#include "bme280_spi/bme280_spi.h"

namespace CPPBME280
{
    esp_err_t BME280SPI::writeByteData(const uint8_t reg, const uint8_t value)
    {
        return spi->WriteRegister(reg, value, SPI_WRITE);
    }

    int BME280SPI::readByteData(const uint8_t reg)
    {
        return spi->ReadRegister(reg, SPI_READ);
    }

    int BME280SPI::readWordData(const uint8_t reg)
    {
        uint8_t buff[2];
        spi->ReadRegisterMultipleBytes(reg, buff, 2, SPI_READ);
        return buff[1] << 8 | buff[0];
    }

    esp_err_t BME280SPI::readBlockData(const uint8_t reg, uint8_t *buf, const int length)
    {
        return spi->ReadRegisterMultipleBytes(reg, buf, length, SPI_READ);
    }

    esp_err_t BME280SPI::InitSpi(CPPSPI::Spi *i_spi, const int ss)
    {
        spi = i_spi;

        return spi->RegisterDevice(MODE, ss, ADDR_BITS, CMD_BITS);
    }
} // namespace CPPBME280