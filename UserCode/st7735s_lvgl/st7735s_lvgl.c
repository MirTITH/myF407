/**
 * @file st7735s_lvgl.c
 * @author X. Y.
 * @brief st7735s 的 lvgl 接口
 * @version 0.1
 * @date 2022-09-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "lvgl.h"
#include "st7735s_driver.h"
#include "cmsis_os.h"

// 缓冲区大小，建议高于 1/10 screen sized.
#define BUFFER_SIZE (128 * 160 / 4)

void vApplicationTickHook(void)
{
    lv_tick_inc(1000 / osKernelSysTickFrequency);
}

void st7735_flush_callback(struct _lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    LCD_Zone_Set(area->x1, area->y1, area->x2, area->y2);
    LCD_DC_DATA();
    LCD_CS_ENABLE();

    HAL_SPI_Transmit_DMA(&hspi2, (uint8_t *)color_p, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1) * 2);
    while (hspi2.State != HAL_SPI_STATE_READY)
        ;
    LCD_CS_DISABLE();
    lv_disp_flush_ready(disp_drv);
}

void st7735_lvgl_init(void)
{
    /*A static or global variable to store the buffers*/
    static lv_disp_draw_buf_t disp_buf;

    /*Static or global buffer(s). The second buffer is optional*/
    static lv_color_t buf_1[BUFFER_SIZE];
    static lv_color_t buf_2[BUFFER_SIZE];

    /*Initialize `disp_buf` with the buffer(s). With only one buffer use NULL instead buf_2 */
    lv_disp_draw_buf_init(&disp_buf, buf_1, buf_2, BUFFER_SIZE);

    static lv_disp_drv_t disp_drv;             /*A variable to hold the drivers. Must be static or global.*/
    lv_disp_drv_init(&disp_drv);               /*Basic initialization*/
    disp_drv.draw_buf = &disp_buf;             /*Set an initialized buffer*/
    disp_drv.flush_cb = st7735_flush_callback; /*Set a flush callback to draw to the display*/
    disp_drv.hor_res  = DISPLAY_H;             /*Set the horizontal resolution in pixels*/
    disp_drv.ver_res  = DISPLAY_W;             /*Set the vertical resolution in pixels*/
    disp_drv.sw_rotate = 1;
    disp_drv.rotated = 1;

    lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/
}
