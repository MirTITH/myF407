/**
 * @file lv_port_disp_st7735s.c
 * @author X. Y.
 * @brief
 * @version 0.1
 * @date 2022-09-18
 *
 * @copyright Copyright (c) 2022
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp_st7735s.h"
#include "st7735s_driver.h"
#include <stdbool.h>
#include "spi.h"

/*********************
 *      DEFINES
 *********************/

// 屏幕缓冲区大小，可以小于屏幕大小，但建议不要小于屏幕大小的 1/10（使用双重缓冲，所以实际内存占用是 BUFFER_SIZE 的两倍）
#define BUFFER_SIZE 6400

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/*Descriptor of a display driver*/
lv_disp_drv_t disp_drv;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    static lv_disp_draw_buf_t draw_buf_dsc_2;
    static lv_color_t buf_2_1[BUFFER_SIZE];                                /*A buffer for 10 rows*/
    static lv_color_t buf_2_2[BUFFER_SIZE];                                /*An other buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, BUFFER_SIZE); /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    lv_disp_drv_init(&disp_drv); /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = lcd1.width;
    disp_drv.ver_res = lcd1.height;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_2;

    // 屏幕旋转
    disp_drv.sw_rotate    = 1; // 使用软件旋转（速度稍慢）
    disp_drv.rotated      = 0; // 旋转选项

    disp_drv.full_refresh = 0; // 是否每次都要刷新全部屏幕

    // use user_data to get lcd struct
    disp_drv.user_data = &lcd1;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    LCD_Init(&lcd1);
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    LCD_Flush(disp_drv->user_data, area->x1, area->y1, area->x2, area->y2, (uint8_t *)color_p);
}

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
