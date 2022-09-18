/**
 * @file user_IRQ.c
 * @author X. Y.  
 * @brief 用户中断管理
 * @version 0.1
 * @date 2022-09-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "lvgl.h"
#include "spi.h"
#include "st7735s_driver.h"
#include "lv_port_disp_st7735s.h"
#include "cmsis_os.h"
#include "io_retargetToUart.h"

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi == &hspi2)
    {
        LCD_SPI_TxCpltCallback(&lcd1);
        lv_disp_flush_ready(&disp_drv);
    }
}

void vApplicationTickHook(void)
{
    lv_tick_inc(1000 / osKernelSysTickFrequency);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    IORetarget_Uart_RxCpltCallback(huart);
}