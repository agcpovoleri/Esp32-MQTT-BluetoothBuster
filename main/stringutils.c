#include "stringutils.h"

//char

bool DecodePublishResponse(char *topic, char *data)
{
    ESP_LOGI("MQTT_SUB", "topic: %s", topic);
	if(strstr(topic, "sensors/"SENSOR_ID"/configuration") != NULL) 
	{
	//if (data == healthCheck) -> retrieve healh check 
	//if (data == asleep) -> stop sending health check and monitor presence
	//if (data == awake) -> start sending health check and monitor presence
	//if (data == search) -> search for beacons
		ESP_LOGI("MQTT_SUB", "Data: %s", data);
		if(strstr(data, "command") != NULL)
		{
			if(strstr(data, "healthCheck"))
			{
				ESP_LOGI("MQTT_SUBSCRIBE", "switch = false");
				LED_OFF;
			}
			else if(strstr(data, "asleep"))
			{
				ESP_LOGI("MQTT_SUBSCRIBE", "switch = false");
				LED_OFF;
			}
			else if(strstr(data, "awake"))
			{
				ESP_LOGI("MQTT_SUBSCRIBE", "switch = true");
				LED_ON;
			}
			return true;
		}
	}
    return false;
}