#ifndef ADS1115_H
#define ADS1115_H


#include "main.h"
#include "driver/i2c.h"

#define ADS1115_ADDRESS     ADS1115_ADDR_GND

#define ADS1115_ADDR_GND        0x48    // ADDR pin connected to GND
#define ADS1115_ADDR_VCC        0x49    // ADDR pin connected to VCC
#define ADS1115_ADDR_SDA        0x4A    // ADDR pin connected to SDA
#define ADS1115_ADDR_SCL        0x4B    // ADDR pin connected to SCL

#define ADS1115_CONVERSION_REG  0x00
#define ADS1115_CONFIG_REG      0x01
#define ADS1115_LO_THRESH_REG   0x02
#define ADS1115_HI_THRESH_REG   0x03


#define ADS1115_OS_SINGLE       0x8000
#define ADS1115_MUX_MASK        0x7000
#define ADS1115_MUX_CH0         0x4000  // AINP = AIN0, AINN = GND
#define ADS1115_MUX_CH1         0x5000  // AINP = AIN1, AINN = GND
#define ADS1115_MUX_CH2         0x6000  // AINP = AIN2, AINN = GND
#define ADS1115_MUX_CH3         0x7000  // AINP = AIN3, AINN = GND
#define ADS1115_PGA_MASK        0x0E00
#define ADS1115_PGA_6_144V      0x0000  // +/-6.144V range
#define ADS1115_PGA_4_096V      0x0200  // +/-4.096V range
#define ADS1115_PGA_2_048V      0x0400  // +/-2.048V range
#define ADS1115_PGA_1_024V      0x0600  // +/-1.024V range
#define ADS1115_PGA_0_512V      0x0800  // +/-0.512V range
#define ADS1115_PGA_0_256V      0x0A00  // +/-0.256V range
#define ADS1115_MODE_CONT       0x0000  // Continuous conversion mode
#define ADS1115_MODE_SINGLE     0x0100  // Single-shot mode
#define ADS1115_DR_MASK         0x00E0
#define ADS1115_DR_8SPS         0x0000  // 8 samples per second
#define ADS1115_DR_16SPS        0x0020  // 16 samples per second
#define ADS1115_DR_32SPS        0x0040  // 32 samples per second
#define ADS1115_DR_64SPS        0x0060  // 64 samples per second
#define ADS1115_DR_128SPS       0x0080  // 128 samples per second
#define ADS1115_DR_250SPS       0x00A0  // 250 samples per second
#define ADS1115_DR_475SPS       0x00C0  // 475 samples per second
#define ADS1115_DR_860SPS       0x00E0  // 860 samples per second
#define ADS1115_CMODE_TRAD      0x0000  // Traditional comparator
#define ADS1115_CMODE_WINDOW    0x0010  // Window comparator
#define ADS1115_CPOL_ACTV_LOW   0x0000  // ALERT/RDY pin active low
#define ADS1115_CPOL_ACTV_HIGH  0x0008  // ALERT/RDY pin active high
#define ADS1115_CLATCH_NONLAT   0x0000  // Non-latching comparator
#define ADS1115_CLATCH_LATCH    0x0004  // Latching comparator
#define ADS1115_CQUE_MASK       0x0003
#define ADS1115_CQUE_1CONV      0x0000  // Assert after one conversions
#define ADS1115_CQUE_2CONV      0x0001  // Assert after two conversions
#define ADS1115_CQUE_4CONV      0x0002  // Assert after four conversions
#define ADS1115_CQUE_NONE       0x0003  // Disable comparator

typedef enum {
    ADS1115_CHANNEL_0 = 0,
    ADS1115_CHANNEL_1,
    ADS1115_CHANNEL_2,
    ADS1115_CHANNEL_3
} ads1115_channel_t;

typedef struct {
    i2c_port_t i2c_port;
    uint8_t device_address;
    uint16_t gain;
    uint16_t data_rate;
} ads1115_config_t;

extern ads1115_config_t adc_config;

extern esp_err_t ads1115_init(ads1115_config_t *config);
extern int16_t ads1115_read_raw(ads1115_config_t *config, ads1115_channel_t channel, uint16_t gain);
extern float ads1115_read_voltage(ads1115_config_t *config, ads1115_channel_t channel, uint16_t gain);
extern void ads1115_config_setup(void);

#endif