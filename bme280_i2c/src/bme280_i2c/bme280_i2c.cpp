#include "bme280_i2c/bme280_i2c.h"

namespace CPPBME280
{
    esp_err_t BME280I2C::writeByteData(const uint8_t reg, const uint8_t value)
    {
        return i2c->WriteRegister(_devAddress, reg, value);
    }

    int BME280I2C::readByteData(const uint8_t reg)
    {
        return i2c->ReadRegister(_devAddress, reg);
    }

    int BME280I2C::readWordData(const uint8_t reg)
    {
        uint8_t buff[2];
        i2c->ReadRegisterMultipleBytes(_devAddress, reg, buff, 2);
        return buff[1] << 8 | buff[0];
    }

    esp_err_t BME280I2C::readBlockData(const uint8_t reg, uint8_t *buf, const int length)
    {
        return i2c->ReadRegisterMultipleBytes(_devAddress, reg, buf, length);
    }

    void BME280I2C::InitI2c(CPPI2C::I2c *i_i2c, const uint8_t dev_addr)
    {
        i2c = i_i2c;
        _devAddress = dev_addr;
    }
} // namespace CPPBME280