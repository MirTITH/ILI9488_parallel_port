#include "main.h"
#include "stm32_adafruit_lcd.h"
#include "stdio.h"
#include "cmsis_os.h"
#include "lvgl.h"
#include "lv_port_disp.h"

// uint16_t screenData[10240];

void StartDefaultTask(void const *argument)
{
    (void)argument;

    lv_init();
    lv_port_disp_init();

    // BSP_LCD_Init();

    // BSP_LCD_DrawRGB16Image(1, 1, 50, 60, screenData);

    uint32_t PreviousWakeTime = osKernelSysTick();
    for (;;) {
        lv_task_handler();
        osDelayUntil(&PreviousWakeTime, 5);
    }
}
