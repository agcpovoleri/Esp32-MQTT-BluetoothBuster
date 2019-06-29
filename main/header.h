#ifndef __HEADER_H__
#define __HEADER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

#include "esp_wifi.h"
#include "esp_system.h"
#include "controller.h"
#include "nvs_flash.h"
#include "esp_event_loop.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt.h"

#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "adcutils.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "stringutils.h"
#include "gpio.h"
#include "mqttutils.h"
#include "bleutils.h"
#include "ble_capture.h"
#include "healthcheck.h"


#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"
#include "esp_ibeacon_api.h"


#define WIFI_SSID                   "agcpNet2.4G"
#define WIFI_PASSWORD               "hasTVu$s30s@2"
#define SOFTAP_SSID                 "ESP32AP"
#define SOFTAP_PASSWORD             "abcdefpassword"

//#define WEB_SERVER                  "192.168.0.8"

//rasp
#define WEB_SERVER                  "192.168.0.100"
//iot.eclipse:
//#define WEB_SERVER                  "198.41.30.241"
#define WEB_PORT                    1883                                // should be an integer and not a string

#define SENSOR_ID                   "e6f690daf8e9"
#define SENSOR_KEY                  "2d9a97f168db68ae6f94b1b547581c1f"

#define PIN_LED                     GPIO_NUM_22
#define POT_ADC_CHANNEL             (ADC1_CHANNEL_6)      //GPIO 34

#define LED_ON                      gpio_set_level(PIN_LED, 0)
#define LED_OFF                     gpio_set_level(PIN_LED, 1)

#define ADC_ACCURACY                4                                  //in percentage

extern EventGroupHandle_t esp32_event_group;
extern const int WIFI_CONNECTED_BIT;
extern const int MQTT_INITIATE_PUBLISH_BIT;
extern const int MQTT_CONNECTED_BIT;
extern const int MQTT_PUBLISHED_BIT;
extern const int BLE_SCANNED_BIT;

extern char sensor_data[2048];

esp_err_t wifi_event_handler(void *ctx, system_event_t *event);

extern xQueueHandle gpio_evt_queue;

#endif //__HEADER_H__