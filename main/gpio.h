#ifndef GPIO_H
#define GPIO_H

#include "main.h"

#include "driver/gpio.h"

#define CURRENT_SELECTION_PIN        GPIO_NUM_25
#define RES_VOL_CON                  GPIO_NUM_33
#define BLACK_CON                    GPIO_NUM_32

#define CON_100K                     GPIO_NUM_26
#define CON_10K                      GPIO_NUM_27
#define CON_1K                       GPIO_NUM_2
#define CON_100                      GPIO_NUM_13

#define PIN_NUM_BCKL                 GPIO_NUM_16

extern void vcr_function_gpio_init(void);
extern void res_sub_divide_gpio_init(void);
extern void lcd_brightness_gpio_init(void);

#endif