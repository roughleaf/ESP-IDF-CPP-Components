#pragma once

#include "driver/ledc.h"

namespace CPPLEDC
{
    class CppLedcTimer
    {
    private:
        ledc_timer_config_t _timer_config;
        void _set_defaults();

    public:
        CppLedcTimer(void);

        esp_err_t InitializeDefaults(void);
        esp_err_t SetTimerSpeedMode(ledc_mode_t speed_mode);
        esp_err_t SetTimerDutyResolution(ledc_timer_bit_t duty_resolution);
        esp_err_t SetTimerSource(ledc_timer_t timer_num);
        esp_err_t SetTimerFrequency(uint32_t freq_hz);
        esp_err_t SetTimerClockSource(ledc_clk_cfg_t clk_cfg);
        esp_err_t SetTimerAll(ledc_mode_t speed_mode,
                              ledc_timer_bit_t duty_resolution,
                              ledc_timer_t timer_num,
                              uint32_t freq_hz,
                              ledc_clk_cfg_t clk_cfg);

    }; // Class CppLedTimer

    class CppLedc
    {
    private:
        ledc_channel_config_t _channel_config;
        void _set_defaults();

    public:
        CppLedc(void);
        CppLedc(int gpioNum);
        CppLedc(int gpioNum, ledc_timer_t timerNum);

        uint32_t GetChannelDutyCycle(void);
        esp_err_t SetChannelGpio(int gpio_num);
        esp_err_t SetChannelSpeedMode(ledc_mode_t speed_mode);
        esp_err_t SetChannel(ledc_channel_t channel);
        esp_err_t SetChannelInterruptEn(ledc_intr_type_t intr_type);
        esp_err_t SetChannelTimerSource(ledc_timer_t timer_num);
        esp_err_t SetChannelDutyCycle(uint32_t duty);
        esp_err_t SetChannelHpoint(int hpoint);
        esp_err_t SetChannelAll(int gpio_num,
                                ledc_mode_t speed_mode,
                                ledc_channel_t channel,
                                ledc_intr_type_t intr_type,
                                ledc_timer_t timer_num,
                                uint32_t duty,
                                int hpoint);

    }; // CppLedc class
} // CPPLEDC Class