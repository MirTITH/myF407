#include "user_main.h"
#include "main.h"
#include "cmsis_os.h"
// #include "stdio_CLI.h"
#include "lvgl_task.h"
#include "lv_demos.h"
#include "lvgl_ui.h"

void StartDefaultTask(void const *argument)
{
    (void)argument;

    // CLI_Start();

    lv_task_create();
    lv_mutex_lock();
    // lv_demo_benchmark();
    lv_example_arc_2();
    lv_mutex_unlock();

    for (;;) {
        osDelay(100);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
    }
}
