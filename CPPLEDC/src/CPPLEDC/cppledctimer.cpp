#include "CPPLEDC/cppledc.h"

namespace CPPLEDC
{
    void CppLedcTimer::_set_defaults()
    {
        _timer_config.speed_mode = LEDC_HIGH_SPEED_MODE;
        _timer_config.duty_resolution = LEDC_TIMER_12_BIT;
        _timer_config.timer_num = LEDC_TIMER_0;
        _timer_config.freq_hz = 5000;
        _timer_config.clk_cfg = LEDC_USE_APB_CLK;
    }

    CppLedcTimer::CppLedcTimer(void)
    {
        _set_defaults();
    }

    esp_err_t CppLedcTimer::InitializeDefaults(void)
    {
        _set_defaults();
        return ledc_timer_config(&_timer_config);
    }

    esp_err_t CppLedcTimer::SetTimerSpeedMode(ledc_mode_t speed_mode)
    {
        _timer_config.speed_mode = speed_mode;

        return ledc_timer_config(&_timer_config);
    }

    esp_err_t CppLedcTimer::SetTimerDutyResolution(ledc_timer_bit_t duty_resolution)
    {
        _timer_config.duty_resolution = duty_resolution;

        return ledc_timer_config(&_timer_config);
    }

    esp_err_t CppLedcTimer::SetTimerSource(ledc_timer_t timer_num)
    {
        _timer_config.timer_num = timer_num;

        return ledc_timer_config(&_timer_config);
    }

    esp_err_t CppLedcTimer::SetTimerFrequency(uint32_t freq_hz)
    {
        _timer_config.freq_hz = freq_hz;

        return ledc_timer_config(&_timer_config);
    }

    esp_err_t CppLedcTimer::SetTimerClockSource(ledc_clk_cfg_t clk_cfg)
    {
        _timer_config.clk_cfg = clk_cfg;

        return ledc_timer_config(&_timer_config);
    }

    esp_err_t CppLedcTimer::SetTimerAll(ledc_mode_t speed_mode,
                                        ledc_timer_bit_t duty_resolution,
                                        ledc_timer_t timer_num,
                                        uint32_t freq_hz,
                                        ledc_clk_cfg_t clk_cfg)
    {
        _timer_config.speed_mode = speed_mode;
        _timer_config.duty_resolution = duty_resolution;
        _timer_config.timer_num = timer_num;
        _timer_config.freq_hz = freq_hz;
        _timer_config.clk_cfg = clk_cfg;

        return ledc_timer_config(&_timer_config);
    }
} // CPPLEDC