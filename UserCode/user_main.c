#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "stdio_CLI.h"
#include "io_retargetToUart.h"
#include "lvgl_task.h"
#include "lvgl.h"

extern lv_font_t msyh_24;

void DMA_XferCpltCallback(DMA_HandleTypeDef *hdma);

lv_color_hsv_t bg_color = {
    .h = 207,
    .s = 100,
    .v = 84};

void StartDefaultTask(void const *argument)
{
    (void)argument;

    hdma_memtomem_dma2_stream0.XferCpltCallback = DMA_XferCpltCallback;

    IORetarget_Uart_Receive_IT();
    CLI_Start();

    lv_task_create();

    lv_mutex_lock();
    lv_obj_t *main_label = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(main_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(main_label, "休对故人思故国，且将新火试新茶。敬请开始吧。");
    lv_obj_set_width(main_label, 280); /*Set smaller width to make the lines wrap*/
    lv_obj_set_style_text_align(main_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(lv_scr_act(), &msyh_24, 0);
    lv_obj_set_style_text_color(lv_scr_act(), lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hsv_to_rgb(bg_color.h, bg_color.s, bg_color.v), 0);
    lv_mutex_unlock();

    osDelay(10000);

    for (;;) {
        bg_color.h++;
        if (bg_color.h > 359) bg_color.h = 0;
        lv_mutex_lock();
        lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hsv_to_rgb(bg_color.h, bg_color.s, bg_color.v), 0);
        lv_mutex_unlock();
        osDelay(20);
    }
}
