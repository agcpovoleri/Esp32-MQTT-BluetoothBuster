#ifndef __MQTTUTILS_H__
#define __MQTTUTILS_H__

#include "header.h"

void connected_cb(void *self, void *params);
void send_beacon_data(void *self, void *params);
void disconnected_cb(void *self, void *params);
void reconnect_cb(void *self, void *params);
void subscribe_cb(void *self, void *params);
void publish_cb(void *self, void *params);
void data_cb(void *self, void *params);
void wifi_conn_init(void);

bool available_sensor_data();
void publish_sensor_data(void *params);
void mqtt_publish_health_check();
void mqtt_publish_presence_identified();

extern mqtt_client * gb_mqttClient;
extern mqtt_settings settings;

#endif //__MQTTUTILS_H__