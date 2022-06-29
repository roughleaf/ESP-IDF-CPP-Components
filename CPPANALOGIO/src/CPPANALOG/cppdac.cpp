#include "CPPANALOG/analogio.h"

namespace CPPANALOG
{
    esp_err_t CppDac::_init(dac_channel_t channel)
    {
        _channel = channel;

        // Set default values for cosine wave generator
        _cosine_wave_config.en_ch = channel;
        _cosine_wave_config.scale = DAC_CW_SCALE_1;
        _cosine_wave_config.phase = DAC_CW_PHASE_0;
        _cosine_wave_config.freq = 1000;
        _cosine_wave_config.offset = 0;

        return dac_cw_generator_config(&_cosine_wave_config);
    }

    CppDac::CppDac(void)
    {
    }

    CppDac::CppDac(dac_channel_t channel)
    {
        _init(channel);
    }

    esp_err_t CppDac::SetChannel(dac_channel_t channel)
    {
        _init(channel);

        return ESP_OK;
    }

    esp_err_t CppDac::SetVoltageByValue(uint8_t level)
    {
        esp_err_t status = ESP_OK;

        status |= DaCsDisable();
        status |= DacEnable();
        status |= dac_output_voltage(_channel, level);

        return status;
    }

    esp_err_t CppDac::SetVoltage(float voltage)
    {

        esp_err_t status = ESP_OK;
        uint8_t level = 0;

        if (voltage < 0)
        {
            voltage = 0;
        }
        if (voltage > 3.3)
        {
            voltage = 3.3;
        }

        level = voltage * 77.27; // 255/3.3 = 77.27

        status |= DaCsDisable();
        status |= DacEnable();
        status |= dac_output_voltage(_channel, level);

        return status;
    }

    esp_err_t CppDac::DacEnable(void)
    {
        return dac_output_enable(_channel);
    }

    esp_err_t CppDac::DacDisable(void)
    {
        return dac_output_disable(_channel);
    }

    esp_err_t CppDac::DacCsEnable(void)
    {
        esp_err_t status = ESP_OK;
        // status |= DacDisable();
        status |= dac_cw_generator_enable();

        return status;
    }

    esp_err_t CppDac::DaCsDisable(void)
    {
        return dac_cw_generator_disable();
    }

    esp_err_t CppDac::SetCsScale(dac_cw_scale_t scale)
    {
        _cosine_wave_config.scale = scale;

        return dac_cw_generator_config(&_cosine_wave_config);
    }

    esp_err_t CppDac::SetCsPhase(dac_cw_phase_t phase)
    {
        _cosine_wave_config.phase = phase;

        return dac_cw_generator_config(&_cosine_wave_config);
    }

    esp_err_t CppDac::SetCsFrequency(uint32_t freq)
    {
        if(freq < 130)
        {
            freq = 130;
        }
        if (freq > 55000)
        {
            freq = 55000;
        }
        _cosine_wave_config.freq = freq;

        return dac_cw_generator_config(&_cosine_wave_config);
    }

    esp_err_t CppDac::SetCsOffset(int8_t offset)
    {
        _cosine_wave_config.offset = offset;

        return dac_cw_generator_config(&_cosine_wave_config);
    }

    esp_err_t CppDac::SetCsConfig(dac_cw_scale_t scale, dac_cw_phase_t phase, uint32_t freq, int8_t offset)
    {
        if(freq < 130)
        {
            freq = 130;
        }
        if (freq > 55000)
        {
            freq = 55000;
        }

        _cosine_wave_config.scale = scale;
        _cosine_wave_config.phase = phase;
        _cosine_wave_config.freq = freq;
        _cosine_wave_config.offset = offset;

        return dac_cw_generator_config(&_cosine_wave_config);
    }
} // CPPANALOG namespace