// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "ui.h"
#include "service_ui_events.h"

void clicked_callback(lv_event_t * e)
{
	custom_btn_click_handler(e);
}

void logoAnim_end_cb(lv_anim_t *anim)
{
   custom_anim_cb_handler(anim);
}

void switch_callback(lv_event_t * e)
{
	custom_switch_cb_handler(e);
}
