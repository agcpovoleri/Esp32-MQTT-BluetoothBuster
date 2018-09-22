#ifndef __BLEUTILS_H__
#define __BLEUTILS_H__

#include "header.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"
#include "esp_ibeacon_api.h"
#include "esp_log.h"

void ble_ibeacon_init(void);
void ble_ibeacon_appRegister(void);

extern esp_ble_ibeacon_vendor_t vendor_config;
#if (IBEACON_MODE == IBEACON_RECEIVER)
extern esp_ble_scan_params_t ble_scan_params;
#elif (IBEACON_MODE == IBEACON_SENDER)
extern esp_ble_adv_params_t ble_adv_params;
#endif

// Prototypes
extern char *bt_event_type_to_string(uint32_t eventType);
extern char *bt_gap_search_event_type_to_string(uint32_t searchEvt);
extern char *bt_addr_t_to_string(esp_ble_addr_type_t type);
extern char *bt_dev_type_to_string(esp_bt_dev_type_t type);

extern char *btsig_gap_type(uint32_t gap_type);

#endif //__BLEUTILS_H__