set(SOURCES src/CPPWIFI/wifi.cpp
            src/CPPWIFI/sntp.cpp)
            
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS include
                    REQUIRES esp_wifi nvs_flash esp_event esp_netif lwip)