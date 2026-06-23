/* VCR function GPIO and resistor sub-divide GPIO pin configuration */
#include "gpio.h"
static const char *GPIO_TAG = "gpio";

void vcr_function_gpio_init(void)
{
	gpio_config_t gpio_out_cfg_1 = {};
		gpio_out_cfg_1.mode = GPIO_MODE_OUTPUT;
		gpio_out_cfg_1.pin_bit_mask = (1ULL<<CURRENT_SELECTION_PIN) | (1ULL<<RES_VOL_CON) | (1ULL<<BLACK_CON);
		gpio_out_cfg_1.pull_down_en = GPIO_PULLDOWN_DISABLE;
		gpio_out_cfg_1.pull_up_en = GPIO_PULLUP_DISABLE;

    ESP_ERROR_CHECK(gpio_config(&gpio_out_cfg_1));
    ESP_LOGI(GPIO_TAG, "voltage-gear | current-gear | resistor-gear gpio_init is ready");
    vTaskDelay(pdMS_TO_TICKS(1));
}

void res_sub_divide_gpio_init(void)
{
    gpio_config_t gpio_out_cfg_2 = {};
		gpio_out_cfg_2.mode = GPIO_MODE_OUTPUT;
		gpio_out_cfg_2.pin_bit_mask = (1ULL<<CON_100) | (1ULL<<CON_1K) | (1ULL<<CON_10K) | (1ULL<<CON_100K);
		gpio_out_cfg_2.pull_down_en = GPIO_PULLDOWN_DISABLE;
		gpio_out_cfg_2.pull_up_en = GPIO_PULLUP_DISABLE;
        
    ESP_ERROR_CHECK(gpio_config(&gpio_out_cfg_2));

    ESP_ERROR_CHECK(gpio_set_level(CON_100, 1));
    ESP_ERROR_CHECK(gpio_set_level(CON_1K, 1));
    ESP_ERROR_CHECK(gpio_set_level(CON_10K, 1));
    ESP_ERROR_CHECK(gpio_set_level(CON_100K, 1));

    ESP_LOGI(GPIO_TAG, "resistor-subdivide_gear gpio_init is ready");
    vTaskDelay(pdMS_TO_TICKS(1));
}

void lcd_brightness_gpio_init(void)
{
    gpio_config_t gpio_out_cfg_3 = {};
		gpio_out_cfg_3.mode = GPIO_MODE_OUTPUT;
		gpio_out_cfg_3.pin_bit_mask = PIN_NUM_BCKL;
		gpio_out_cfg_3.pull_down_en = GPIO_PULLDOWN_DISABLE;
		gpio_out_cfg_3.pull_up_en = GPIO_PULLUP_DISABLE;
        
    ESP_ERROR_CHECK(gpio_config(&gpio_out_cfg_3));

    ESP_ERROR_CHECK(gpio_set_level(PIN_NUM_BCKL, 1));
    vTaskDelay(pdMS_TO_TICKS(1));
}
