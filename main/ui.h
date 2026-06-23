#ifndef UI_H
#define UI_H


#include "main.h"









extern TaskHandle_t ui_power_state_display_task_handle;
extern TaskHandle_t ui_parameters_reading_display_task_handle;
extern TaskHandle_t ui_sub_resistor_reading_display_task_handle;

extern void ui_power_state_display_task(void *pvParameters);
extern void ui_parameters_reading_display_task(void *pvParameters);
extern void ui_sub_resistor_reading_display_task(void *pvParameters);


#endif