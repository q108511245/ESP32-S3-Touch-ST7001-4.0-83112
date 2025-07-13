#pragma once

#include "ui.h"
#include "service_view.h"
#include "project_config.h"

/**
 * @brief 自定义按钮点击事件处理函数
 */
void custom_btn_click_handler(lv_event_t *e);

/**
 * @brief 自定义动画结束回调函数
 */
void custom_anim_cb_handler(lv_anim_t *anim);

/**
 * @brief 自定义开关点击事件处理函数
 */
void custom_switch_cb_handler(lv_event_t *e);

// #include "ui.h"
// #include "service_ui_events.h"

// void clicked_callback(lv_event_t * e)
// {
// 	custom_btn_click_handler(e);
// }

// void logoAnim_end_cb(lv_anim_t *anim)
// {
//    custom_anim_cb_handler(anim);
// }

// void switch_callback(lv_event_t * e)
// {
// 	custom_switch_cb_handler(e);
// }

// lv_anim_set_ready_cb(&PropertyAnimation_0, logoAnim_end_cb);

// void logoAnim_end_cb(lv_anim_t *anim);

// # 新增 cmakelist filelist.txt
// images/ui_img_qing60_png.c
// images/ui_img_dawu60_png.c
// images/ui_img_duoyun60_png.c
// images/ui_img_leizhenyu60_png.c
// images/ui_img_shachenbao80_png.c
// images/ui_img_wushuju80_png.c
// images/ui_img_xue80_png.c
// images/ui_img_yintian80_png.c
// images/ui_img_yu80_png.c
// images/ui_img_zhenyu80_png.c

// 新增 ui.h 文件
// LV_IMG_DECLARE(ui_img_qing60_png);
// LV_IMG_DECLARE(ui_img_dawu60_png);
// LV_IMG_DECLARE(ui_img_duoyun60_png);
// LV_IMG_DECLARE(ui_img_leizhenyu60_png);
// LV_IMG_DECLARE(ui_img_shachenbao80_png);
// LV_IMG_DECLARE(ui_img_wushuju80_png);
// LV_IMG_DECLARE(ui_img_xue80_png);
// LV_IMG_DECLARE(ui_img_yintian80_png);
// LV_IMG_DECLARE(ui_img_yu80_png);
// LV_IMG_DECLARE(ui_img_zhenyu80_png);