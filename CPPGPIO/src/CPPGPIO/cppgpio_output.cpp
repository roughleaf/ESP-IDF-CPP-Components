#include "CPPGPIO/cppgpio.h"

namespace CPPGPIO
{
    esp_err_t GpioOutput::_init(const gpio_num_t pin, const bool activeLow)
    {
        esp_err_t status{ESP_OK};

        _active_low = activeLow;
        _pin = pin;

        gpio_config_t cfg;
        cfg.pin_bit_mask = 1ULL << pin;
        cfg.mode = GPIO_MODE_OUTPUT;
        cfg.pull_up_en = GPIO_PULLUP_DISABLE;
        cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
        cfg.intr_type = GPIO_INTR_DISABLE;

        status |= gpio_config(&cfg);

        return status;
    }

    GpioOutput::GpioOutput(const gpio_num_t pin, const bool activeLow)
    {
        _init(pin, activeLow);
    }

    GpioOutput::GpioOutput(const gpio_num_t pin)
    {
        _init(pin, false);
    }

    GpioOutput::GpioOutput(void)
    {
    }

    esp_err_t GpioOutput::init(const gpio_num_t pin, const bool activeLow)
    {
        return _init(pin, activeLow);
    }

    esp_err_t GpioOutput::init(const gpio_num_t pin)
    {
        return _init(pin, false);
    }

    esp_err_t GpioOutput::on(void)
    {
        _level = true;
        return gpio_set_level(_pin, _active_low ? 0 : 1);
    }

    esp_err_t GpioOutput::off(void)
    {
        _level = false;
        return gpio_set_level(_pin, _active_low ? 1 : 0);
    }

    esp_err_t GpioOutput::toggle(void)
    {
        _level = _level ? 0 : 1;
        return gpio_set_level(_pin, _level ? 1 : 0);
    }

    esp_err_t GpioOutput::setLevel(int level)
    {
        _level = _active_low ? !level : level;
        return gpio_set_level(_pin, _level);
    }
} // Namespace CPPGPIO