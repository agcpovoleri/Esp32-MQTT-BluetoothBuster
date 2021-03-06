/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "healthcheck.h"

void health_task(void *pvParameter)
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
	
	while(1) {
        
		mqtt_publish_health_check();
		blinkLed();
        vTaskDelay(30000 / portTICK_PERIOD_MS);
    }
}

void blinkLed(){
	gpio_set_level(BLINK_GPIO, 0);
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	gpio_set_level(BLINK_GPIO, 1);
}
