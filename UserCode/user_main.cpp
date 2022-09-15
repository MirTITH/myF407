#include "main.h"
// #include "cmsis_os.h"
#include "stdio.h"
#include <iostream>
#include "user_main.h"

using namespace std;

float real_num = 2;

void StartDefaultTask(void const *argument)
{
    cout << "The number is: " << endl;
    cout << "The number is: " << endl;
    cout << "The number is: " << endl;
    for (;;) {
        real_num += 0.1;
        // printf("Hello %lf\n", real_num);

        cout << "The number is: " << real_num << endl;

        HAL_Delay(100);
    }
}
