#include "ads1115.h"

static const char *ADS1115_TAG = "ADS1115";

esp_err_t ads1115_init(ads1115_config_t *config)
{
    if (!config) {
        return ESP_ERR_INVALID_ARG;
    }
    // Configure default values if not set
    if (config->gain == 0) {
        config->gain = ADS1115_PGA_6_144V;
    }
    if (config->data_rate == 0) {
        config->data_rate = ADS1115_DR_128SPS;
    }
    ESP_LOGI(ADS1115_TAG, "Initializing ADS1115 at address 0x%02X", config->device_address);
    // Check if device responds
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (config->device_address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(config->i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        ESP_LOGE(ADS1115_TAG, "Device not found at address 0x%02X", config->device_address);
        return ret;
    }
    ESP_LOGI(ADS1115_TAG, "ADS1115 initialized successfully");
    return ESP_OK;
}

ads1115_config_t adc_config = {0};
void ads1115_config_setup(void)
{
    // Configure ADC
    adc_config.i2c_port = I2C_MASTER_NUM;
    adc_config.device_address = ADS1115_ADDRESS;
    adc_config.gain = ADS1115_PGA_4_096V;
    adc_config.data_rate = ADS1115_DR_250SPS;
    ads1115_init(&adc_config);
}

int16_t ads1115_read_raw(ads1115_config_t *config, ads1115_channel_t channel, uint16_t gain)
{
    if (!config) {
        return 0;
    }
    uint16_t config_reg = ADS1115_OS_SINGLE;
    switch (channel) {
        case ADS1115_CHANNEL_0:
            config_reg |= ADS1115_MUX_CH0;
            break;
        case ADS1115_CHANNEL_1:
            config_reg |= ADS1115_MUX_CH1;
            break;
        case ADS1115_CHANNEL_2:
            config_reg |= ADS1115_MUX_CH2;
            break;
        case ADS1115_CHANNEL_3:
            config_reg |= ADS1115_MUX_CH3;
            break;
        default:
            config_reg |= ADS1115_MUX_CH0;
            break;
    }
    config_reg |= gain ? gain : config->gain;
    config_reg |= config->data_rate;
    config_reg |= ADS1115_MODE_SINGLE;
    config_reg |= ADS1115_CQUE_NONE;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (config->device_address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, ADS1115_CONFIG_REG, true);
    i2c_master_write_byte(cmd, config_reg >> 8, true);
    i2c_master_write_byte(cmd, config_reg & 0xFF, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(config->i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        ESP_LOGE(ADS1115_TAG, "Failed to write config register for channel %d", channel);
        return 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));
    int16_t raw_value = 0;
    uint8_t raw_bytes[2] = {0};
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (config->device_address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, ADS1115_CONVERSION_REG, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (config->device_address << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, &raw_bytes[0], I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &raw_bytes[1], I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(config->i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        ESP_LOGE(ADS1115_TAG, "Failed to read conversion register for channel %d", channel);
        return 0;
    } else {
        raw_value = ((int16_t)raw_bytes[0] << 8) | raw_bytes[1];
    }
    return raw_value;
}

float ads1115_read_voltage(ads1115_config_t *config, ads1115_channel_t channel, uint16_t gain)
{
    uint16_t active_gain = gain ? gain : config->gain;
    int16_t raw_value = ads1115_read_raw(config, channel, active_gain);
    float voltage = 0.0;

    switch (active_gain) {
        case ADS1115_PGA_6_144V:
            voltage = (raw_value * 6.144f) / 32767.0f;
            break;
        case ADS1115_PGA_4_096V:
            voltage = (raw_value * 4.096f) / 32767.0f;
            break;
        case ADS1115_PGA_2_048V:
            voltage = (raw_value * 2.048f) / 32767.0f;
            break;
        case ADS1115_PGA_1_024V:
            voltage = (raw_value * 1.024f) / 32767.0f;
            break;
        case ADS1115_PGA_0_512V:
            voltage = (raw_value * 0.512f) / 32767.0f;
            break;
        case ADS1115_PGA_0_256V:
            voltage = (raw_value * 0.256f) / 32767.0f;
            break;
        default:
            voltage = (raw_value * 6.144f) / 32767.0f;
            break;
    }
    return voltage;
}
