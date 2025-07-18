// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "../ui.h"

void ui_ScreenAlarm_screen_init(void)
{
    ui_ScreenAlarm = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScreenAlarm, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ScreenAlarm, lv_color_hex(0x030303), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenAlarm, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImageAlarm = lv_img_create(ui_ScreenAlarm);
    lv_img_set_src(ui_ImageAlarm, &ui_img_450751469);
    lv_obj_set_width(ui_ImageAlarm, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ImageAlarm, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ImageAlarm, 0);
    lv_obj_set_y(ui_ImageAlarm, -30);
    lv_obj_set_align(ui_ImageAlarm, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageAlarm, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_ImageAlarm, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label30 = lv_label_create(ui_ScreenAlarm);
    lv_obj_set_width(ui_Label30, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label30, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label30, 0);
    lv_obj_set_y(ui_Label30, 30);
    lv_obj_set_align(ui_Label30, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label30, "报警中...");
    lv_obj_set_style_text_color(ui_Label30, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label30, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label30, &ui_font_Font22, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_ScreenAlarm, ui_event_ScreenAlarm, LV_EVENT_ALL, NULL);

}
