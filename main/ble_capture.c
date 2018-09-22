#include "ble_capture.h"

const char* BLE_CAPTURE_TAG = "BLECAPTURE_DEMO";

void BleSearchInit()
{
    
}

void BleSearch_Task()
{
    BleSearchInit();
    while(1)
    {
		ESP_LOGI(BLE_CAPTURE_TAG, "Sending data to MQTT : %s", sensor_data);
		if (sensor_data != NULL) {
			publish_sensor_data(gb_mqttClient, sensor_data);
		}
		xEventGroupSetBits(esp32_event_group, MQTT_PUBLISHED_BIT);
        vTaskDelay(5000/portTICK_RATE_MS);
    }
}
