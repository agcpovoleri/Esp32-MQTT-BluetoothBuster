#include "header.h"

EventGroupHandle_t esp32_event_group = NULL;

const int WIFI_CONNECTED_BIT = BIT0;
const int MQTT_CONNECTED_BIT = BIT1;
const int MQTT_PUBLISHED_BIT = BIT2;
const int BLE_SCANNED_BIT = BIT3;

char sensor_data[1024];

const char* DEMO_TAG = "IBEACON_DEMO";

esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
        case SYSTEM_EVENT_STA_START:
            esp_wifi_connect();
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            xEventGroupSetBits(esp32_event_group, WIFI_CONNECTED_BIT);
            gb_mqttClient = mqtt_start(&settings);
            //init app here
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            // This is a workaround as ESP32 WiFi libs don't currently auto-reassociate.
            esp_wifi_connect();
            mqtt_stop();
            gb_mqttClient = NULL;
            xEventGroupClearBits(esp32_event_group, WIFI_CONNECTED_BIT);
            break;
        default:
            break;
    }
    return ESP_OK;
}

void GpioInit()
{
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = 1 << PIN_LED;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
}

void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_bt_controller_init(&bt_cfg);
    esp_bt_controller_enable(ESP_BT_MODE_BLE);

    ble_ibeacon_init();
    /* set scan parameters */
#if (IBEACON_MODE == IBEACON_RECEIVER)
    esp_ble_gap_set_scan_params(&ble_scan_params);

#elif (IBEACON_MODE == IBEACON_SENDER)
    esp_ble_ibeacon_t ibeacon_adv_data;
    esp_err_t status = esp_ble_config_ibeacon_data (&vendor_config, &ibeacon_adv_data);
    if (status == ESP_OK){
        esp_ble_gap_config_adv_data_raw((uint8_t*)&ibeacon_adv_data, sizeof(ibeacon_adv_data));
    }
    else {
        ESP_LOGE(DEMO_TAG, "Config iBeacon data failed, status =0x%x\n", status);
    }
#endif

    wifi_conn_init();

    //GpioInit();
    
    /*
    * Start thead to manage Beacon Search
    * BleSearch_Task sets BLE_SCANNED_BIT which enables the publish act
    */
	xTaskCreate(&BleSearch_Task, "BleSearch_Task", 2048, NULL,  8, NULL);

    while(1)
    {
        //xEventGroupWaitBits(esp32_event_group, MQTT_INITIATE_PUBLISH_BIT, true, true, portMAX_DELAY);
		//ESP_LOGI(DEMO_TAG, "Flag MQTT_INITIATE_PUBLISH_BIT - OK");
        xEventGroupWaitBits(esp32_event_group, WIFI_CONNECTED_BIT, true, true, portMAX_DELAY);
		xEventGroupWaitBits(esp32_event_group, MQTT_CONNECTED_BIT, true, true, portMAX_DELAY);
		
		xEventGroupWaitBits(esp32_event_group, BLE_SCANNED_BIT, true, true, portMAX_DELAY);
		xEventGroupWaitBits(esp32_event_group, MQTT_PUBLISHED_BIT, true, true, portMAX_DELAY);
				
		ESP_LOGI(DEMO_TAG, "Everything is fine. ");
		//xTaskCreatePinnedToCore(&bt_task, "btTask", 2048, NULL, 5, NULL, 0);
    }
}

