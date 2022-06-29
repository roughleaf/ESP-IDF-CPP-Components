#include "CPPANALOG/analogio.h"

namespace CPPANALOG
{
    bool CppAdc1::_checkCalFuse()
    {
        esp_err_t status {ESP_OK};
        bool cali_enable = false;

        status = esp_adc_cal_check_efuse(ADC_CALI_SCHEME);
        if (ESP_OK == status)
        {
            cali_enable = true;
            esp_adc_cal_characterize(ADC_UNIT_1, _adcAttenuation, _bitWidth, 0, &_adc1_characteristics);
        }
        return cali_enable;
    }

    CppAdc1::CppAdc1(void)
    {
    }

    CppAdc1::CppAdc1(adc1_channel_t channel)
    {
        SetChannel(channel);
    }

    void CppAdc1::SetChannel(adc1_channel_t channel)
    {
        _adc1Channel = channel;
        adc1_config_width(_bitWidth);
        adc1_config_channel_atten(_adc1Channel, _adcAttenuation);

        _calEnabled = _checkCalFuse();
    }

    bool CppAdc1::CheckCalFuse()
    {
        return _checkCalFuse();
    }

    int CppAdc1::GetRaw()
    {
        return adc1_get_raw(_adc1Channel);
    }

    int CppAdc1::GetVoltage()
    {
        if(_calEnabled)
        {
            return esp_adc_cal_raw_to_voltage(adc1_get_raw(_adc1Channel), &_adc1_characteristics);
        }
        else
        {
            return -1;
        }
    }

    esp_err_t CppAdc1::SetBitWidth(adc_bits_width_t bitWidth)
    {
        _bitWidth = bitWidth;
        _calEnabled = _checkCalFuse();
        return adc1_config_width(_bitWidth);
    }

    esp_err_t CppAdc1::SetBitWidth(int bitWidth)
    {
        switch (bitWidth)
        {
        case 9:
            _bitWidth = ADC_WIDTH_BIT_9;
            break;
        case 10:
            _bitWidth = ADC_WIDTH_BIT_10;
            break;
        case 11:
            _bitWidth = ADC_WIDTH_BIT_11;
            break;
        case 12:
            _bitWidth = ADC_WIDTH_BIT_12;
            break;
        default:
            _bitWidth = ADC_WIDTH_BIT_12;
            break;
        }
        _calEnabled = _checkCalFuse();
        return adc1_config_width(_bitWidth);
    }

    esp_err_t CppAdc1::SetAttenuation(adc_atten_t attenuation)
    {
        _adcAttenuation = attenuation;
        _calEnabled = _checkCalFuse();
        return adc1_config_channel_atten(_adc1Channel, _adcAttenuation);
    }

} // CPPANALOG namespace