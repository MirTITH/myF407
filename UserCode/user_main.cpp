#include "user_main.hpp"
#include "main.h"
#include "cmsis_os.h"
#include "stdio_CLI.h"

using namespace std;

void StartDefaultTask(void const *argument)
{
    (void)argument;

    CLI_Start();

    for (;;) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
        osDelay(100);
    }
}
