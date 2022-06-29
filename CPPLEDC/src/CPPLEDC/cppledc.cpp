#include "CPPLEDC/cppledc.h"

namespace CPPLEDC
{
    void CppLedc::_set_defaults()
    {
        _channel_config.gpio_num = -1;
        _channel_config.speed_mode = LEDC_HIGH_SPEED_MODE;
        _channel_config.channel = LEDC_CHANNEL_0;
        _channel_config.intr_type = LEDC_INTR_DISABLE;
        _channel_config.timer_sel = LEDC_TIMER_0;
        _channel_config.duty = 0;
        _channel_config.hpoint = 0;
    }

    CppLedc::CppLedc(void)
    {
        _set_defaults();
    }

    CppLedc::CppLedc(int gpioNum)
    {
        _set_defaults();
        _channel_config.gpio_num = gpioNum;

        ledc_channel_config(&_channel_config);
    }

    CppLedc::CppLedc(int gpioNum, ledc_timer_t timerNum)
    {
        _set_defaults();
        _channel_config.gpio_num = gpioNum;
        _channel_config.timer_sel = timerNum;

        ledc_channel_config(&_channel_config);
    }

    uint32_t CppLedc::GetChannelDutyCycle(void)
    {
        return _channel_config.duty;
    }

    esp_err_t CppLedc::SetChannelGpio(int gpio_num)
    {
        _channel_config.gpio_num = gpio_num;

        return ledc_channel_config(&_channel_config);
    }

    esp_err_t CppLedc::SetChannelSpeedMode(ledc_mode_t speed_mode)
    {
        _channel_config.speed_mode = speed_mode;

        return ledc_channel_config(&_channel_config);
    }

    esp_err_t CppLedc::SetChannel(ledc_channel_t channel)
    {
        _channel_config.channel = channel;

        return ledc_channel_config(&_channel_config);
    }

    esp_err_t CppLedc::SetChannelInterruptEn(ledc_intr_type_t intr_type)
    {
        _channel_config.intr_type = intr_type;

        return ledc_channel_config(&_channel_config);
    }

    esp_err_t CppLedc::SetChannelTimerSource(ledc_timer_t timer_num)
    {
        _channel_config.timer_sel = timer_num;

        return ledc_channel_config(&_channel_config);
    }

    esp_err_t CppLedc::SetChannelDutyCycle(uint32_t duty)
    {
        esp_err_t status = ESP_OK;

        _channel_config.duty = duty;

        status |= ledc_set_duty(_channel_config.speed_mode,
                                _channel_config.channel,
                                _channel_config.duty);

        status |= ledc_update_duty(_channel_config.speed_mode,
                                   _channel_config.channel);

        return status;
    }

    esp_err_t CppLedc::SetChannelHpoint(int hpoint)
    {
        _channel_config.hpoint = hpoint;

        return ledc_channel_config(&_channel_config);
    }

    esp_err_t CppLedc::SetChannelAll(int gpio_num,
                                     ledc_mode_t speed_mode,
                                     ledc_channel_t channel,
                                     ledc_intr_type_t intr_type,
                                     ledc_timer_t timer_num,
                                     uint32_t duty,
                                     int hpoint)
    {
        _channel_config.gpio_num = gpio_num;
        _channel_config.speed_mode = speed_mode;
        _channel_config.channel = channel;
        _channel_config.intr_type = intr_type;
        _channel_config.timer_sel = timer_num;
        _channel_config.duty = duty;
        _channel_config.hpoint = hpoint;

        return ledc_channel_config(&_channel_config);
    }
} // CPPLEDC