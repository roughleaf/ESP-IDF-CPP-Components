#pragma once

#include "driver/spi_common.h"
#include "driver/spi_master.h"

namespace CPPSPI
{
    class Spi
    {
    private:
        spi_bus_config_t _spi_bus_cfg{};
        spi_device_interface_config_t _spi_interface_cfg{};
        spi_device_handle_t _handle{};
        spi_host_device_t _spi_peripheral{};
        
        spi_transaction_t _spi_transaction{};

        esp_err_t transferByte(const uint8_t reg_addr, const uint8_t data, const uint8_t command = 0);
        esp_err_t transferMultiplesBytes(const uint8_t reg_addr, uint8_t* tx_buf, uint8_t* rx_buf, size_t data_length, const uint8_t command = 0);

    public:
        esp_err_t Init(const spi_host_device_t spi_peripheral, const int pin_miso, const int pin_mosi, const int pin_sclk);
        esp_err_t RegisterDevice(const uint8_t mode, const int ss, const int addr_length, const int command_length, const int bus_speed = 1000);
        uint8_t ReadRegister(const uint8_t reg_addr, const uint8_t command = 0);
        esp_err_t WriteRegister(const uint8_t reg_addr, const uint8_t reg_data, const uint8_t command = 0);
        esp_err_t WriteRegisterMultipleBytes(const uint8_t reg_addr, uint8_t* reg_data_buffer, const uint8_t byte_count, const uint8_t command = 0);
        esp_err_t ReadRegisterMultipleBytes(const uint8_t reg_addr, uint8_t* reg_data_buffer, const uint8_t byte_count, const uint8_t command = 0);
        spi_device_handle_t GetHandle(void);

    }; // class Spi
} // namespace CPPSPI
