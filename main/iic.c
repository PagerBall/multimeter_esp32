#include "iic.h"
static const char *IIC_TAG = "iic";

esp_err_t i2c_master_init(void)
{
    i2c_config_t i2c_bus_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    esp_err_t ret = i2c_param_config(I2C_MASTER_NUM, &i2c_bus_conf);
    if (ret != ESP_OK)
    {
        ESP_LOGE(IIC_TAG, "I2C parameter config failed");
        return ret;
    }
    else
    {
        ESP_LOGI(IIC_TAG, "I2C parameter config successful");
    }
    ret = i2c_driver_install(I2C_MASTER_NUM, i2c_bus_conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    if (ret != ESP_OK)
    {
        ESP_LOGE(IIC_TAG, "I2C driver install failed");
        return ret;
    }
    else
    {
        ESP_LOGI(IIC_TAG, "I2C initialized successfully");
    }
    return ESP_OK;
}