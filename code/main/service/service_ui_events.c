#include "service_ui_events.h"
#include "driver_nvs.h"
#include "service_wifi.h"

void custom_anim_cb_handler(lv_anim_t *anim)
{
    send_xQueue(EVENT_ANIM_LOGOFINISH);
}

/**
 * @brief 自定义开关点击事件处理函数
 */
void custom_switch_cb_handler(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target(e);
    bool state = lv_obj_has_state(sw, LV_STATE_CHECKED);
    if (state)
    {
        if (e->target == ui_SwitchMQTT)
        {
            logger_log(TAG, "start mqtt");
            send_xQueue(EVENT_MQTT_START);
        }
    }
    else
    {
        if (e->target == ui_SwitchMQTT)
        {
            logger_log(TAG, "stop mqtt");
            send_xQueue(EVENT_MQTT_STOP);
        }
    }
}

/**
 * @brief 自定义按钮点击事件处理函数
 */
void custom_btn_click_handler(lv_event_t *e)
{
    uint8_t event_type = EVENT_EVENT_CLICKED;
    if (e->code == LV_EVENT_CLICKED)
    {
        if (e->target == ui_ImageRefView)
        {
            event_type = EVENT_WIFI_SCANSTART;
        }
        else if (e->target == ui_ButtonWiFiSure)
        {
            char buf[64];
            lv_roller_get_selected_str(ui_RollerWiFiSelect, buf, sizeof(buf));
            logger_log(TAG, "WiFiName: %s\n", buf);
            // 判断长度
            if (strlen(buf) > 1)
            {
                // 把选择的WiFi缓存起来
                saveWiFiConfigSsid(buf);
                // 跳转到输入密码页面
                event_type = UI_OPEN_WIFIPASSWORD_VIEW;
            }
        }
        else if (e->target == ui_ButtonWiFiSure2)
        {
            const char *text = lv_textarea_get_text(ui_TextAreaPassword);
            logger_log(TAG, "Password: %s\n", text);
            // 判断长度
            if (strlen(text) > 1)
            {
                // 把选择的WiFi缓存起来
                char password[64];
                strncpy(password, text, sizeof(password) - 1);
                password[sizeof(password) - 1] = '\0';
                saveWiFiConfigPassWord(password);
                event_type = EVENT_WIFI_SELECT_RESULT;
            }
        }
        else if (e->target == ui_ImgButtonUpdate)
        {
            // 更新
            event_type = EVENT_OTA_REFRESH_START;
        }
        else if (e->target == ui_ImgButtonUpdateCancel)
        {
            // 取消更新
            event_type = EVENT_OTA_REFRESH_CANCEL;
        }
        else if (e->target == ui_ScreenClock)
        {
            // 时钟页面点击
            event_type = EVENT_CLICKED_CLOCK_VIEW;
        }
        else if (e->target == ui_ImgButtonRestWfi)
        {
            // 重置WiFi
            event_type = EVENT_CLICKED_RESTWIFI;
        }
        else if (e->target == ui_ImgButton1)
        {
            // 扫描WiFi
            event_type = EVENT_WIFI_SCANSTART;
        }
        else if (e->target == ui_comp_get_child(ui_ContainerSelectTime,
                                                UI_COMP_CONTAINERSELECTTIME_CONTAINERALL_CONTAINER6_BUTTONSURE))
        {
            // 扫描WiFi
            event_type = EVENT_CLOCK_ADD_SUBMIT;
        }
        else if (e->target == ui_comp_get_child(ui_ContainerDialog,
                                                UI_COMP_CONTAINERDIALOG_CONTAINER25_CONTAINER27_BUTTONSUREDELETE))
        {
            // 删除闹钟
            event_type = EVENT_CLOCK_DELETE_SURE;
        }
        else if (e->target == ui_ScreenAlarm)
        {
            // 报警页面点击返回主页
            event_type = UI_SNTP_CONNECTING_OVER;
        }
    }
    else if (e->code == LV_EVENT_SCREEN_LOADED)
    {
        if (e->target == ui_ScreenAlarmClock)
        {
            logger_log(TAG, "闹钟页面加载完成");
            event_type = EVENT_CLOCK_LOADED_OVER;
        }
    }
    // 发送事件
    send_xQueue(event_type);
}