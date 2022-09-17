#include "user_main.hpp"
#include "main.h"
#include "cmsis_os.h"
#include "stdio_CLI.h"

extern "C" {
#include "st7735_driver.h"
}

using namespace std;

void StartDefaultTask(void const *argument)
{
    (void)argument;

    CLI_Start();

    LCD_Init();

    LCD_fillScreen(ST7735_BLUE);

    for (;;) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
        osDelay(100);
    }
}
