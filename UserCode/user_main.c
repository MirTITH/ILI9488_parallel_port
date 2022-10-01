#include "main.h"
#include "cmsis_os.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_demos.h"
#include "dma.h"
#include "tim.h"
#include "stdio_CLI.h"
#include "io_retargetToUart.h"
#include "stdio.h"

void DMA_XferCpltCallback(DMA_HandleTypeDef *hdma);

void StartDefaultTask(void const *argument)
{
    (void)argument;

    hdma_memtomem_dma2_stream0.XferCpltCallback = DMA_XferCpltCallback;
    HAL_TIM_Base_Start_IT(&htim6);

    CLI_Start();
    IORetarget_Uart_Receive_IT();

    lv_init();
    lv_port_disp_init();
    
    lv_demo_benchmark();

    uint32_t PreviousWakeTime = osKernelSysTick();
    for (;;) {
        lv_task_handler();
        vTaskDelayUntil(&PreviousWakeTime, 5);
    }
}
