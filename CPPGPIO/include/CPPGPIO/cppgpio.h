#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_event.h"
#include "driver/gpio.h"

namespace CPPGPIO
{
    ESP_EVENT_DECLARE_BASE(INPUT_EVENTS);

    class GpioBase
    {
    protected:
        bool _active_low = false;
    }; // GpioBase Class

    class GpioInput : public GpioBase
    {
    private:
        esp_err_t _init(const gpio_num_t pin, const bool activeLow);
        static bool _interrupt_service_installed;

        esp_event_handler_t _event_handle = nullptr;
        static portMUX_TYPE _eventChangeMutex;

        esp_err_t _clearEventHandlers();

        struct interrupt_args
        {
            bool _event_handler_set = false;
            bool _custom_event_handler_set = false;
            bool _queue_enabled = false;
            gpio_num_t _pin;
            esp_event_loop_handle_t _custom_event_loop_handle {nullptr};
            xQueueHandle _queue_handle {nullptr};
        } _interrupt_args;

    public:
        GpioInput(const gpio_num_t pin, const bool activeLow);
        GpioInput(const gpio_num_t pin);
        GpioInput(void);
        esp_err_t init(const gpio_num_t pin, const bool activeLow);
        esp_err_t init(const gpio_num_t pin);
        int read(void);

        esp_err_t enablePullup(void);
        esp_err_t disablePullup(void);
        esp_err_t enablePulldown(void);
        esp_err_t disablePulldown(void);
        esp_err_t enablePullupPulldown(void);
        esp_err_t disablePullupPulldown(void);
        
        esp_err_t enableInterrupt(gpio_int_type_t int_type);
        esp_err_t setEventHandler(esp_event_handler_t Gpio_e_h);
        esp_err_t setEventHandler(esp_event_loop_handle_t Gpio_e_l, esp_event_handler_t Gpio_e_h);
        void setQueueHandle(xQueueHandle Gpio_e_q);
        static void IRAM_ATTR gpio_isr_callback(void* arg);
    }; // GpioInput Class

    class GpioOutput : public GpioBase
    {
    private:
        gpio_num_t _pin;
        int _level = 0;
        esp_err_t _init(const gpio_num_t pin, const bool activeLow);

    public:
        GpioOutput(const gpio_num_t pin, const bool activeLow);
        GpioOutput(const gpio_num_t pin);
        GpioOutput(void);
        esp_err_t init(const gpio_num_t pin, const bool activeLow);
        esp_err_t init(const gpio_num_t pin);
        esp_err_t on(void);
        esp_err_t off(void);
        esp_err_t toggle(void);
        esp_err_t setLevel(int level);
    }; // GpioOutput Class
} // Gpio Namespace