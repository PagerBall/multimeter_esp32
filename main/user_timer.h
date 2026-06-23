#ifndef USER_TIMER_H
#define USER_TIMER_H

#include "main.h"
#include "driver/gptimer.h"


extern void lvgl_tick_timer_init(void);
extern void pcnt_timer_1000_init(void);
extern void vcr_timer_200ms_init(void);
extern void vref_timer_1000ms_init(void);

extern void lv_tick_task(void *arg);

#endif