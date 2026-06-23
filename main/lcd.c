#include "lcd.h"
#include "custom.h"
#include "events_init.h"

TaskHandle_t lcd_gui_task_handle = NULL;
SemaphoreHandle_t lvgl_mutex = NULL;
void lcd_gui_task(void *pvParameters)
{
    static lv_color_t buf1[DISP_BUF_SIZE];
    static lv_color_t buf2[DISP_BUF_SIZE];
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, DISP_BUF_SIZE);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = disp_driver_flush;
    disp_drv.hor_res = CONFIG_LV_HOR_RES_MAX;
    disp_drv.ver_res = CONFIG_LV_VER_RES_MAX;
    lv_disp_drv_register(&disp_drv);

    setup_ui(&guider_ui);
    events_init(&guider_ui);
    custom_init(&guider_ui);

    while (1)
    {
        if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE)
        {
            lv_timer_handler();
            xSemaphoreGive(lvgl_mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}