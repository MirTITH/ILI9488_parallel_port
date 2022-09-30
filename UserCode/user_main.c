#include "main.h"
#include "stm32_adafruit_lcd.h"
#include "cmsis_os.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_demos.h"

void StartDefaultTask(void const *argument)
{
    (void)argument;

    lv_init();
    lv_port_disp_init();
    
    lv_demo_music();

    uint32_t PreviousWakeTime = osKernelSysTick();
    for (;;) {
        lv_task_handler();
        osDelayUntil(&PreviousWakeTime, 5);
    }
}
