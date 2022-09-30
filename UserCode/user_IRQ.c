/**
 * @file user_IRQ.c
 * @author X. Y.
 * @brief 用户中断管理
 * @version 0.1
 * @date 2022-09-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "lvgl.h"
#include "cmsis_os.h"

void vApplicationTickHook(void)
{
    lv_tick_inc(1000 / osKernelSysTickFrequency);
}
