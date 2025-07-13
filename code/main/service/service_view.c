#include "service_view.h"
#include "service_weather.h"

uint32_t current_item_index = 0;

/**
 * 跳转wifi的选择页面
 */
void goWiFiSelectView(uint8_t hasBack)
{
    // 隐藏掉返回按钮
    if (!hasBack)
    {
        if (ui_ImgButtonClose6)
        {
            lv_obj_add_flag(ui_ImgButtonClose6, LV_OBJ_FLAG_HIDDEN);
        }
    }
    if (ui_ScreenWiFiSetting)
    {
        _ui_screen_change(&ui_ScreenWiFiSetting, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenWiFiSetting_screen_init);
    }
}

/**
 * 显示选择页面的加载动画
 */
void showSelectViewLoading()
{
    lv_obj_clear_flag(ui_ContainerLoading2, LV_OBJ_FLAG_HIDDEN);
}

/**
 * 隐藏选择页面的加载动画
 */
void hindSelectViewLoading()
{
    lv_obj_add_flag(ui_ContainerLoading2, LV_OBJ_FLAG_HIDDEN);
}

/**
 * 显示WiFi连接的加载动画
 */
void showSelectPasswordViewLoading()
{
    lv_obj_clear_flag(ui_ContainerLoading3, LV_OBJ_FLAG_HIDDEN);
}

/**
 * 隐藏WiFi连接的加载动画
 */
void hindSelectPasswordViewLoading()
{
    lv_obj_add_flag(ui_ContainerLoading3, LV_OBJ_FLAG_HIDDEN);
}

void goClockView()
{
    _ui_screen_change(&ui_ScreenClock, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenClock_screen_init);
}

/**
 * 进入404页面
 */
void go404Page()
{
    _ui_screen_change(&ui_Screen404, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Screen404_screen_init);
}

/**
 * 进入主页
 */
void goMainView()
{
    _ui_screen_change(&ui_ScreenMain, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenMain_screen_init);
}

/**
 * 进入wifi密码
 */
void goWiFiPasswordView()
{
    _ui_screen_change(&ui_ScreenWiFiPassword, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenWiFiPassword_screen_init);
}

void goAlarmView()
{
    _ui_screen_change(&ui_ScreenAlarm, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenAlarm_screen_init);
}

// 函数：listWiFi
// 功能：列出WiFi选项
// 参数：const char * options：WiFi选项
void listWiFi(const char *options)
{
    if (lvgl_port_lock(-1))
    {
        // 设置滚轮选项
        lv_roller_set_options(ui_RollerWiFiSelect, options, LV_ROLLER_MODE_NORMAL);
        lvgl_port_unlock();
    }
}

/**
 * 设置时钟角度
 */
void setMainClock(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, uint8_t year, uint8_t weekday)
{
    // 静态变量保存上一次的值
    static uint8_t last_hours = 0;
    static uint8_t last_minutes = 0;
    static uint8_t last_day = 0;
    static uint8_t last_month = 0;
    static uint8_t last_year = 0;
    static uint8_t last_weekday = 0;

    // 检查数据是否变化
    if (hours == last_hours && minutes == last_minutes && day == last_day && month == last_month && year == last_year && weekday == last_weekday)
    {
        return; // 数据未变化，不刷新控件
    }
    logger_log(TAG, "refresh clock view");
    // 更新静态变量为最新数据
    last_hours = hours;
    last_minutes = minutes;
    last_day = day;
    last_month = month;
    last_year = year;
    last_weekday = weekday;
    // 日期和星期显示
    const char *weekday_names[] = {"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};
    lv_label_set_text(ui_LabelWeek, weekday_names[weekday]);
    char strftime_buf[20];
    char strftimeHour_buf[20];
    char strftimeMin_buf[20];
    snprintf(strftime_buf, sizeof(strftime_buf), "%d月%d日", month, day);
    snprintf(strftimeHour_buf, sizeof(strftimeHour_buf), hours < 10 ? "0%d" : "%d", hours);
    snprintf(strftimeMin_buf, sizeof(strftimeMin_buf), minutes < 10 ? "0%d" : "%d", minutes);
    if (lvgl_port_lock(-1))
    {
        lv_label_set_text(ui_LabelDate, strftime_buf);
        lv_label_set_text(ui_LabelHour, strftimeHour_buf);
        lv_label_set_text(ui_LabelMin, strftimeMin_buf);
        // lv_label_set_text(ui_LabelMaoHao, seconds % 2 ? ":" : " ");
        lvgl_port_unlock();
    }
}

/**
 * 设置时钟显示
 */
void setClockView(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, uint8_t year, uint8_t weekday)
{

    char strftimeHour_buf[20];
    char strftimeMin_buf[20];
    char strftimeSecond_buf[20];

    snprintf(strftimeHour_buf, sizeof(strftimeHour_buf), hours < 10 ? "0%d" : "%d", hours);
    snprintf(strftimeMin_buf, sizeof(strftimeMin_buf), minutes < 10 ? "0%d" : "%d", minutes);
    snprintf(strftimeSecond_buf, sizeof(strftimeSecond_buf), seconds < 10 ? "0%d" : "%d", seconds);
    if (lvgl_port_lock(-1))
    {
        lv_label_set_text(ui_LabelClockHour, strftimeHour_buf);
        lv_label_set_text(ui_LabelClockMin, strftimeMin_buf);
        lv_label_set_text(ui_LabelClockSec, strftimeSecond_buf);
        lvgl_port_unlock();
    }
}

/**
 * 启动升级
 */
void startUpgrade(void)
{
    lv_obj_add_flag(ui_ImgButtonUpdate, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImgButtonUpdateCancel, LV_OBJ_FLAG_HIDDEN);
}

/**
 * 取消升级
 */
void cancelUpgrade(void)
{
    lv_obj_add_flag(ui_ImgButtonUpdateCancel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImgButtonUpdate, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui_LabelProgress, "0%");
    lv_arc_set_value(ui_Arc1, 0);
}

/**
 * 下载进度
 */
void setUpgradeText(const char *upgradeText, const char *progressText, int16_t value)
{
    if (lvgl_port_lock(-1))
    {
        lv_label_set_text(ui_LabelProgress, progressText);
        lv_arc_set_value(ui_Arc1, value);
        lvgl_port_unlock();
    }
}

/**
 * 当前是否是主页面
 */
bool isMainView()
{
    // 获取当前活动屏幕指针
    lv_obj_t *current_screen = lv_scr_act();
    if (current_screen == ui_ScreenMain)
    {
        return true;
    }
    return false;
}

/**
 * 设置WiFi名称和密码
 */
void setWiFiText(char *ssid, char *password)
{
    lv_label_set_text(ui_LabelWiFiName, ssid);
    lv_label_set_text(ui_LabelWiFiPassword, password);
}

/**
 * 设置wifi状态
 */
void setWiFiStatusText(char *status)
{
    lv_label_set_text(ui_LabelWiFiStatus, status);
}

/**
 * 设置MQTT名称和密码
 */
void setMQTTText(char *uri, char *topic)
{
    lv_label_set_text(ui_LabelMqttUrl, uri);
    lv_label_set_text(ui_LabelMqttPort, topic);
}

/**
 * 设置MQTT状态
 */
void setMQTTStatusText(char *status)
{
    lv_label_set_text(ui_LabelMqttStatus, status);
    if (strcmp(status, "已连接") == 0)
    {
        lv_obj_add_state(ui_SwitchMQTT, LV_STATE_CHECKED);
        lv_obj_set_style_text_color(ui_LabelMqttStatus, lv_color_hex(0x16F30F), LV_STATE_DEFAULT);
    }
    else
    {
        lv_obj_clear_state(ui_SwitchMQTT, LV_STATE_CHECKED);
        lv_obj_set_style_text_color(ui_LabelMqttStatus, lv_color_hex(0xFF0000), LV_STATE_DEFAULT);
    }
}

/**
 * 设置WiFi名称
 */
void setWiFiTextPasswordPage(char *ssid)
{
    lv_label_set_text(ui_selectWifiName, ssid);
}

/**
 * 隐藏时钟无数据视图
 */
void hideClockNodataView(void)
{
    lv_obj_add_flag(ui_ContainerNoData, LV_OBJ_FLAG_HIDDEN);
}

/**
 * 显示时钟无数据视图
 */
void showClockNodataView(void)
{
    lv_obj_clear_flag(ui_ContainerNoData, LV_OBJ_FLAG_HIDDEN);
}

void ui_event_ClockItem(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *item = lv_event_get_target(e);
    lv_obj_t *list = lv_obj_get_parent(item);
    // 获取点击的item索引
    current_item_index = lv_obj_get_index(item);
    // 打印
    logger_log(TAG, "ClockItem: %d", (int)current_item_index);
    if (event_code == LV_EVENT_LONG_PRESSED)
    {

        _ui_flag_modify(ui_ContainerDialog, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    }
}

/**
 * 清空时钟列表
 */
void clear_clock_list(void)
{
    // 确保容器存在
    if (ui_ContainerClockList == NULL)
    {
        return;
    }

    // 获取容器的第一个子对象
    lv_obj_t *child = lv_obj_get_child(ui_ContainerClockList, 0);

    // 循环删除所有子对象
    while (child != NULL)
    {
        lv_obj_del(child);                                  // 删除子对象
        child = lv_obj_get_child(ui_ContainerClockList, 0); // 获取新的第一个子对象
    }
}

void ui_event_SwitchClock(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *item = lv_event_get_target(e);
    // 获取用户数据
    void *user_data = lv_event_get_user_data(e);
    uint8_t position = (uint8_t)(uintptr_t)user_data;

    // 打印或处理 position
    logger_log(TAG, "Switch changed at position: %d", (int)position);
    // 获取点击的item索引
    current_item_index = (int)position;
    // 打印
    logger_log(TAG, "ClockItem: %d", (int)current_item_index);
    if (event_code == LV_EVENT_VALUE_CHANGED)
    {
        send_xQueue(EVENT_CLOCK_STATUS_CHANGE);
    }
}

/**
 * 添加时钟项
 */
void addClockItem(char *textTime, uint8_t status, uint8_t position)
{
    lv_obj_t *ui_ClockItem = lv_obj_create(ui_ContainerClockList);
    lv_obj_remove_style_all(ui_ClockItem);
    lv_obj_set_height(ui_ClockItem, 90);
    lv_obj_set_width(ui_ClockItem, lv_pct(90));
    lv_obj_set_align(ui_ClockItem, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_ClockItem, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_ClockItem, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_ClockItem, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_ClockItem, lv_color_hex(0x232323), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ClockItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_ClockItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_ClockItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_ClockItem, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_ClockItem, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_pad_left(ui_ClockItem, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_ClockItem, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_ClockItem, 5, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_ClockItem, 5, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    lv_obj_t *ui_ContainerTen = lv_obj_create(ui_ClockItem);
    lv_obj_remove_style_all(ui_ContainerTen);
    lv_obj_set_height(ui_ContainerTen, 80);
    lv_obj_set_width(ui_ContainerTen, lv_pct(100));
    lv_obj_set_align(ui_ContainerTen, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_ContainerTen, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_ContainerTen, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ContainerTen, lv_color_hex(0x232323), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ContainerTen, 200, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *ui_ContainerEight = lv_obj_create(ui_ContainerTen);
    lv_obj_remove_style_all(ui_ContainerEight);
    lv_obj_set_width(ui_ContainerEight, lv_pct(90));
    lv_obj_set_height(ui_ContainerEight, lv_pct(100));
    lv_obj_set_align(ui_ContainerEight, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_ContainerEight, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_ContainerEight, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_BETWEEN);
    lv_obj_clear_flag(ui_ContainerEight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *ui_ClockTime = lv_label_create(ui_ContainerEight);
    lv_obj_set_width(ui_ClockTime, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_ClockTime, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_ClockTime, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ClockTime, textTime);
    lv_obj_set_style_text_color(ui_ClockTime, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_ClockTime, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ClockTime, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *ui_SwitchClock = lv_switch_create(ui_ContainerEight);
    lv_obj_set_width(ui_SwitchClock, 50);
    lv_obj_set_height(ui_SwitchClock, 25);
    lv_obj_set_align(ui_SwitchClock, LV_ALIGN_CENTER);
    // lv_obj_set_user_data(ui_SwitchClock,position);
    // 设置状态
    if (status == 1)
    {
        lv_obj_add_state(ui_SwitchClock, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(ui_SwitchClock, LV_STATE_CHECKED);
    }
    lv_obj_add_event_cb(ui_SwitchClock, ui_event_SwitchClock, LV_EVENT_VALUE_CHANGED, (void *)(uintptr_t)position);
    lv_obj_add_event_cb(ui_ClockItem, ui_event_ClockItem, LV_EVENT_LONG_PRESSED, NULL);
}

/**
 * 隐藏时钟删除对话框
 */
void hideDeleteDialog()
{
    _ui_flag_modify(ui_ContainerDialog, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
}

/**
 * 隐藏新增闹钟对话框
 */
void hideClockSureDialog()
{
    _ui_flag_modify(ui_ContainerSelectTime, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
}

/**
 * @brief 根据天气描述字符串获取对应的天气图标
 * @param day_weather 天气描述字符串（如 "晴", "多云转小雨" 等）
 *
 */
void get_weather_icon(lv_obj_t *obj, u_int8_t size, const char *day_weather)
{
    if (!day_weather || strlen(day_weather) == 0)
    {
        lv_img_set_src(obj, size == 80 ? &ui_img_qing80_png : &ui_img_qing60_png);
    }
    else
    {
        // 转换为小写便于匹配（可选）
        char weather_copy[64];
        strncpy(weather_copy, day_weather, sizeof(weather_copy) - 1);
        weather_copy[sizeof(weather_copy) - 1] = '\0';

        for (int i = 0; weather_copy[i]; i++)
        {
            weather_copy[i] = tolower(weather_copy[i]); // 全部转小写
        }

        // 匹配关键词并返回对应图标
        if (strstr(weather_copy, "晴"))
        {
            lv_img_set_src(obj, size == 80 ? &ui_img_qing80_png : &ui_img_qing60_png);
        }
        else if (strstr(weather_copy, "多云"))
        {
            lv_img_set_src(obj, size == 80 ? &ui_img_qing80_png : &ui_img_duoyun60_png);
        }
        else if (strstr(weather_copy, "阴"))
        {
            lv_img_set_src(obj, size == 80 ? &ui_img_qing80_png : &ui_img_duoyun60_png);
        }
        else if (strstr(weather_copy, "雷") && strstr(weather_copy, "雨"))
        {
            lv_img_set_src(obj, size == 80 ? &ui_img_zhenyu80_png : &ui_img_leizhenyu60_png);
        }
        else if (strstr(weather_copy, "雨"))
        {
            lv_img_set_src(obj, size == 80 ? &ui_img_yu80_png : &ui_img_yu60_png);
        }
        else if (strstr(weather_copy, "雪"))
        {
            lv_img_set_src(obj, size == 80 ? &ui_img_xue80_png : &ui_img_xue60_png);
        }
        else if (strstr(weather_copy, "雾") || strstr(weather_copy, "霾"))
        {
            lv_img_set_src(obj, size == 80 ? &ui_img_wushuju80_png : &ui_img_dawu60_png);
        }
        else
        {
            lv_img_set_src(obj, size == 80 ? &ui_img_wushuju80_png : &ui_img_wushuju60_png);
        }
    }
}

/**
 * 显示天气数据
 */
void showWeatherData(weather_data_t *weatherData)
{
    // 显示温度
    lv_label_set_text(ui_LabelTempTpday, weatherData[0].day_air_temperature);
    lv_label_set_text(ui_LabelWindText, weatherData[0].day_wind_power);
    lv_label_set_text(ui_LabelWind, weatherData[0].day_wind_direction);
    lv_label_set_text(ui_LabelHBA, weatherData[0].air_press);

    lv_label_set_text_fmt(ui_LabelTempValue, "%s℃", weatherData[1].day_air_temperature);
    lv_label_set_text_fmt(ui_LabelTempValue2, "%s℃", weatherData[2].day_air_temperature);
    lv_label_set_text_fmt(ui_LabelTempValue3, "%s℃", weatherData[3].day_air_temperature);
    lv_label_set_text_fmt(ui_LabelTempValue4, "%s℃", weatherData[4].day_air_temperature);
    lv_label_set_text_fmt(ui_LabelTempValue5, "%s℃", weatherData[5].day_air_temperature);
    // 显示湿度
    lv_label_set_text(ui_LabelHumValue, weatherData[1].jiangshui);
    lv_label_set_text(ui_LabelHumValue2, weatherData[2].jiangshui);
    lv_label_set_text(ui_LabelHumValue3, weatherData[3].jiangshui);
    lv_label_set_text(ui_LabelHumValue4, weatherData[4].jiangshui);
    lv_label_set_text(ui_LabelHumValue5, weatherData[5].jiangshui);
    // 显示日期
    //  日期和星期显示
    const char *weekday_names[] = {"星期一", "星期二", "星期三", "星期四", "星期五", "星期六", "星期日"};
    lv_label_set_text(ui_LabelWeatherTime, weekday_names[weatherData[1].weekday-1]);
    lv_label_set_text(ui_LabelWeatherTime2, weekday_names[weatherData[2].weekday-1]);
    lv_label_set_text(ui_LabelWeatherTime3, weekday_names[weatherData[3].weekday-1]);
    lv_label_set_text(ui_LabelWeatherTime4, weekday_names[weatherData[4].weekday-1]);
    lv_label_set_text(ui_LabelWeatherTime5, weekday_names[weatherData[5].weekday-1]);
    // 显示天气图标
    get_weather_icon(ui_ImageWeatherToday, 80, weatherData[0].day_weather);
    get_weather_icon(ui_ImageWeatherIcon, 60, weatherData[1].day_weather);
    get_weather_icon(ui_ImageWeatherIcon2, 60, weatherData[2].day_weather);
    get_weather_icon(ui_ImageWeatherIcon3, 60, weatherData[3].day_weather);
    get_weather_icon(ui_ImageWeatherIcon4, 60, weatherData[4].day_weather);
    get_weather_icon(ui_ImageWeatherIcon5, 60, weatherData[5].day_weather);
}