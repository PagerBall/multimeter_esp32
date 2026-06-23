#ifndef BAT_PCNT_H
#define BAT_PCNT_H

#include "main.h"

#include "driver/pulse_cnt.h"


#define CHAN_GPIO_NUM_17          GPIO_NUM_17
#define CHAN_GPIO_NUM_NC          GPIO_NUM_NC


extern void bat_pcnt_init(void);
extern void bat_pcnt_start(void);
extern void bat_pcnt_stop(void);
extern void bat_pcnt_clear(void);
extern void bat_pcnt_send_count(void);

#endif