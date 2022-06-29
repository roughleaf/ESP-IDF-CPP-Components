#pragma once

#include "driver/adc.h"
#include "driver/dac.h"
#include "esp_adc_cal.h"

// ADC Calibration
#if CONFIG_IDF_TARGET_ESP32
constexpr static const esp_adc_cal_value_t ADC_CALI_SCHEME = ESP_ADC_CAL_VAL_EFUSE_VREF;
#elif CONFIG_IDF_TARGET_ESP32S2
constexpr static const esp_adc_cal_value_t ADC_CALI_SCHEME = ESP_ADC_CAL_VAL_EFUSE_TP;
#elif CONFIG_IDF_TARGET_ESP32C3
constexpr static const esp_adc_cal_value_t ADC_CALI_SCHEME = ESP_ADC_CAL_VAL_EFUSE_TP;
#elif CONFIG_IDF_TARGET_ESP32S3
constexpr static const esp_adc_cal_value_t ADC_CALI_SCHEME = ESP_ADC_CAL_VAL_EFUSE_TP_FIT;
#endif

namespace CPPANALOG
{
    class CppAdc1
    {
        private:
            adc_bits_width_t _bitWidth = ADC_WIDTH_BIT_12;
            adc1_channel_t _adc1Channel;
            adc_atten_t _adcAttenuation = ADC_ATTEN_DB_11;
            bool _calEnabled = false;
            esp_adc_cal_characteristics_t _adc1_characteristics;
            bool _checkCalFuse();
            
        public:
            CppAdc1(void);
            CppAdc1(adc1_channel_t channel);
            void SetChannel(adc1_channel_t channel);
            esp_err_t SetBitWidth(adc_bits_width_t bitWidth);
            esp_err_t SetBitWidth(int bitWidth);
            esp_err_t SetAttenuation(adc_atten_t attenuation);
            bool CheckCalFuse();
            int GetRaw();
            int GetVoltage();
    }; // CppAdc Class

    // DAC channel 1 is GPIO25(ESP32) / GPIO17(ESP32S2)
    // DAC channel 2 is GPIO26(ESP32) / GPIO18(ESP32S2)
    class CppDac
    {
        private:
            dac_channel_t _channel;
            dac_cw_config_t _cosine_wave_config;
            esp_err_t _init(dac_channel_t channel);
        public:
            CppDac(void);
            CppDac(dac_channel_t channel);
            esp_err_t SetChannel(dac_channel_t channel);
            esp_err_t SetVoltageByValue(uint8_t level);
            esp_err_t SetVoltage(float voltage);
            esp_err_t DacEnable(void);
            esp_err_t DacDisable(void);
            esp_err_t DacCsEnable(void);
            esp_err_t DaCsDisable(void);
            esp_err_t SetCsScale(dac_cw_scale_t scale);
            esp_err_t SetCsPhase(dac_cw_phase_t phase);
            esp_err_t SetCsFrequency(uint32_t freq);
            esp_err_t SetCsOffset(int8_t offset);
            esp_err_t SetCsConfig(dac_cw_scale_t scale, dac_cw_phase_t phase, uint32_t freq, int8_t offset);
    }; // CppDac Class
} // CPPANALOG namespace