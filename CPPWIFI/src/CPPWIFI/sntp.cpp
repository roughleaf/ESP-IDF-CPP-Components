#include "CPPWIFI/sntp.h"
#include <iostream>

namespace SNTP
{
    //Statics
    std::chrono::_V2::system_clock::time_point Sntp::_lastUpdate{};
    bool Sntp::_running{false};

    void Sntp::callbackOnNtpUpdate(timeval *tv)
    {
        std::cout << "NTP updated, current time is: " << timeNowAscii() << '\n';
        _lastUpdate = std::chrono::system_clock::now();
    }

    esp_err_t Sntp::init(void)
    {
        if (!_running)
        {
            if(state_e::CONNECTED != Wifi::GetState())
            {
                std::cout << "Failed to initialise SNTP, Wifi not connected\n";
                return ESP_FAIL;
            }

            setenv("TZ", "GMT-2", 1); // (GMT+2)
            tzset();

            sntp_setoperatingmode(SNTP_OPMODE_POLL);
            sntp_setservername(0, "time.google.com");
            sntp_setservername(1, "pool.ntp.com");

            sntp_set_time_sync_notification_cb(&callbackOnNtpUpdate);
            sntp_set_sync_interval(60 * 1000 * 1000); // Update time every hour

            sntp_init();

            std::cout << "SNTP Initialised\n";

            _running = true;
        }

        if (_running)
        {
            return ESP_OK;
        }
        return ESP_FAIL;
    }

    bool Sntp::setUpdateInterval(uint32_t ms, const bool immediate)
    {
        if (_running)
        {
            sntp_set_sync_interval(ms);
            if (immediate)
            {
                sntp_restart();
            }
            return true;
        }
        return false;
    }

    [[nodiscard]] const auto Sntp::timePointNow(void)
    {
        return std::chrono::system_clock::now();
    }

    [[nodiscard]] const auto Sntp::timeSinceLastUpdate(void)
    {
        return timePointNow() - _lastUpdate;
    }

    [[nodiscard]] std::chrono::seconds Sntp::epochSeconds(void)
    {
        return std::chrono::duration_cast<std::chrono::seconds>(timePointNow().time_since_epoch());
    }

    [[nodiscard]] const char *Sntp::timeNowAscii(void)
    {
        const std::time_t timeNow{std::chrono::system_clock::to_time_t(timePointNow())};

        return std::asctime(std::localtime(&timeNow));
    }
}// namespace SNTP