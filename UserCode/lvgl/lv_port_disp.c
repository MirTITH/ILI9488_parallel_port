/**
 * @file lv_port_disp.c
 * @author X. Y.
 * @brief
 * @version 0.1
 * @date 2022-09-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "lv_port_disp.h"
#include <stdbool.h>
#include "lcd.h"
#include "lcd_io_fsmc16.h"
#include "ili9488.h"
#include "dma.h"

extern LCD_DrvTypeDef *lcd_drv;

#define LV_PORT_BUFFER_SIZE 15360

lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/

static void disp_init(void);

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

void LCD_IO_WriteCmd8(uint8_t Cmd);
void LCD_IO_WriteCmd16(uint16_t Cmd);
void LCD_IO_WriteData8(uint8_t Data);
void LCD_IO_WriteData16(uint16_t Data);

#define LCD_ADDR_DATA (LCD_ADDR_BASE + (1 << (LCD_REGSELECT_BIT + 2)) - 2)

#define ILI9488_CASET 0x2A
#define ILI9488_PASET 0x2B
#define ILI9488_RAMWR 0x2C
#define ILI9488_RAMRD 0x2E

#define LCD_IO_WriteData16_to_2x8(dt) \
    {                                 \
        LCD_IO_WriteData8((dt) >> 8); \
        LCD_IO_WriteData8(dt);        \
    }

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    static lv_disp_draw_buf_t draw_buf_dsc;
    static lv_color_t buf_1[LV_PORT_BUFFER_SIZE];                            /*A buffer for 10 rows*/
    static lv_color_t buf_2[LV_PORT_BUFFER_SIZE];                            /*An other buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc, buf_1, buf_2, LV_PORT_BUFFER_SIZE); /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    lv_disp_drv_init(&disp_drv); /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = lcd_drv->GetLcdPixelWidth();
    disp_drv.ver_res = lcd_drv->GetLcdPixelHeight();

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc;

    // 屏幕旋转
    // disp_drv.sw_rotate = 1; // 使用软件旋转（速度稍慢）
    // disp_drv.rotated = 1;   // 旋转选项

    disp_drv.full_refresh = 0; // 是否每次都要刷新全部屏幕

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    lcd_drv->Init();
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    // 设置刷新区域
    LCD_IO_WriteCmd8(ILI9488_CASET);
    LCD_IO_WriteData16_to_2x8(area->x1);
    LCD_IO_WriteData16_to_2x8(area->x2);
    LCD_IO_WriteCmd8(ILI9488_PASET);
    LCD_IO_WriteData16_to_2x8(area->y1);
    LCD_IO_WriteData16_to_2x8(area->y2);

    // 准备写入数据
    *(volatile uint16_t *)LCD_ADDR_BASE = (uint16_t)ILI9488_RAMWR;

    // 使用 DMA 写入数据
    HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream0, (uint32_t)color_p, (uint32_t)LCD_ADDR_DATA, (area->y2 - area->y1 + 1) * (area->x2 - area->x1 + 1));
}
