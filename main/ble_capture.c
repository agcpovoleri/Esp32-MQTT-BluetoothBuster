#include "header.h"

const char* BLE_CAPTURE_TAG = "BLECAPTURE_DEMO";
char sensor_data[1024];
void BleSearchInit()
{
    
}

void BleSearch_Task()
{
    BleSearchInit();
    while(1)
    {
		
		//ESP_LOGI(BLE_CAPTURE_TAG, "Sending data to MQTT : %s", BLE_CAPTURE_TAG);
		if (BLE_CAPTURE_TAG != NULL){// && available_sensor_data()) {

			//publish_sensor_data("BLE_CAPTURE_TAG");
		}					
			
		//vTaskDelay(2000/portTICK_RATE_MS);
		xEventGroupSetBits(esp32_event_group, MQTT_PUBLISHED_BIT);
    }
}




