#include "user_main.hpp"
#include "main.h"
#include "cmsis_os.h"
// #include "stdio_CLI.h"
#include "lvgl.h"
#include "lv_port_disp_st7735s.h"
#include "lv_demos.h"

using namespace std;

void lvglTask(void const *argument)
{
    (void)argument;
    lv_init();
    lv_port_disp_init();
    // lv_demo_stress();
    lv_demo_benchmark();

    uint32_t PreviousWakeTime = osKernelSysTick();
    for (;;) {
        lv_timer_handler();
        osDelayUntil(&PreviousWakeTime, 5);
    }
}

void StartDefaultTask(void const *argument)
{
    (void)argument;

    osThreadDef_t cliThreadDef = {
        .name         = (char *)"lvgl",
        .pthread      = lvglTask,
        .tpriority    = osPriorityNormal,
        .instances    = 0,
        .stacksize    = 1024,
        .buffer       = NULL,
        .controlblock = NULL};

    osThreadCreate(&cliThreadDef, NULL);

    // CLI_Start();

    for (;;) {
        osDelay(100);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
    }
}
