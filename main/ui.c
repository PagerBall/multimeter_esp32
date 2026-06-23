#include "ui.h"


TaskHandle_t ui_power_state_display_task_handle = NULL;
void ui_power_state_display_task(void *pvParameters)
{
    int battery_count;
    char battery_state[12];
    while (1)
    {
        if(xQueueReceive(bat_pcnt_2_ui_queue, &battery_count, portMAX_DELAY) == pdPASS)
        {
            snprintf(battery_state, sizeof(battery_state), "%d", battery_count);
            if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
            {
                lv_label_set_text(guider_ui.screen_1_battery_pcnt, battery_state);
                xSemaphoreGive(lvgl_mutex);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

#define VCR_VOLTAGE_DIVIDER (492.0f / 22.0f)
#define VCR_DIODE_DROP 0.016125f

TaskHandle_t ui_parameters_reading_display_task_handle = NULL;
void ui_parameters_reading_display_task(void *pvParameters)
{
    uint32_t measure_channel;
    float raw_2_voltage = 0.0f;
    float vcr_measure_voltage = 0.0f;
    char vcr_measure_str[16];
    char vref_measure_str[16];
    float vref_voltage = ads1115_read_voltage(&adc_config, ADS1115_CHANNEL_1, ADS1115_PGA_2_048V);
    while(1)
    {
        if(xQueueReceive(timer_200_2_ui_parameters_reading_queue, &measure_channel, 0) == pdTRUE || xQueueReceive(timer_1000_2_ui_parameters_reading_queue, &measure_channel, 0) == pdTRUE)
        {
            ads1115_channel_t ch = (ads1115_channel_t)measure_channel;
            switch (ch)
            {
                case ADS1115_CHANNEL_0:
                    raw_2_voltage = ads1115_read_voltage(&adc_config, ch, 0);
                    snprintf(vcr_measure_str, sizeof(vcr_measure_str), "%.4f", raw_2_voltage);
                    if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
                    {
                        lv_label_set_text(guider_ui.screen_1_measure_value, vcr_measure_str);
                        xSemaphoreGive(lvgl_mutex);
                    }
                break;
                case ADS1115_CHANNEL_1:
                    raw_2_voltage = ads1115_read_voltage(&adc_config, ch, ADS1115_PGA_2_048V);
                    vref_voltage = raw_2_voltage;
                    snprintf(vref_measure_str, sizeof(vref_measure_str), "%.4f", raw_2_voltage);
                    if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
                    {
                        lv_label_set_text(guider_ui.screen_1_vref_value, vref_measure_str);
                        xSemaphoreGive(lvgl_mutex);
                    }
                break;
                case ADS1115_CHANNEL_2:
                    xTaskNotify(ui_sub_resistor_reading_display_task_handle, 1, eSetValueWithOverwrite);
                break;
                case ADS1115_CHANNEL_3:
                    raw_2_voltage = ads1115_read_voltage(&adc_config, ch, 0);
                    vcr_measure_voltage = VCR_VOLTAGE_DIVIDER * (raw_2_voltage - vref_voltage - VCR_DIODE_DROP);
                    snprintf(vcr_measure_str, sizeof(vcr_measure_str), "%.4f", vcr_measure_voltage);
                    if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
                    {
                        lv_label_set_text(guider_ui.screen_1_measure_value, vcr_measure_str);
                        xSemaphoreGive(lvgl_mutex);
                    }
                break;
                default:
                break;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

TaskHandle_t ui_sub_resistor_reading_display_task_handle = NULL;
void ui_sub_resistor_reading_display_task(void *pvParameters)
{
    uint32_t measure_channel = 2;
    float vout = 0;
    char res_value_str[16];
    const float VCC = 3.30f;

    const float ref_ohms[] = {100000.0f, 10000.0f, 1000.0f, 100.0f};
    const int ref_gpios[] = {CON_100K, CON_10K, CON_1K, CON_100};
    const int num_ranges = 4;

    while(1)
    {
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) == pdTRUE)
        {
            float best_vout = 0;
            int best_range = -1;

            for (int i = 0; i < num_ranges; i++)
            {
                for (int j = 0; j < num_ranges; j++)
                    gpio_set_level(ref_gpios[j], (j == i) ? 0 : 1);

                vTaskDelay(pdMS_TO_TICKS(1));
                vout = ads1115_read_voltage(&adc_config, (ads1115_channel_t)measure_channel, 0);
                if (vout < 0.0f) vout = 0.0f;
                best_vout = vout;

                if (vout < 0.05f)
                    continue;

                float denom = VCC - vout;
                if (denom < 0.001f)
                    continue;

                float r_ohms = (vout * ref_ohms[i]) / denom;
                if (r_ohms > 100000000.0f)
                    continue;

                best_range = i;
                break;
            }

            for (int j = 0; j < num_ranges; j++)
                gpio_set_level(ref_gpios[j], 1);

            const char *unit = "MΩ";
            if (best_range >= 0)
            {
                float vout_used = best_vout;
                float denom = VCC - vout_used;
                float r_ohms = (vout_used * ref_ohms[best_range]) / denom;

                if (r_ohms >= 1000000.0f)
                {
                    snprintf(res_value_str, sizeof(res_value_str), "%.4f", r_ohms / 1000000.0f);
                }
                else if (r_ohms >= 1000.0f)
                {
                    snprintf(res_value_str, sizeof(res_value_str), "%.4f", r_ohms / 1000.0f);
                    unit = "KΩ";
                }
                else
                {
                    snprintf(res_value_str, sizeof(res_value_str), "%.4f", r_ohms);
                    unit = "Ω";
                }
            }
            else if (best_vout > 3.28f)
            {
                snprintf(res_value_str, sizeof(res_value_str), "O.L");
            }
            else
            {
                snprintf(res_value_str, sizeof(res_value_str), "0.0000");
                unit = "Ω";
            }

            if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
            {
                lv_label_set_text(guider_ui.screen_1_measure_value, res_value_str);
                lv_label_set_text(guider_ui.screen_1_measure_unit, unit);
                xSemaphoreGive(lvgl_mutex);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}