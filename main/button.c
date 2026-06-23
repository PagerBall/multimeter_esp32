#include "button.h"

static const char *DOWN_BUTTON_TAG = "down_button_single_click";

typedef enum {
    measure_current = 0,
    measure_resistance,
    measure_voltage,
    measure_max
} measure_state_t;
static measure_state_t current_state = measure_voltage;

void down_button_single_click_cb(void *arg, void *data)
{
    measure_state_t next_state = (current_state + 1) % measure_max;

    if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) != pdTRUE)
    {
        ESP_LOGE(DOWN_BUTTON_TAG, "Failed to take LVGL mutex");
        return;
    }

    current_state = next_state;
    ESP_LOGI(DOWN_BUTTON_TAG, "Button clicked, switching to state: %d", current_state);

    switch(current_state)
    {
        case measure_current:
            ESP_LOGI(DOWN_BUTTON_TAG, "State: current measurement");
            gpio_set_level(CURRENT_SELECTION_PIN, 1);
            gpio_set_level(RES_VOL_CON, 0);
            gpio_set_level(BLACK_CON, 0);
            lv_obj_set_style_text_color(guider_ui.screen_1_gear_symbol, lv_color_hex(0xC56E6E), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(guider_ui.screen_1_gear_text, lv_color_hex(0xC56E6E), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(guider_ui.screen_1_measure_label, lv_color_hex(0xC56E6E), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_1_measure_label, lv_color_hex(0xC56E6E), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_label_set_text(guider_ui.screen_1_gear_symbol, "I");
            lv_label_set_text(guider_ui.screen_1_measure_unit, "A");
            lv_label_set_text(guider_ui.screen_1_gear_text, "current");
        break;

        case measure_resistance:
            ESP_LOGI(DOWN_BUTTON_TAG, "State: resistance measurement");
            gpio_set_level(CURRENT_SELECTION_PIN, 0);
            gpio_set_level(RES_VOL_CON, 1);
            gpio_set_level(BLACK_CON, 1);
            lv_obj_set_style_text_color(guider_ui.screen_1_gear_symbol, lv_color_hex(0x6EC56E), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(guider_ui.screen_1_gear_text, lv_color_hex(0x6EC56E), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(guider_ui.screen_1_measure_label, lv_color_hex(0x6EC56E), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_1_measure_label, lv_color_hex(0x6EC56E), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_label_set_text(guider_ui.screen_1_gear_symbol, "R");
            lv_label_set_text(guider_ui.screen_1_measure_unit, "MΩ");
            lv_label_set_text(guider_ui.screen_1_gear_text, "resistor");
        break;

        case measure_voltage:
            ESP_LOGI(DOWN_BUTTON_TAG, "State: voltage measurement");
            gpio_set_level(CURRENT_SELECTION_PIN, 0);
            gpio_set_level(RES_VOL_CON, 0);
            gpio_set_level(BLACK_CON, 1);
            lv_obj_set_style_text_color(guider_ui.screen_1_gear_symbol, lv_color_hex(0x6EC59F), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(guider_ui.screen_1_gear_text, lv_color_hex(0x6EC59F), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(guider_ui.screen_1_measure_label, lv_color_hex(0x6EC59F), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_1_measure_label, lv_color_hex(0x6EC59F), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_label_set_text(guider_ui.screen_1_gear_symbol, "U");
            lv_label_set_text(guider_ui.screen_1_measure_unit, "V");
            lv_label_set_text(guider_ui.screen_1_gear_text, "voltage");
        break;

        default:
            ESP_LOGI(DOWN_BUTTON_TAG, "Unknown state: %d", current_state);
        break;
    }

    xSemaphoreGive(lvgl_mutex);

    xQueueSend(button_gear_2_timer200_cb_queue, &current_state, pdMS_TO_TICKS(10));
}

void switch_button_vcr_init(void)
{
    button_config_t btn_cfg = {0};

    button_gpio_config_t down_gpio_cfg = {
        .gpio_num = DOWN_GPIO_PIN_14,
        .active_level = BUTTON_ACTIVE_LOW_LEVEL,
        .enable_power_save = true,
        .disable_pull = false,
    };
    button_handle_t DOWN_GPIO_PIN_14 = NULL;
    esp_err_t down_ret = iot_button_new_gpio_device(&btn_cfg, &down_gpio_cfg, &DOWN_GPIO_PIN_14);
    assert(down_ret == ESP_OK);
    down_ret = iot_button_register_cb(DOWN_GPIO_PIN_14, BUTTON_SINGLE_CLICK, NULL, down_button_single_click_cb, NULL);

    ESP_ERROR_CHECK(down_ret);

    vTaskDelay(pdMS_TO_TICKS(1));
}

// void left_button_press_up_cb(void *arg, void *data)
// {
//     iot_button_print_event((button_handle_t)arg);
//     ESP_LOGI(LEFT_BUTTON_UP_TAG, "");
// }

// void right_button_press_down_cb(void *arg, void *data)
// {
//     iot_button_print_event((button_handle_t)arg);
//     ESP_LOGI(RIGHT_BUTTON_DOWM_TAG, "");
// }

// void right_button_press_up_cb(void *arg, void *data)
// {
//     iot_button_print_event((button_handle_t)arg);
//     ESP_LOGI(RIGHT_BUTTON_UP_TAG, "");
// }


// 四向开关初始化
// void four_directions_button_init(void)
// {
//     button_config_t btn_cfg = {0};

//     button_gpio_config_t left_gpio_cfg = {
//         .gpio_num = TURN_LEFT_GPIO_PIN_34,
//         .active_level = BUTTON_ACTIVE_HIGH_LEVEL,
//         .enable_power_save = true,
//         .disable_pull = false,
//     };

//     button_gpio_config_t right_gpio_cfg = {
//         .gpio_num = TURN_RIGHT_GPIO_PIN_35,
//         .active_level = BUTTON_ACTIVE_HIGH_LEVEL,
//         .enable_power_save = true,
//         .disable_pull = false,
//     };

//     button_handle_t TURN_LEFT_GPIO_PIN_34;
//     esp_err_t left_press_ret = iot_button_new_gpio_device(&btn_cfg, &left_gpio_cfg, &TURN_LEFT_GPIO_PIN_34);
//     assert(left_press_ret == ESP_OK);

//     left_press_ret = iot_button_register_cb(TURN_LEFT_GPIO_PIN_34, BUTTON_PRESS_DOWN, NULL, left_button_press_down_cb, NULL);
//     left_press_ret = iot_button_register_cb(TURN_LEFT_GPIO_PIN_34, BUTTON_PRESS_UP, NULL, left_button_press_up_cb, NULL);

//     button_handle_t TURN_RIGHT_GPIO_PIN_35;
//     esp_err_t right_press_ret = iot_button_new_gpio_device(&btn_cfg, &right_gpio_cfg, &TURN_RIGHT_GPIO_PIN_35);
//     assert(right_press_ret == ESP_OK);

//     right_press_ret = iot_button_register_cb(TURN_RIGHT_GPIO_PIN_35, BUTTON_PRESS_DOWN, NULL, right_button_press_down_cb, NULL);
//     right_press_ret = iot_button_register_cb(TURN_RIGHT_GPIO_PIN_35, BUTTON_PRESS_UP, NULL, right_button_press_up_cb, NULL);

//     ESP_ERROR_CHECK(left_press_ret);
//     ESP_ERROR_CHECK(right_press_ret);

//     vTaskDelay(pdMS_TO_TICKS(1));
// }

