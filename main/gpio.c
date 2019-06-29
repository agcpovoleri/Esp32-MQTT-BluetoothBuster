#include "gpio.h"

const char *GPIO_SENSOR_TAG = "GPIO_SENSOR_TAG";

void gpio_task_example(void* arg)
{
    uint32_t io_num;
    while(1) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            //printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
			
			mqtt_publish_presence_identified();
			ESP_LOGI(GPIO_SENSOR_TAG, "Moviment detect!");			
						
			xEventGroupSetBits(esp32_event_group, MQTT_INITIATE_PUBLISH_BIT);
        }
    }
}
