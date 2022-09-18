/**
 * @file lvgl_task.c
 * @author X. Y.
 * @brief
 * @version 0.1
 * @date 2022-09-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "lvgl_task.h"
#include "lvgl.h"
#include "lv_port_disp_st7735s.h"

static SemaphoreHandle_t xMutex;

// 初始化完成标识
static volatile bool Lvgl_Initialized = false;

void lv_mutex_lock()
{
    xSemaphoreTake(xMutex, portMAX_DELAY);
}

void lv_mutex_unlock()
{
    xSemaphoreGive(xMutex);
}

static void lv_thread(void const *argument)
{
    (void)argument;

    lv_init();
    lv_port_disp_init();

    Lvgl_Initialized = true;

    uint32_t PreviousWakeTime = osKernelSysTick();

    for (;;) {
        lv_mutex_lock();
        lv_task_handler();
        lv_mutex_unlock();
        osDelayUntil(&PreviousWakeTime, 5);
    }
}

void lv_task_create()
{
    Lvgl_Initialized = false;
    // 创建信号量
    xMutex = xSemaphoreCreateMutex();

    // 定义线程
    osThreadDef_t cliThreadDef = {
        .name         = (char *)"lvgl",
        .pthread      = lv_thread,
        .tpriority    = osPriorityNormal,
        .instances    = 0,
        .stacksize    = 1024,
        .buffer       = NULL,
        .controlblock = NULL};

    osThreadCreate(&cliThreadDef, NULL);
    while (Lvgl_Initialized == false); // 等待初始化完成
}
