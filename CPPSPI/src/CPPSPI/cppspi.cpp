#include "CPPSPI/cppspi.h"

namespace CPPSPI
{
    esp_err_t Spi::transferByte(const uint8_t reg_addr, const uint8_t data, const uint8_t command)
    {
        _spi_transaction.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA;
        _spi_transaction.cmd = command;
        _spi_transaction.length = 8;
        _spi_transaction.addr = reg_addr;
        _spi_transaction.tx_data[0] = data;

        return spi_device_transmit(_handle, &_spi_transaction);
    }

    esp_err_t Spi::transferMultiplesBytes(const uint8_t reg_addr, uint8_t* tx_buf, uint8_t* rx_buf, size_t data_length, const uint8_t command)
    {
        spi_transaction_t spi_transaction_multibyte;        // spi_transaction_t to use the tx and rx buffers

        if (data_length < 1) { data_length = 1; }

        spi_transaction_multibyte.flags = 0;
        spi_transaction_multibyte.length = (8*data_length);
        spi_transaction_multibyte.rxlength = 0;
        spi_transaction_multibyte.cmd = command;
        spi_transaction_multibyte.addr = reg_addr;
        spi_transaction_multibyte.tx_buffer = tx_buf;
        spi_transaction_multibyte.rx_buffer = rx_buf;        

        return spi_device_transmit(_handle, &spi_transaction_multibyte);
    }

    esp_err_t Spi::Init(const spi_host_device_t spi_peripheral, const int pin_miso, const int pin_mosi, const int pin_sclk)
    {
        esp_err_t status = ESP_OK;

        _spi_peripheral = spi_peripheral;

        _spi_transaction.tx_buffer = nullptr;
        _spi_transaction.rx_buffer = nullptr;

        _spi_bus_cfg.mosi_io_num = pin_mosi;
        _spi_bus_cfg.miso_io_num = pin_miso;
        _spi_bus_cfg.sclk_io_num = pin_sclk;
        _spi_bus_cfg.quadwp_io_num = -1;
        _spi_bus_cfg.quadhd_io_num = -1;

        status |= spi_bus_initialize(spi_peripheral, &_spi_bus_cfg, SPI_DMA_CH_AUTO);

        return status;
    }

    esp_err_t Spi::RegisterDevice(const uint8_t mode, const int ss, const int addr_length, const int command_length, const int bus_speed)
    {
        esp_err_t status = ESP_OK;

        _spi_interface_cfg = {};
        _spi_interface_cfg.command_bits = command_length; 
        _spi_interface_cfg.address_bits = addr_length;
        _spi_interface_cfg.mode = mode;
        _spi_interface_cfg.clock_speed_hz = bus_speed; 
        _spi_interface_cfg.spics_io_num = ss;
        _spi_interface_cfg.queue_size = 5;

        status |= spi_bus_add_device(_spi_peripheral, &_spi_interface_cfg, &_handle);

        return status;
    }

    uint8_t Spi::ReadRegister(const uint8_t reg_addr, const uint8_t command)
    {
        transferByte(reg_addr, 0, command);

        return _spi_transaction.rx_data[0];
    }

    esp_err_t Spi::WriteRegister(const uint8_t reg_addr, const uint8_t reg_data, const uint8_t command)
    {
        esp_err_t status{ESP_OK};

        status |= transferByte(reg_addr, reg_data, command);

        return status;
    }

    esp_err_t Spi::WriteRegisterMultipleBytes(const uint8_t reg_addr, uint8_t* reg_data_buffer, const uint8_t byte_count, const uint8_t command)
    {
        return transferMultiplesBytes(reg_addr, reg_data_buffer, nullptr, byte_count, command);
    }

    esp_err_t Spi::ReadRegisterMultipleBytes(const uint8_t reg_addr, uint8_t* reg_data_buffer, const uint8_t byte_count, const uint8_t command)
    {   
        return transferMultiplesBytes(reg_addr, nullptr, reg_data_buffer, byte_count, command);
    }

    spi_device_handle_t Spi::GetHandle(void)
    {
        return _handle;
    }

} // namespace CPPSPI
