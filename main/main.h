#ifndef MAIN_H
#define MAIN_H

#define MAX_STRING_LENGTH 8

#include <stdio.h>
#include <malloc.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "esp_system.h"
#include "esp_idf_version.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "esp_heap_caps.h"
#include "lvgl.h"
#include "lvgl_helpers.h"

#include "gui_guider.h"

#include "lcd.h"
#include "ui.h"
#include "button.h"
#include "gpio.h"
#include "iic.h"
#include "ads1115.h"
#include "bat_pcnt.h"
#include "user_timer.h"

extern QueueHandle_t timer_200_2_ui_parameters_reading_queue;
extern QueueHandle_t timer_1000_2_ui_parameters_reading_queue;
extern QueueHandle_t button_gear_2_timer200_cb_queue;
extern QueueHandle_t bat_pcnt_2_ui_queue;

#endif