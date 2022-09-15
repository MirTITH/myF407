#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"
#include <iostream>
#include "user_main.h"

using namespace std;

float real_num = 2;

void StartDefaultTask(void const *argument)
{
    cout << "Hello ";
    cout << "first line" << endl;
    cout << "This is the 2nd line" << endl;
    for (;;) {
        real_num += 0.1;
        // printf("Hello %lf\n", real_num);

        cout << "The number is: " << real_num << endl;

        osDelay(100);
    }
}
