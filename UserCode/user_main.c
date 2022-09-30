#include "main.h"
#include "stm32_adafruit_lcd.h"
#include "stdio.h"

void StartDefaultTask(void const *argument)
{
    (void)argument;
    BSP_LCD_Init();

    uint16_t color = {0};
    uint32_t last_tick;

    for (;;) {
        last_tick = HAL_GetTick();
        for (int i = 0; i < 100; i++) {
            BSP_LCD_Clear(color++);
        }
        printf("fps:%f\n", 100000.0 / (HAL_GetTick() - last_tick));
    }
}
