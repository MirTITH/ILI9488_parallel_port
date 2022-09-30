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

extern LCD_DrvTypeDef *lcd_drv;

#define LV_PORT_BUFFER_SIZE 15360

static void disp_init(void);

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

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

    static lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/

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
    lcd_drv->DrawRGBImage(disp_drv->hor_res - area->x2 - 1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (uint16_t *)color_p);

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}
