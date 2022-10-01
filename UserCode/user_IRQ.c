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
#include "dma.h"
#include "io_retargetToUart.h"

volatile uint32_t RunTimeCounterValue = 0;

void vApplicationTickHook(void)
{
    lv_tick_inc(1000 / osKernelSysTickFrequency);
}

extern lv_disp_drv_t disp_drv;

void DMA_XferCpltCallback(DMA_HandleTypeDef *hdma)
{
    lv_disp_flush_ready(&disp_drv);
}

unsigned long getRunTimeCounterValue(void)
{
    return RunTimeCounterValue;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    IORetarget_Uart_RxCpltCallback(huart);
}
