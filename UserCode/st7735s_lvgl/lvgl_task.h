/**
 * @file lvgl_task.h
 * @author X. Y.  
 * @brief 
 * @version 0.1
 * @date 2022-09-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "cmsis_os.h"
void lv_task_create();

void lv_mutex_lock();

void lv_mutex_unlock();

#ifdef __cplusplus
}
#endif
