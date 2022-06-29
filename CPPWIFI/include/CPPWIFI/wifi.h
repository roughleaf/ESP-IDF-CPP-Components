#pragma once

#include <cstring>
//#include <algorithm> 
#include <mutex>

#include "esp_wifi.h"
#include "esp_event.h"

//#include "lwip/err.h" // Needed for NTP
//#include "lwip/sys.h" // Needed for NTP

namespace WIFI
{
    class Wifi
    {
    public:
        enum class state_e
        {
            NOT_INITIALIZED,
            INITIALIZED,
            READY_TO_CONNECT,
            CONNECTING,
            WAITING_FOR_IP,
            CONNECTED,
            DISCONNECTED,
            ERROR
        };

    private:
        static esp_err_t _init();
        static wifi_init_config_t _wifi_init_cfg;
        static wifi_config_t _wifi_cfg;

        static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                                       int32_t event_id, void *event_data);
        static void ip_event_handler(void *arg, esp_event_base_t event_base,
                                     int32_t event_id, void *event_data);

        static state_e _state;
        static esp_err_t _get_mac(void);
        static char _mac_addr_cstr[13];
        static std::mutex _mutx;

    public:
        Wifi(void);

        void SetCredentials(const char *ssid, const char *password);
        esp_err_t Init();
        esp_err_t Begin(void);

        constexpr static const state_e &GetState(void) { return _state; }
        constexpr static const char *GetMac(void) { return _mac_addr_cstr; }
    }; // Wifi class

} // namaspace WIFI