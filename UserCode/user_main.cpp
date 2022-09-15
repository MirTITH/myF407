#include "main.h"
#include "cmsis_os.h"
#include <iostream>
#include "user_main.h"

using namespace std;


void Thread(void const *argument)
{
    const osThreadDef_t *this_thread = (const osThreadDef_t *)argument;
    double dnum                      = 0;

    for (;;) {
        // taskENTER_CRITICAL();
        cout << this_thread->name << ": "<< dnum << endl;
        dnum += 0.1;
        // taskEXIT_CRITICAL();
        osDelay(100);
    }
}

void StartDefaultTask(void const *argument)
{
    (void)argument;

    osThreadDef_t t1 = {
        .name         = (char *)"thread1",
        .pthread      = Thread,
        .tpriority    = osPriorityNormal,
        .instances    = 0,
        .stacksize    = 256,
        .buffer       = NULL,
        .controlblock = NULL};

    osThreadCreate(&t1, &t1);

        osThreadDef_t t2 = {
        .name         = (char *)"thread2",
        .pthread      = Thread,
        .tpriority    = osPriorityNormal,
        .instances    = 0,
        .stacksize    = 256,
        .buffer       = NULL,
        .controlblock = NULL};

    osThreadCreate(&t2, &t2);

        osThreadDef_t t3 = {
        .name         = (char *)"thread3",
        .pthread      = Thread,
        .tpriority    = osPriorityBelowNormal,
        .instances    = 0,
        .stacksize    = 256,
        .buffer       = NULL,
        .controlblock = NULL};

    osThreadCreate(&t3, &t3);

    for (;;) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
        osDelay(200);
    }
}
