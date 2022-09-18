#include "user_main.hpp"
#include "main.h"
#include "cmsis_os.h"
// #include "stdio.h"
// #include "stdio_CLI.h"
#include "lvgl.h"
#include "st7735s_lvgl.h"
#include "st7735s_driver.h"
#include "lv_demos.h"

using namespace std;

void lvglTask(void const *argument)
{
    (void)argument;
    LCD_Init();
    lv_init();
    st7735_lvgl_init();
    lv_demo_stress();


    for (;;) {
        lv_timer_handler();
        osDelay(5);
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
