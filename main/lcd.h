#ifndef LCD_H
#define LCD_H


#include "main.h"

extern TaskHandle_t lcd_gui_task_handle;
extern SemaphoreHandle_t lvgl_mutex;
extern void lcd_gui_task(void *pvParameters);


#endif