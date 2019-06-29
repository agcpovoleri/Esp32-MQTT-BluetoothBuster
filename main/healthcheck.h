#ifndef __HEALTHCHECK_H__
#define __HEALTHCHECK_H__

#include "header.h"

#define BLINK_GPIO 2
//#define BLINK_GPIO GPIO2

void health_task(void *pvParameter);
void blinkLed();

#endif //__HEALTHCHECK_H__