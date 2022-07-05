#include <memory>
#include "bme280_common/bme280_common.h"

namespace CPPBME280
{
    int BME280::getStatus()
    {
        return readByteData(STATUS);
    }

    int BME280::getCalibrateData()
    {
        //============================== Temperature Calibration Data ===========================
        dig_t1 = static_cast<unsigned short>(readWordData(0x88));
        dig_t2 = static_cast<signed short>(readWordData(0x8A));
        dig_t3 = static_cast<signed short>(readWordData(0x8C));
        //=======================================================================================
        //============================== Pressure Calibration Data ==============================
        dig_p1 = static_cast<unsigned short>(readWordData(0x8E));
        dig_p2 = static_cast<signed short>(readWordData(0x90));
        dig_p3 = static_cast<signed short>(readWordData(0x92));
        dig_p4 = static_cast<signed short>(readWordData(0x94));
        dig_p5 = static_cast<signed short>(readWordData(0x96));
        dig_p6 = static_cast<signed short>(readWordData(0x98));
        dig_p7 = static_cast<signed short>(readWordData(0x9A));
        dig_p8 = static_cast<signed short>(readWordData(0x9C));
        dig_p9 = static_cast<signed short>(readWordData(0x9E));
        //=======================================================================================
        //============================== Humidity Calibration Data ==============================
        dig_h1 = static_cast<unsigned char>(readByteData(0xA1));
        dig_h2 = static_cast<signed short>(readWordData(0xE1));
        dig_h3 = static_cast<unsigned char>(readByteData(0xE3));
        int8_t digH4Msb = static_cast<int8_t>(readByteData(0xE4));
        int8_t digH4H5Shared = static_cast<int8_t>(readByteData(0xE5)); // this register hold parts of the values of dig_H4 and dig_h5
        int8_t digH5Msb = static_cast<int8_t>(readByteData(0xE6));
        dig_h6 = static_cast<int8_t>(readByteData(0xE7));

        dig_h4 = static_cast<signed short>(digH4Msb << 4 | (digH4H5Shared & 0x0F));        // split and shift the bits appropriately.
        dig_h5 = static_cast<signed short>(digH5Msb << 4 | ((digH4H5Shared & 0xF0) >> 4)); // split and shift the bits appropriately.
        //=======================================================================================

        return 0;
    }

    int BME280::getSensorData(SensorRawData *resultRaw)
    {
        esp_err_t status = ESP_OK;
        std::unique_ptr<uint8_t[]> buff = std::make_unique<uint8_t[]>(8);
        uint8_t buf[8]{};

        if (_sensorModeValue == sensorForcedMode)
        {
            SetMode(sensorForcedMode);
            while (StatusMeasuringBusy() || ImUpdateBusy())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
            }
        }

        status = readBlockData(PRESS_MSB, buff.get(), 8);

        uint8_t pressMsb = buff[0];
        uint8_t pressLsb = buff[1];
        uint8_t pressXlsb = buff[2];
        uint8_t tempMsb = buff[3];
        uint8_t tempLsb = buff[4];
        uint8_t tempXlsb = buff[5];
        uint8_t humMsb = buff[6];
        uint8_t humLsb = buff[7];

        resultRaw->temperature = tempMsb << 12 | tempLsb << 4 | tempXlsb >> 4;
        resultRaw->pressure = pressMsb << 12 | pressLsb << 4 | pressXlsb >> 4;
        resultRaw->humididty = humMsb << 8 | humLsb;

        return status;
    }

    float BME280::compensateTemp(const signed long adc_T)
    {
        int32_t var1;
        int32_t var2;
        int32_t temperature;
        int32_t temperature_min = -4000;
        int32_t temperature_max = 8500;

        var1 = (int32_t)((adc_T / 8) - ((int32_t)dig_t1 * 2));
        var1 = (var1 * ((int32_t)dig_t2)) / 2048;
        var2 = (int32_t)((adc_T / 16) - ((int32_t)dig_t1));
        var2 = (((var2 * var2) / 4096) * ((int32_t)dig_t3)) / 16384;
        t_fine = var1 + var2;
        temperature = (t_fine * 5 + 128) / 256;

        if (temperature < temperature_min)
        {
            temperature = temperature_min;
        }
        else if (temperature > temperature_max)
        {
            temperature = temperature_max;
        }

        float returnTemperature = temperature;

        return returnTemperature / 100;
    }

    float BME280::compensatePressure(const unsigned long adc_P)
    {
        int64_t var1;
        int64_t var2;
        int64_t var3;
        int64_t var4;
        uint32_t pressure;
        uint32_t pressure_min = 3000000;
        uint32_t pressure_max = 11000000;

        var1 = ((int64_t)t_fine) - 128000;
        var2 = var1 * var1 * (int64_t)dig_p6;
        var2 = var2 + ((var1 * (int64_t)dig_p5) * 131072);
        var2 = var2 + (((int64_t)dig_p4) * 34359738368);
        var1 = ((var1 * var1 * (int64_t)dig_p3) / 256) + ((var1 * ((int64_t)dig_p2) * 4096));
        var3 = ((int64_t)1) * 140737488355328;
        var1 = (var3 + var1) * ((int64_t)dig_p1) / 8589934592;

        /* To avoid divide by zero exception */
        if (var1 != 0)
        {
            var4 = 1048576 - adc_P;
            var4 = (((var4 * INT64_C(2147483648)) - var2) * 3125) / var1;
            var1 = (((int64_t)dig_p9) * (var4 / 8192) * (var4 / 8192)) / 33554432;
            var2 = (((int64_t)dig_p8) * var4) / 524288;
            var4 = ((var4 + var1 + var2) / 256) + (((int64_t)dig_p7) * 16);
            pressure = (uint32_t)(((var4 / 2) * 100) / 128);

            if (pressure < pressure_min)
            {
                pressure = pressure_min;
            }
            else if (pressure > pressure_max)
            {
                pressure = pressure_max;
            }
        }
        else
        {
            pressure = pressure_min;
        }

        float returnPressure = pressure;

        return returnPressure / 100;
    }

    int BME280::compensateHumidity(const unsigned long adc_H)
    {
        int32_t var1;
        int32_t var2;
        int32_t var3;
        int32_t var4;
        int32_t var5;
        uint32_t humidity;
        uint32_t humidity_max = 102400;

        var1 = t_fine - ((int32_t)76800);
        var2 = (int32_t)(adc_H * 16384);
        var3 = (int32_t)(((int32_t)dig_h4) * 1048576);
        var4 = ((int32_t)dig_h5) * var1;
        var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
        var2 = (var1 * ((int32_t)dig_h6)) / 1024;
        var3 = (var1 * ((int32_t)dig_h3)) / 2048;
        var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
        var2 = ((var4 * ((int32_t)dig_h2)) + 8192) / 16384;
        var3 = var5 * var2;
        var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
        var5 = var3 - ((var4 * ((int32_t)dig_h1)) / 16);
        var5 = (var5 < 0 ? 0 : var5);
        var5 = (var5 > 419430400 ? 419430400 : var5);
        humidity = (uint32_t)(var5 / 4096);

        if (humidity > humidity_max)
        {
            humidity = humidity_max;
        }

        return humidity/1024;
    }

    esp_err_t BME280::Init(const uint8_t humidityOversampling,
                           const uint8_t temperatureOversampling,
                           const uint8_t pressureOversampling,
                           const uint8_t sensorMode)
    {
        _humidityOversamplingValue = humidityOversampling;
        _pressureOversamplingValue = pressureOversampling;
        _temperatureOversamplingValue = temperatureOversampling;
        _sensorModeValue = sensorMode;

        esp_err_t status = ESP_OK;

        status |= writeByteData(CONFIG, 0); // Enable SPI 4-wire
        status |= getCalibrateData();
        status |= writeByteData(CTRL_HUM, _humidityOversamplingValue);
        status |= writeByteData(CTRL_MEAS, _pressureOversamplingValue | _temperatureOversamplingValue | _sensorModeValue);

        return status;
    }

    int BME280::GetDeviceID()
    {
        return readByteData(ID);
    }

    esp_err_t BME280::SetConfig(const uint8_t config)
    {
        return writeByteData(CONFIG, config);
    }

    esp_err_t BME280::SetConfigStandbyT(const uint8_t standby) // config bits 7, 6, 5  page 30
    {
        uint8_t temp = readByteData(CONFIG) & 0b00011111;

        return writeByteData(CONFIG, temp | standby);
    }

    esp_err_t BME280::SetConfigFilter(const uint8_t filter) // config bits 4, 3, 2
    {
        uint8_t temp = readByteData(CONFIG);
        temp = temp & 0b11100011;
        temp = temp | filter << 2;

        return writeByteData(CONFIG, temp);
    }

    esp_err_t BME280::SetCtrlMeas(const uint8_t ctrlMeas)
    {
        _pressureOversamplingValue = 0 | (ctrlMeas & 0b11100011);
        _temperatureOversamplingValue = 0 | (ctrlMeas & 0b00011111);
        _sensorModeValue = 0 | (ctrlMeas & 0b11111100);

        return writeByteData(CTRL_MEAS, ctrlMeas);
    }

    esp_err_t BME280::SetTemperatureOversampling(const uint8_t tempOversampling) // ctrl_meas bits 7, 6, 5   page 29
    {
        uint8_t temp = readByteData(CTRL_MEAS) & 0b00011111;
        _temperatureOversamplingValue = tempOversampling;

        return writeByteData(CTRL_MEAS, temp | tempOversampling);
    }

    esp_err_t BME280::SetPressureOversampling(const uint8_t pressureOversampling) // ctrl_meas bits 4, 3, 2
    {
        uint8_t temp = readByteData(CTRL_MEAS) & 0b11100011;
        _pressureOversamplingValue = pressureOversampling;

        return writeByteData(CTRL_MEAS, temp | pressureOversampling);
    }

    esp_err_t BME280::SetOversampling(const uint8_t tempOversampling, const uint8_t pressureOversampling)
    {
        _pressureOversamplingValue = 0 | pressureOversampling;
        _temperatureOversamplingValue = 0 | tempOversampling;

        return writeByteData(CTRL_MEAS, tempOversampling | pressureOversampling | _sensorModeValue);
    }

    esp_err_t BME280::SetMode(const uint8_t mode) // ctrl_meas bits 1, 0
    {
        uint8_t temp = readByteData(CTRL_MEAS) & 0b11111100;
        _sensorModeValue = mode;

        return writeByteData(CTRL_MEAS, temp | mode);
    }

    esp_err_t BME280::SetCtrlHum(const int humididtyOversampling) // ctrl_hum bits 2, 1, 0    page 28
    {
        _humidityOversamplingValue = humididtyOversampling;
        
        return writeByteData(CTRL_HUM, humididtyOversampling);
    }

    esp_err_t BME280::GetAllResults(BME280ResultData *results)
    {
        esp_err_t status = ESP_OK;
        SensorRawData resultRaw{};

        status = getSensorData(&resultRaw);

        results->temperature = compensateTemp(resultRaw.temperature);
        results->humididty = compensateHumidity(resultRaw.humididty);
        results->pressure = compensatePressure(resultRaw.pressure);

        return status;
    }

    esp_err_t BME280::GetAllResults(float *temperature, int *humidity, float *pressure)
    {
        esp_err_t status = ESP_OK;
        SensorRawData resultRaw{};

        status = getSensorData(&resultRaw);

        *temperature = compensateTemp(resultRaw.temperature);
        *humidity = compensateHumidity(resultRaw.humididty);
        *pressure = compensatePressure(resultRaw.pressure);

        return status;
    }

    float BME280::GetTemperature(void) // Preferable to use GetAllResults()
    {
        BME280ResultData results{};

        GetAllResults(&results);

        return results.temperature; // compensateTemp(resultRaw.temperature);
    }

    float BME280::GetPressure(void)
    {
        BME280ResultData results{};

        GetAllResults(&results);

        return results.pressure;
    }

    int BME280::GetHumidity(void)
    {
        BME280ResultData results{};

        GetAllResults(&results);

        return results.humididty;
    }

    bool BME280::StatusMeasuringBusy(void) // check status (0xF3) bit 3
    {
        return ((readByteData(STATUS) & 8) == 8) ? true : false;
    }

    bool BME280::ImUpdateBusy(void) // check status (0xF3) bit 0
    {
        return ((readByteData(STATUS) & 1) == 1) ? true : false;
    }

    esp_err_t BME280::Reset(void) // write 0xB6 into reset (0xE0)
    {
        return writeByteData(RESET, 0xB6);
    }

} // namespace CPPBME280