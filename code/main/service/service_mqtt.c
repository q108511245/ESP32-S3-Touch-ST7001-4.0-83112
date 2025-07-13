#include "service_mqtt.h"
#include "mqtt_client.h"

// 定义Mac地址存储空间
uint8_t mac_addr[6] = {0};
char uuid[50];

// mqtt句柄
esp_mqtt_client_handle_t client;

// 全局变量记录连接状态
static bool mqtt_connected = false;

/* 事件处理函数 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    // logger_log(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id = 0;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        logger_log(TAG, "MQTT_EVENT_CONNECTED");
        mqtt_connected = true;
        // mqtt连接成功，订阅状态主题
        msg_id = esp_mqtt_client_subscribe(client, SUBSCRIBE_TOPIC, 0);
        logger_log(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        mqtt_connected = false;
        logger_log(TAG, "MQTT_EVENT_DISCONNECTED");
        send_xQueue(EVENT_MQTT_DISCONNECTED);
        break;
    case MQTT_EVENT_SUBSCRIBED:
        logger_log(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        send_xQueue(EVENT_MQTT_CONNECTED);
        logger_log(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        logger_log(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        logger_log(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        logger_log(TAG, "MQTT_EVENT_DATA");
        logger_log(TAG, "TOPIC=%.*s\r\n", event->topic_len, event->topic);
        logger_log(TAG, "DATA=%.*s\r\n", event->data_len, event->data);
        logger_log(TAG, "sent publish successful, msg_id=%d", msg_id);
        cJSON *root = cJSON_Parse(event->data);
        cJSON *id = cJSON_GetObjectItem(root, "uuid");
        cJSON *order = cJSON_GetObjectItem(root, "order");
        if (id != NULL && order != NULL)
        {
            if (strcmp(id->valuestring, uuid) == 0)
            {
                // // 点亮屏幕
                // if (lvgl_screen_is_sleep())
                // {
                //     lvgl_screen_change_on();
                // }
                // // 根据order处理事件
                // if (strcmp(order->valuestring, "start") == 0)
                // {
                //     goAlarmView();
                // }
                // else
                // {
                //     goMainClockView();
                // }
            }
        }
        cJSON_Delete(root);
        break;
    case MQTT_EVENT_ERROR:
        // 重新连接
        logger_log(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
            logger_log(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        logger_log(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void MQTTInitTask(void *pvParameters)
{
    // 获取Mac地址
    esp_efuse_mac_get_default(mac_addr);
    sprintf(uuid, "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    // 配置 MQTT 客户端
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_URI,
        .credentials = {
            .client_id = MQTT_CLIENT_ID,
        },
        .session = {
            .keepalive = 40,
        },
    };
    // 创建 MQTT 客户端
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
    vTaskDelete(NULL);
}

/**
 * 关闭mqtt
 */
esp_err_t remove_mqtt()
{
    if (client != NULL)
    {
        // 1. 先断开当前连接（如果正在连接）
        esp_mqtt_client_disconnect(client);
        // 2. 停止客户端
        esp_mqtt_client_stop(client);
        // 3.完全销毁客户端
        esp_mqtt_client_destroy(client);
        logger_log(TAG, "MQTT 客户端已删除");
    }
    return ESP_OK;
}

/**
 * 初始化 MQTT 客户端
 */
esp_err_t init_mqtt()
{

    BaseType_t baseType = xTaskCreate(MQTTInitTask, "MQTTInitTask", 1024 * 4, NULL, 1, NULL);
    if (baseType != pdPASS)
    {
        logger_log(TAG, "MQTTInitTask create failed!");
        return ESP_FAIL;
    }
    else
    {
        return ESP_OK;
    }
}
