#include "main.h"

static const char *MAIN_TAG = "MAIN";

lv_ui guider_ui;

QueueHandle_t timer_200_2_ui_parameters_reading_queue;
QueueHandle_t timer_1000_2_ui_parameters_reading_queue;
QueueHandle_t button_gear_2_timer200_cb_queue;
QueueHandle_t bat_pcnt_2_ui_queue;

void app_main(void)
{
    timer_200_2_ui_parameters_reading_queue = xQueueCreate(1, sizeof(int));
    timer_1000_2_ui_parameters_reading_queue = xQueueCreate(1, sizeof(int));
    button_gear_2_timer200_cb_queue = xQueueCreate( 1, sizeof(float));
    bat_pcnt_2_ui_queue = xQueueCreate(1, sizeof(int));

    lv_init();
    lvgl_driver_init();

    lvgl_tick_timer_init();
    lcd_brightness_gpio_init();

    pcnt_timer_1000_init();
    vcr_timer_200ms_init();
    vref_timer_1000ms_init();

    switch_button_vcr_init();

    vcr_function_gpio_init();
    res_sub_divide_gpio_init();

    bat_pcnt_init();

    esp_err_t i2c_ret = i2c_master_init();
    if (i2c_ret != ESP_OK) {
        ESP_LOGE(MAIN_TAG, "I2C init failed, aborting");
        return;
    }

    ads1115_config_setup();

    lvgl_mutex = xSemaphoreCreateMutex();

    if (xTaskCreate(lcd_gui_task, "lcd_gui_task", 2048*2, NULL, 5, &lcd_gui_task_handle) != pdPASS ||
        xTaskCreate(ui_power_state_display_task, "ui_power_state_display_task", 2048*2, NULL, 5, &ui_power_state_display_task_handle) != pdPASS ||
        xTaskCreate(ui_parameters_reading_display_task, "ui_parameters_reading_display_task", 4096, NULL, 5, &ui_parameters_reading_display_task_handle) != pdPASS ||
        xTaskCreate(ui_sub_resistor_reading_display_task, "ui_sub_resistor_reading_display_task", 4096, NULL, 5, &ui_sub_resistor_reading_display_task_handle) != pdPASS) {
        ESP_LOGE(MAIN_TAG, "Failed to create tasks");
    }

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
