#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace CPPBME280
{
    class BME280
    {
        // Registers
        constexpr static uint8_t HUM_LSB = 0xFE;
        constexpr static uint8_t HUM_MSB = 0xFD;
        constexpr static uint8_t TEMP_XLSB = 0xFC;
        constexpr static uint8_t TEMP_LSB = 0xFB;
        constexpr static uint8_t TEMP_MSB = 0xFA;
        constexpr static uint8_t PRESS_XLSB = 0xF9;
        constexpr static uint8_t PRESS_LSB = 0xF8;
        constexpr static uint8_t PRESS_MSB = 0xF7;
        constexpr static uint8_t CONFIG = 0xF5;
        constexpr static uint8_t CTRL_MEAS = 0xF4;
        constexpr static uint8_t STATUS = 0xF3;
        constexpr static uint8_t CTRL_HUM = 0xF2;
        constexpr static uint8_t RESET = 0xE0;
        constexpr static uint8_t ID = 0xD0;

        // Settings
        constexpr static uint8_t pressureSensorDisable = 0x00 << 2;
        constexpr static uint8_t pressureOversamplingX1 = 0x01 << 2;
        constexpr static uint8_t pressureOversamplingX2 = 0x02 << 2;
        constexpr static uint8_t pressureOversamplingX4 = 0x03 << 2;
        constexpr static uint8_t pressureOversamplingX8 = 0x04 << 2;
        constexpr static uint8_t pressureOversamplingX16 = 0x05 << 2;
        constexpr static uint8_t temperatureSensorDisable = 0x00 << 5;
        constexpr static uint8_t temperatureOversamplingX1 = 0x01 << 5;
        constexpr static uint8_t temperatureOversamplingX2 = 0x02 << 5;
        constexpr static uint8_t temperatureOversamplingX4 = 0x03 << 5;
        constexpr static uint8_t temperatureOversamplingX8 = 0x04 << 5;
        constexpr static uint8_t temperatureOversamplingX16 = 0x05 << 5;
        constexpr static uint8_t sensorSleepMode = 0x00;
        constexpr static uint8_t sensorForcedMode = 0x01;
        constexpr static uint8_t sensorNormalMode = 0x03;

        constexpr static uint8_t configStandby0_5ms = 0x00 << 5;
        constexpr static uint8_t configStandby62_5ms = 0x01 << 5;
        constexpr static uint8_t configStandby125ms = 0x02 << 5;
        constexpr static uint8_t configStandby250ms = 0x03 << 5;
        constexpr static uint8_t configStandby500ms = 0x04 << 5;
        constexpr static uint8_t configStandby1000ms = 0x05 << 5;
        constexpr static uint8_t configStandby10ms = 0x06 << 5;
        constexpr static uint8_t configStandby20ms = 0x07 << 5;
        constexpr static uint8_t configFilterOff = 0x00 << 2;
        constexpr static uint8_t configFilter2 = 0x01 << 2;
        constexpr static uint8_t configFilter4 = 0x02 << 2;
        constexpr static uint8_t configFilter8 = 0x03 << 2;
        constexpr static uint8_t configFilter16 = 0x04 << 2;

        constexpr static uint8_t humiditySensorDisable = 0x00;
        constexpr static uint8_t humidityOversamplingX1 = 0x01;
        constexpr static uint8_t humidityOversamplingX2 = 0x02;
        constexpr static uint8_t humidityOversamplingX4 = 0x03;
        constexpr static uint8_t humidityOversamplingX8 = 0x04;
        constexpr static uint8_t humidityOversamplingX16 = 0x05;

    private:
        struct SensorRawData
        {
            long temperature = 0;
            unsigned long humididty = 0;
            unsigned long pressure = 0;
        };

        uint8_t _humidityOversamplingValue = humidityOversamplingX1;    // Default to 1X over sampling
        uint8_t _pressureOversamplingValue = pressureOversamplingX1;    // Default to 1X over sampling
        uint8_t _temperatureOversamplingValue = temperatureOversamplingX1; // Default to 1X over sampling
        uint8_t _sensorModeValue = sensorForcedMode;              // Default to forced mode

        // Calibration Data
        unsigned short  dig_t1 = 0;
        signed short    dig_t2 = 0;
        signed short    dig_t3 = 0;
        signed long     t_fine = 0;
        unsigned short  dig_p1 = 0;
        signed short    dig_p2 = 0;
        signed short    dig_p3 = 0;
        signed short    dig_p4 = 0;
        signed short    dig_p5 = 0;
        signed short    dig_p6 = 0;
        signed short    dig_p7 = 0;
        signed short    dig_p8 = 0;
        signed short    dig_p9 = 0;
        uint8_t         dig_h1 = 0;
        signed short    dig_h2 = 0;
        uint8_t         dig_h3 = 0;
        signed short    dig_h4 = 0;
        signed short    dig_h5 = 0;
        signed char     dig_h6 = 0;

        int getStatus();
        int getCalibrateData();
        int getSensorData(SensorRawData *resultRaw);
        float compensateTemp(const signed long adc_T);
        float compensatePressure(const unsigned long adc_P);
        int compensateHumidity(const unsigned long adc_H);

    protected:
        virtual esp_err_t writeByteData(const uint8_t reg, const uint8_t value);
        virtual int readByteData(const uint8_t reg);
        virtual int readWordData(const uint8_t reg);
        virtual esp_err_t readBlockData(const uint8_t reg, uint8_t *buf, const int length);
        
    public:
        struct BME280ResultData
        {
            float temperature = 0.0;
            int humididty = 0;
            float pressure = 0.0;
        } results;

        esp_err_t Init(const uint8_t humidityOversampling = humidityOversamplingX1,
                       const uint8_t temperatureOversampling = temperatureOversamplingX1,
                       const uint8_t pressureOversampling = pressureOversamplingX1,
                       const uint8_t sensorMode = sensorForcedMode);
        int GetDeviceID(void);
        esp_err_t SetConfig(const uint8_t config);
        esp_err_t SetConfigStandbyT(const uint8_t standby);   // config bits 7, 6, 5  page 30
        esp_err_t SetConfigFilter(const uint8_t filter);      // config bits 4, 3, 2
        esp_err_t SetCtrlMeas(const uint8_t ctrlMeas);
        esp_err_t SetTemperatureOversampling(const uint8_t tempOversampling);     // ctrl_meas bits 7, 6, 5   page 29
        esp_err_t SetPressureOversampling(const uint8_t pressureOversampling);    // ctrl_meas bits 4, 3, 2
        esp_err_t SetOversampling(const uint8_t tempOversampling, const uint8_t pressureOversampling);
        esp_err_t SetMode(const uint8_t mode);                                    // ctrl_meas bits 1, 0
        esp_err_t SetCtrlHum(const int humididtyOversampling);                    // ctrl_hum bits 2, 1, 0    page 28
        esp_err_t GetAllResults(BME280ResultData *results);
        esp_err_t GetAllResults(float *temperature, int *humidity, float *pressure);
        float GetTemperature(void);    // Preferable to use GetAllResults()
        float GetPressure(void);       
        int GetHumidity(void);       
        bool StatusMeasuringBusy(void); // check status (0xF3) bit 3
        bool ImUpdateBusy(void);        // check status (0xF3) bit 0
        esp_err_t Reset(void);                // write 0xB6 into reset (0xE0)
    };
} // namespace CPPBME280
