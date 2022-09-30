#include "user_main.h"
#include "main.h"
#include "stm32_adafruit_lcd.h"
#include "stdio.h"

void user_main()
{
    BSP_LCD_Init();
    uint32_t id = 0;

    for (;;) {
        BSP_LCD_Clear(LCD_COLOR_BLACK);
        HAL_Delay(100);
        BSP_LCD_Clear(LCD_COLOR_MAGENTA);
        HAL_Delay(100);
        BSP_LCD_Clear(LCD_COLOR_YELLOW);
        HAL_Delay(100);
        BSP_LCD_Clear(LCD_COLOR_WHITE);
        HAL_Delay(100);
    }
}
