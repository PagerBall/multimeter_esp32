# ESP32 Multimeter

An ESP32-based multimeter with LVGL touchscreen UI, supporting voltage, current, and resistance measurements.

## Hardware

- ESP32 microcontroller
- ADS1115 16-bit ADC (I2C)
- ST7735S TFT LCD display
- Rotary switch for mode selection (voltage/current/resistance)
- Battery monitoring via pulse counter

## Dependencies

- ESP-IDF v5.4.1
- LVGL 8.3.10
- LVGL ESP32 Drivers 1.0.5
- espressif/button 4.1.3
- esp-idf-lib/ads111x 1.1.14

## Build

```bash
idf.py set-target esp32
idf.py build
idf.py flash monitor
```
