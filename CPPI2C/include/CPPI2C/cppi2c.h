#pragma once

#include "driver/i2c.h"
#include "esp_intr_alloc.h"

namespace CPPI2C
{
    class I2c
    {
    private:
        uint16_t _slaveAddr{};
        i2c_port_t _port{};
        i2c_mode_t _mode{};
        size_t _slv_rx_buf_len{};
        size_t _slv_tx_buf_len{};
        int _intr_alloc_flags{};

    public:
        I2c(i2c_port_t port, size_t slv_rx_buf_len = 0, size_t slv_tx_buf_len = 0, int intr_alloc_flags = 0);
        ~I2c();

        esp_err_t InitMaster(int sda_io_num,
                             int scl_io_num,
                             uint32_t clk_speed,
                             bool sda_pullup_en = false,
                             bool scl_pullup_en = false,
                             uint32_t clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL);

        uint8_t ReadRegister(uint8_t dev_addr, uint8_t reg_addr);        
        esp_err_t WriteRegister(uint8_t dev_addr, uint8_t reg_addr, uint8_t txData);
        esp_err_t ReadRegisterMultipleBytes(uint8_t dev_addr, uint8_t reg_addr, uint8_t *rx_data, int length);
        esp_err_t WriteRegisterMultipleBytes(uint8_t dev_addr, uint8_t reg_addr, uint8_t *tx_data, int length);
    }; // class I2c
} // namespace CPPI2C

        // Slave mode not implimented
        /*esp_err_t InitSlave(int sda_io_num,
                            int scl_io_num,
                            uint8_t addr_10bit_en,
                            uint16_t slave_addr,
                            uint32_t maximum_speed = 400000,
                            bool sda_pullup_en = false,
                            bool scl_pullup_en = false);

        esp_err_t FlushTxFifo(void);
        esp_err_t FlushRxFifo(void);*/