#pragma once

#include <ctime>
#include <chrono>
#include <iomanip>
#include <string>

#include "esp_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "CPPWIFI/wifi.h"

namespace SNTP
{
    class Sntp : private WIFI::Wifi // Inherits from Wifi to access the wifi state
    {       
        static void callbackOnNtpUpdate(timeval *tv);

    public:
        Sntp(void) = default;
        ~Sntp(void) { sntp_stop(); };

        static esp_err_t init(void);

        static bool setUpdateInterval(uint32_t ms, bool immediate = false);

        [[nodiscard]] bool sntpState(void) {return _running;}

        [[nodiscard]] static const auto timePointNow(void);

        [[nodiscard]] static const auto timeSinceLastUpdate(void);

        [[nodiscard]] static const char* timeNowAscii(void);

        [[nodiscard]] static std::chrono::seconds epochSeconds(void);

    private:
        static std::chrono::_V2::system_clock::time_point _lastUpdate;
        static bool _running;

    }; // Class Sntp
} // namespace SNTP