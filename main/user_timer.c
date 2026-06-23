#include "user_timer.h"

static const char *TIMER_200MS_VCR_TAG = "timer_200ms_vcr";
static const char *TIMER_1000MS_VREF_TAG = "timer_1000ms_vref";



/* LVGL tick timer */
esp_timer_handle_t lvgl_tick_timer_handle = NULL;
void lv_tick_task(void *arg)
{
    lv_tick_inc(1);
}
void lvgl_tick_timer_init(void)
{
    const esp_timer_create_args_t timer_args = {
        .callback = &lv_tick_task,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "lv_tick_timer"
    };
    esp_timer_create(&timer_args, &lvgl_tick_timer_handle);
    esp_timer_start_periodic(lvgl_tick_timer_handle, 10000);
}

/* PCNT 1-second timer */
gptimer_handle_t pcnt_timer_1000_handle = NULL;
static bool IRAM_ATTR timer_1000_cb(gptimer_handle_t pcnt_timer_1000_handle, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    bat_pcnt_send_count();
    bat_pcnt_clear();
    return true;
}
void pcnt_timer_1000_init(void)
{
    gptimer_config_t timer_1000_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000,
        .intr_priority = 1,
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_1000_config, &pcnt_timer_1000_handle));
    gptimer_alarm_config_t timer_1000_alarm_config = {
        .alarm_count = 1000000,
        .reload_count = 0,
        .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(pcnt_timer_1000_handle, &timer_1000_alarm_config));
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(pcnt_timer_1000_handle, &(gptimer_event_callbacks_t){.on_alarm = timer_1000_cb}, NULL));
    ESP_ERROR_CHECK(gptimer_enable(pcnt_timer_1000_handle));
    ESP_ERROR_CHECK(gptimer_start(pcnt_timer_1000_handle));
}

/* VCR measurement 200ms timer */
#define TIMER_200MS_PERIOD_MS        200
TimerHandle_t timer_200ms;
uint32_t vcr_measure_channel = 3;
uint32_t ulNotifiedValue;
void vcr_parameter_timer_200ms_callback(TimerHandle_t xTimer)
{
    if (xQueueReceive(button_gear_2_timer200_cb_queue, &ulNotifiedValue, pdMS_TO_TICKS(10)) == pdTRUE)
    {
        switch (ulNotifiedValue)
        {
            case 0:
                vcr_measure_channel = 0;
            break;
            case 1:
                vcr_measure_channel = 2;
            break;
            case 2:
                vcr_measure_channel = 3;
            break;
            default:
            break;
        }
    }
    xQueueSend(timer_200_2_ui_parameters_reading_queue, &vcr_measure_channel, pdMS_TO_TICKS(10));
}
void vcr_timer_200ms_init(void)
{
    timer_200ms = xTimerCreate("Timer_200ms", pdMS_TO_TICKS(TIMER_200MS_PERIOD_MS), pdTRUE, (void*)0, vcr_parameter_timer_200ms_callback);
    if (timer_200ms == NULL) {
        ESP_LOGE(TIMER_200MS_VCR_TAG, "Failed to create Timer_200ms");
        return;
    }
    if(xTimerStart(timer_200ms, 0) != pdPASS)
    {
        ESP_LOGE(TIMER_200MS_VCR_TAG, "Failed to start fast timer");
        return;
    }
}

/* VREF measurement 1000ms timer */
#define TIMER_1000MS_PERIOD_MS       1000
TimerHandle_t timer_1000ms;
void vref_voltage_timer_1000ms_callback(TimerHandle_t xTimer)
{
    uint32_t vref_measure_channel = 1;
    xQueueSend(timer_1000_2_ui_parameters_reading_queue, &vref_measure_channel, pdMS_TO_TICKS(10));
}
void vref_timer_1000ms_init(void)
{
    timer_1000ms = xTimerCreate("Timer_1000ms", pdMS_TO_TICKS(TIMER_1000MS_PERIOD_MS), pdTRUE, (void*)1, vref_voltage_timer_1000ms_callback);
    if (timer_1000ms == NULL) {
        ESP_LOGE(TIMER_1000MS_VREF_TAG, "Failed to create Timer_1000ms");
        return;
    }
    if(xTimerStart(timer_1000ms, 0) != pdPASS) {
        ESP_LOGE(TIMER_1000MS_VREF_TAG, "Failed to start slow timer");
        return;
    }
}

