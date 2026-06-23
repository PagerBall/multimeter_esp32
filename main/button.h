#ifndef BUTTON_H
#define BUTTON_H

#include "main.h"

#include "iot_button.h"
#include "button_gpio.h"

#define BUTTON_ACTIVE_HIGH_LEVEL      1
#define BUTTON_ACTIVE_LOW_LEVEL       0

#define UP_GPIO_PIN_15                GPIO_NUM_15
#define DOWN_GPIO_PIN_14              GPIO_NUM_14

#define TURN_LEFT_GPIO_PIN_34         GPIO_NUM_34
#define TURN_RIGHT_GPIO_PIN_35        GPIO_NUM_35

extern void switch_button_vcr_init(void);

#endif