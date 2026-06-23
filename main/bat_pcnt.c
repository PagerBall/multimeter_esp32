#include "bat_pcnt.h"

int pcnt_high_limit = 32000;
int pcnt_low_limit = -32000;

pcnt_unit_handle_t power_data_unit = NULL;
pcnt_channel_handle_t power_data_chan = NULL;


void bat_pcnt_init(void)
{
    pcnt_unit_config_t unit_config = {
        .high_limit = pcnt_high_limit,
        .low_limit = pcnt_low_limit,
        .intr_priority = 0,
        .flags.accum_count = 1,
    };
    pcnt_chan_config_t chan_config = {
        .edge_gpio_num = CHAN_GPIO_NUM_17,
        .level_gpio_num = CHAN_GPIO_NUM_NC,
        .flags.virt_level_io_level = 1,
    };
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &power_data_unit));
    ESP_ERROR_CHECK(pcnt_new_channel(power_data_unit, &chan_config, &power_data_chan));

    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(power_data_chan, PCNT_CHANNEL_EDGE_ACTION_HOLD, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(power_data_chan, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    ESP_ERROR_CHECK(pcnt_unit_enable(power_data_unit));
    ESP_ERROR_CHECK(pcnt_unit_start(power_data_unit));
}






void bat_pcnt_start(void)
{
    pcnt_unit_start(power_data_unit);
}

void bat_pcnt_stop(void)
{
    pcnt_unit_stop(power_data_unit);
}

void bat_pcnt_clear(void)
{
    pcnt_unit_clear_count(power_data_unit);
}


void bat_pcnt_send_count(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int pulse_count = 0;

    pcnt_unit_get_count(power_data_unit, &pulse_count);
    xQueueSendFromISR(bat_pcnt_2_ui_queue, &pulse_count, &xHigherPriorityTaskWoken);
}


