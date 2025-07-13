#include "service_wifi.h"
#include "esp_system.h"

// 是否需要覆盖WiFi账号密码
static int isReConnect = 1;

char wifiScanResult[512] = {0};

// 选择的WiFi名称
char mSsid[50] = {0};
// 选择的WiFi密码
char mPassword[50] = {0};

/***********************************************************************
 * 函数:
 * 描述:   wifi 回调函数
 * 参数:
 * 返回: 无
 * 备注：
 ************************************************************************/
void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_id == WIFI_EVENT_STA_START)
    {
        // // 开始连接
        //  logger_log(TAG, "esp_wifi_connect start");
    }
    else if (event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        // wifi断开时
        logger_log(TAG, "wifi connect fail %d", isReConnect);
        // WiFi连接失败
        if (isReConnect == 0)
        {
            send_xQueue(EVENT_WIFI_CONNECT_FAIL);
        }
    }
    else if (event_id == IP_EVENT_STA_GOT_IP)
    {
        // 获得IP
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        logger_log(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        configStruct config = nvs_get_config();
        // 记录WiFi账号密码
        strncpy(config.ssid, mSsid, sizeof(config.ssid));
        strncpy(config.password, mPassword, sizeof(config.password));
        // 存储起来
        // 设置成1，重启直接进入程序
        config.reset = 1;
        ESP_ERROR_CHECK(nvs_save_config(config));
        send_xQueue(EVENT_WIFI_CONNECTFINISH);
    }
    else if (event_id == WIFI_EVENT_STA_CONNECTED)
    {
        logger_log(TAG, "WiFi is connected");
    }
    else if (event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        logger_log(TAG, "WiFi is disconnected");
    }
    else if (event_id == WIFI_EVENT_SCAN_DONE)
    {
        // 获取扫描结果

        uint16_t number = 20;
        wifi_ap_record_t ap_info[20];

        uint16_t ap_count = 0;
        esp_wifi_scan_get_ap_num(&ap_count);
        esp_wifi_scan_get_ap_records(&number, ap_info);

        // 打印扫描结果
        // for (int i = 0; i < ap_count; i++)
        // {
        //     printf("SSID: %s\n", ap_info[i].ssid);
        //     printf("BSSID: " MACSTR "\n", MAC2STR(ap_info[i].bssid));
        //     printf("RSSI: %d\n", ap_info[i].rssi);
        //     printf("\n");
        // }
        // 停止WiFi扫描
        esp_wifi_scan_stop();
        // 把SSID拼接成字符串
        memset(wifiScanResult, 0, sizeof(wifiScanResult));
        for (int i = 0; i < ap_count; i++)
        {
            int len = strlen(wifiScanResult);
            snprintf(wifiScanResult + len, sizeof(wifiScanResult) - len, "%s%s", (char *)ap_info[i].ssid, i < ap_count - 1 ? "\n" : "");
        }

        // 发送扫描结束的事件
        send_xQueue(UI_WIFI_SCAN_RESULT);
    }
}

/**
 * @brief 初始化WiFi
 */
void wifi_init()
{
    // 初始化WiFi
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    // 清除保存的 SSID 和密码
    // esp_wifi_restore();
    // 注册wifi 连接过程中回调函数
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));
    // 设置WiFi模式为STA（Station）
    esp_wifi_set_mode(WIFI_MODE_STA);
    // 开始WiFi扫描
    esp_wifi_start();
}

/**
 * @brief 扫描WiFi
 * @param goView 是否进入WiFi选择页面
 */
void wifi_scan()
{
    // 清除AP列表
    esp_wifi_clear_ap_list();
    esp_wifi_scan_start(NULL, false);
}

/***********************************************************************
 * 函数:
 * 描述:   wifi sta初始化代码相对比较固定，一般不需要做修改
 * 参数:
 * 返回: 无
 * 备注：
 ************************************************************************/
esp_err_t wifi_init_sta(uint8_t mIsReConnect)
{
    isReConnect = mIsReConnect;
    if (isReConnect == 1)
    {
        // 1. 停止当前连接
        esp_err_t err = esp_wifi_disconnect();
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "esp_wifi_disconnect failed: %s", esp_err_to_name(err));
        }
        // 设置账户和密码,在menuconfig中设置
        wifi_config_t wifi_config = {0};
        // 2. 清除旧配置
        memset(&wifi_config, 0, sizeof(wifi_config));
        // 初始化结构体，将所有成员设置为0
        strncpy((char *)wifi_config.sta.ssid, mSsid, sizeof(wifi_config.sta.ssid));             // 设置SSID
        strncpy((char *)wifi_config.sta.password, mPassword, sizeof(wifi_config.sta.password)); // 设置密码
                                                                                                // 3. 设置新配置
        esp_err_t error = esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
        if (error != ESP_OK)
        {
            logger_log(TAG, "esp_wifi_set_config error = %s\n", esp_err_to_name(error));
        }
    }
    else
    {
        // 获取保存的WiFi账号密码
        wifi_config_t wifi_config = {0};
         // 获取wifi账号密码进行连接
        configStruct config = nvs_get_config();
        // 初始化结构体，将所有成员设置为0
        strcpy((char *)wifi_config.sta.ssid, config.ssid);         // 设置SSID
        strcpy((char *)wifi_config.sta.password, config.password); // 设置密码
        esp_wifi_get_config(ESP_IF_WIFI_STA, &wifi_config);
        // 将 SSID 和密码复制到 mSsid 和 mPassword
        strncpy(mSsid, (const char *)wifi_config.sta.ssid, sizeof(mSsid) - 1);
        mSsid[sizeof(mSsid) - 1] = '\0';
        strncpy(mPassword, (const char *)wifi_config.sta.password, sizeof(mPassword) - 1);
        mPassword[sizeof(mPassword) - 1] = '\0';
    }
    logger_log(TAG, "ssid = %s", mSsid);
    logger_log(TAG, "password = %s", mPassword);
    // 如果账号密码为空，则进入WiFi选择页面
    if (strlen(mSsid) == 0 || strlen(mPassword) == 0)
    {
        send_xQueue(EVENT_WIFI_CONNECT_FAIL);
        return ESP_OK;
    }
    // 4. 重新连接
    ESP_ERROR_CHECK(esp_wifi_connect());
    logger_log(TAG, "wifi_init_sta finished.");
    return ESP_OK;
}

/**
 * @brief 保存WiFi配置
 */
esp_err_t saveWiFiConfigSsid(char *ssid)
{
    strncpy(mSsid, ssid, sizeof(mSsid));
    return ESP_OK;
}

/**
 * @brief 保存WiFi密码
 */
esp_err_t saveWiFiConfigPassWord(char *password)
{
    strncpy(mPassword, password, sizeof(mPassword));
    return ESP_OK;
}
