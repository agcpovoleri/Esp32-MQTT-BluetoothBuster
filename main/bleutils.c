#include "bleutils.h"

const char *BLE_UTILS_TAG = "BLE_UTILS";

#define BT_BD_ADDR_STR         "%02x:%02x:%02x:%02x:%02x:%02x"
#define BT_BD_ADDR_HEX(addr)   addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]

#define tag "ble1"

#if (IBEACON_MODE == IBEACON_RECEIVER)
esp_ble_scan_params_t ble_scan_params = {
    .scan_type              = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type          = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy     = BLE_SCAN_FILTER_ALLOW_ALL,
    .scan_interval          = 0x50,
    .scan_window            = 0x30
};

#elif (IBEACON_MODE == IBEACON_SENDER)
esp_ble_adv_params_t ble_adv_params = {
    .adv_int_min        = 0x20,
    .adv_int_max        = 0x40,
    .adv_type           = ADV_TYPE_NONCONN_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};
#endif

extern esp_ble_ibeacon_vendor_t vendor_config;
void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
    case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:{
#if (IBEACON_MODE == IBEACON_SENDER)
        esp_ble_gap_start_advertising(&ble_adv_params);
#endif
        break;
    }
    case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT: {
#if (IBEACON_MODE == IBEACON_RECEIVER)
        //the unit of theclTabCtrl duration is second, 0 means scan permanently
        uint32_t duration = 0;
        esp_ble_gap_start_scanning(duration);
#endif
        break;
    }
    case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT:
        //scan start complete event to indicate scan start successfully or failed
        if (param->scan_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(BLE_UTILS_TAG, "Scan start failed");
        }
        break;
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        //adv start complete event to indicate adv start successfully or failed
        if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(BLE_UTILS_TAG, "Adv start failed");
        }
        break;
    case ESP_GAP_BLE_SCAN_RESULT_EVT: {
        esp_ble_gap_cb_param_t *scan_result = (esp_ble_gap_cb_param_t *)param;
        switch (scan_result->scan_rst.search_evt) {
        case ESP_GAP_SEARCH_INQ_RES_EVT:
            /* Search for BLE iBeacon Packet */
            if (esp_ble_is_ibeacon_packet(scan_result->scan_rst.ble_adv, scan_result->scan_rst.adv_data_len)){
                esp_ble_ibeacon_t *ibeacon_data = (esp_ble_ibeacon_t*)(scan_result->scan_rst.ble_adv);
                ESP_LOGI(BLE_UTILS_TAG, "----------iBeacon Found----------");
                esp_log_buffer_hex("IBEACON_DEMO: Device address:", scan_result->scan_rst.bda, BD_ADDR_LEN );
                esp_log_buffer_hex("IBEACON_DEMO: Proximity UUID:", ibeacon_data->ibeacon_vendor.proximity_uuid, ESP_UUID_LEN_128);

                //uint16_t major = ENDIAN_CHANGE_U16(ibeacon_data->ibeacon_vendor.major);
                //uint16_t minor = ENDIAN_CHANGE_U16(ibeacon_data->ibeacon_vendor.minor);
                //ESP_LOGI(BLE_UTILS_TAG, "Major: 0x%04x (%d)", major, major);
                //ESP_LOGI(BLE_UTILS_TAG, "Minor: 0x%04x (%d)", minor, minor);
                //ESP_LOGI(BLE_UTILS_TAG, "Measured power (RSSI at a 1m distance):%d dbm", ibeacon_data->ibeacon_vendor.measured_power);
                //ESP_LOGI(BLE_UTILS_TAG, "RSSI of packet:%d dbm", scan_result->scan_rst.rssi);

				//sprintf(sensor_data, "{\"uuid\":\"%s\", \"MAC\":\"%s\" ,\"measured_power\":\"%d\", \"rssi\":\"%d\"}", "test", "any", ibeacon_data->ibeacon_vendor.measured_power, scan_result->scan_rst.rssi);
				//ESP_LOGI(BLE_UTILS_TAG, "SENSOR_DATA: %s", sensor_data);
				
				ESP_LOGI(tag, "Device address (bda): %02x:%02x:%02x:%02x:%02x:%02x", BT_BD_ADDR_HEX(scan_result->scan_rst.bda));
				//ESP_LOGI(tag, "Device type         : %s", bt_dev_type_to_string(scan_result->scan_rst.dev_type));
				//ESP_LOGI(tag, "Search_evt          : %s", bt_gap_search_event_type_to_string(scan_result->scan_rst.search_evt));
				//ESP_LOGI(tag, "Addr_type           : %s", bt_addr_t_to_string(scan_result->scan_rst.ble_addr_type));
				ESP_LOGI(tag, "RSSI                : %d", scan_result->scan_rst.rssi);
				ESP_LOGI(tag, "Flag                : %d", scan_result->scan_rst.flag);
				  
				//bit 0 (OFF) LE Limited Discoverable Mode
				//bit 1 (OFF) LE General Discoverable Mode
				//bit 2 (ON) BR/EDR Not Supported
				//bit 3 (OFF) Simultaneous LE and BR/EDR to Same Device Capable (controller)
				//bit 4 (OFF) Simultaneous LE and BR/EDR to Same Device Capable (Host)
				
				//Number of tags found
				//ESP_LOGI(tag, "num_resps           : %d", scan_result->scan_rst.num_resps);
				
				sprintf(sensor_data, "{\"uuid\":\"%s\", \"MAC\":\"%02x:%02x:%02x:%02x:%02x:%02x\" ,\"measured_power\":\"%d\", \"rssi\":\"%d\"}", 
								SENSOR_ID, 
								BT_BD_ADDR_HEX(scan_result->scan_rst.bda), 
								ibeacon_data->ibeacon_vendor.measured_power, 
								scan_result->scan_rst.rssi);
				ESP_LOGI(BLE_UTILS_TAG, "SENSOR_DATA: %s", sensor_data);			
				
				xEventGroupSetBits(esp32_event_group, BLE_SCANNED_BIT);				
				//vTaskDelay(1000/portTICK_RATE_MS);
            }
            break;
        default:
            break;
        }
        break;
    }

    case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:
        if (param->scan_stop_cmpl.status != ESP_BT_STATUS_SUCCESS){
            ESP_LOGE(BLE_UTILS_TAG, "Scan stop failed");
        }
        else {
            ESP_LOGI(BLE_UTILS_TAG, "Stop scan successfully");
        }
        break;

    case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
        if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS){
            ESP_LOGE(BLE_UTILS_TAG, "Adv stop failed");
        }
        else {
            ESP_LOGI(BLE_UTILS_TAG, "Stop adv successfully");
        }
        break;

    default:
        break;
    }
}

void ble_ibeacon_appRegister(void)
{
    esp_err_t status;

    ESP_LOGI(BLE_UTILS_TAG, "register callback");

    //register the scan callback function to the gap module
    if ((status = esp_ble_gap_register_callback(esp_gap_cb)) != ESP_OK) {
        ESP_LOGE(BLE_UTILS_TAG, "gap register error, error code = %x", status);
        return;
    }

}

void ble_ibeacon_init(void)
{
    esp_bluedroid_init();
    esp_bluedroid_enable();
    ble_ibeacon_appRegister();
}



uint32_t convertU16ToU32(uint16_t inShortA, uint16_t inShortB) 
{
	return inShortA<< 16 | inShortB;
}


uint16_t convertU8ToU16(uint8_t inByteA, uint8_t inByteB)
{
	return inByteA<<8 | inByteB;
}


// https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.health_thermometer.xml
double decode1809(uint8_t  *payload)
{
	return ((float)(payload[4]<<16 | payload[3]<<8 | payload[2])) / 100.0;
}

uint8_t decode180f(uint8_t  *payload)
{
	return payload[2];
}

void dump_16bituuid(uint8_t  *payload,uint8_t length)
{
	//Find UUID from the two first bytes
	uint16_t uuid =0;

	uuid = convertU8ToU16 (payload[1],payload[0]);
	ESP_LOGD(tag,"Dump16 UUID %04X, len %d", uuid,length);


	length -=2; //Reduce with header size

	// A list of all GATT Services is here https://www.bluetooth.com/specifications/gatt/services
	switch(uuid)
	{
		
		case 0x1809:  // Health Thermometer
			if (length >=4) //Validate input payload length;
			{				
				ESP_LOGI(tag,"@ 0x1809 Temperature %f", decode1809(payload));
			}
			
			break;
		case 0x180f : //Battery Service (mandatory)
			if (length >=1)
			{ 
				ESP_LOGI(tag,"@ 0x180F Battery %d %%", decode180f(payload));
			}
			break;

		default:
			ESP_LOGI(tag,"@ 16 BIT UUID 0x%04X - Packet decoding for thtis type not implemented",uuid); // Read the Bluetooth spec and implement it
			break;
	}
}


void bin_to_strhex(unsigned char *bin, unsigned int binsz, char **result)
{
  char          hex_str[]= "0123456789abcdef";
  unsigned int  i;

  *result = (char *)malloc(binsz * 2 + 1);
  (*result)[binsz * 2] = 0;

  if (!binsz)
    return;

  for (i = 0; i < binsz; i++)
    {
      (*result)[i * 2 + 0] = hex_str[(bin[i] >> 4) & 0x0F];
      (*result)[i * 2 + 1] = hex_str[(bin[i]     ) & 0x0F];
    }  
}

uint8_t dump_adv_payload(uint8_t *payload) 
{
	uint8_t length;
	uint8_t ad_type;
	uint8_t sizeConsumed = 0;
	int finished = 0;
	uint8_t total_length=0;
	//sprintf(text, "%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x")

	// GAP assigned numbers for the Type in the payload is defined here:
	// https://www.bluetooth.com/specifications/assigned-numbers/generic-access-profile

	while(!finished) {
		length = *payload;
		payload++;
		if (length != 0) {
			ad_type = *payload;
			
			ESP_LOGI(tag, "# Payload type: 0x%.2x (%s), length: %d", ad_type, btsig_gap_type(ad_type), length);			


			// Decode packets - implemented just a few types
			switch(ad_type)
			{
				case 0x16: // 16bit UUID - Bluetooth Core Specification:Vol. 3, Part C, sections 11.1.10 and 18.10 (v4.0)
					dump_16bituuid(payload+1, length);
					break;

				case 0x09:  //Complete local name - Bluetooth Core Specification:Vol. 3, Part C, section 8.1.2 (v2.1 + EDR, 3.0 + HS and 4.0)Vol. 3, Part C, sections 11.1.2 and 18.4 (v4.0)Core Specification Supplement, Part A, section 1.2
					ESP_LOGI(tag, "# Complete local name: %.*s", length, payload);			
					break;

				default:
					break;
			}

			// Dump the raw HEX data
			// Also dump as string, if possible, to make it possible to scan for text
			int i;
			int size = length / sizeof(char);
			char *hex_str = (char*) calloc(3 * size,1);
			char *ascii_str = (char*) calloc(length+1,1); 
			char *buf_ptr1 = hex_str;
			char *buf_ptr2 = ascii_str;

			unsigned char *source = (unsigned char *)payload+1;	

			if ((hex_str) && (ascii_str)) 
			{
				for (i = 0; i < size; i++)
				{
					buf_ptr1 += sprintf(buf_ptr1, i < size - 1 ? "%02X:" : "%02X", source[i]);

					
					char ch = source[i];
					
					//quick fix since isalpha had unexpected results
					int ichar = ((int) ch) & 0xFF;
					if ((ichar<32) || (ichar>126))
					{
						ch = '.';  // unprintable characters are represented as "."
					}
					
					buf_ptr2 += sprintf(buf_ptr2, "%c", ch);					
					
				}

				ESP_LOGI(tag,"* Payload: %s (%s)", hex_str, ascii_str);
			}
			if (hex_str) free(hex_str);
			hex_str=0;

			if (ascii_str) free(ascii_str);
			ascii_str=0;

			payload += length;
			total_length += length+1;
		}	

		sizeConsumed = 1 + length;
		if (sizeConsumed >=31 || length == 0) {
			finished = 1;
		}
	} // !finished
	return total_length;
} // dump_adv_payload


char *bt_dev_type_to_string(esp_bt_dev_type_t type) {
	switch(type) {
	case ESP_BT_DEVICE_TYPE_BREDR:
		return "ESP_BT_DEVICE_TYPE_BREDR";
	case ESP_BT_DEVICE_TYPE_BLE:
		return "ESP_BT_DEVICE_TYPE_BLE";
	case ESP_BT_DEVICE_TYPE_DUMO:
		return "ESP_BT_DEVICE_TYPE_DUMO";
	default:
		return "Unknown";
	}
} // bt_dev_type_to_string

char *bt_addr_t_to_string(esp_ble_addr_type_t type) {
	switch(type) {
		case BLE_ADDR_TYPE_PUBLIC:
			return "BLE_ADDR_TYPE_PUBLIC";
		case BLE_ADDR_TYPE_RANDOM:
			return "BLE_ADDR_TYPE_RANDOM";
		case BLE_ADDR_TYPE_RPA_PUBLIC:
			return "BLE_ADDR_TYPE_RPA_PUBLIC";
		case BLE_ADDR_TYPE_RPA_RANDOM:
			return "BLE_ADDR_TYPE_RPA_RANDOM";
		default:
			return "Unknown addr_t";
	}
} // bt_addr_t_to_string

char *bt_gap_search_event_type_to_string(uint32_t searchEvt) {
	switch(searchEvt) {
		case ESP_GAP_SEARCH_INQ_RES_EVT:
			return "ESP_GAP_SEARCH_INQ_RES_EVT";
		case ESP_GAP_SEARCH_INQ_CMPL_EVT:
			return "ESP_GAP_SEARCH_INQ_CMPL_EVT";
		case ESP_GAP_SEARCH_DISC_RES_EVT:
			return "ESP_GAP_SEARCH_DISC_RES_EVT";
		case ESP_GAP_SEARCH_DISC_BLE_RES_EVT:
			return "ESP_GAP_SEARCH_DISC_BLE_RES_EVT";
		case ESP_GAP_SEARCH_DISC_CMPL_EVT:
			return "ESP_GAP_SEARCH_DISC_CMPL_EVT";
		case ESP_GAP_SEARCH_DI_DISC_CMPL_EVT:
			return "ESP_GAP_SEARCH_DI_DISC_CMPL_EVT";
		case ESP_GAP_SEARCH_SEARCH_CANCEL_CMPL_EVT:
			return "ESP_GAP_SEARCH_SEARCH_CANCEL_CMPL_EVT";
		default:
			return "Unknown event type";
	}
} // bt_gap_search_event_type_to_string

/*
 * Convert a BT GAP event type to a string representation.
 */
char *bt_event_type_to_string(uint32_t eventType) {
	switch(eventType) {
		case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
			return "ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT";
		case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
			return "ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT";
		case ESP_GAP_BLE_SCAN_RESULT_EVT:
			return "ESP_GAP_BLE_SCAN_RESULT_EVT";
		case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
			return "ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT";
		default:
			return "Unknown event type";
	}
} // bt_event_type_to_string


char *btsig_gap_type(uint32_t gap_type) {
	switch (gap_type)
	{
		case 0x01: return "Flags";
		case 0x02: return "Incomplete List of 16-bit Service Class UUIDs";
		case 0x03: return "Complete List of 16-bit Service Class UUIDs";
		case 0x04: return "Incomplete List of 32-bit Service Class UUIDs";
		case 0x05: return "Complete List of 32-bit Service Class UUIDs";
		case 0x06: return "Incomplete List of 128-bit Service Class UUIDs";
		case 0x07: return "Complete List of 128-bit Service Class UUIDs";
		case 0x08: return "Shortened Local Name";
		case 0x09: return "Complete Local Name";
		case 0x0A: return "Tx Power Level";
		case 0x0D: return "Class of Device";
		case 0x0E: return "Simple Pairing Hash C/C-192";
		case 0x0F: return "Simple Pairing Randomizer R/R-192";
		case 0x10: return "Device ID/Security Manager TK Value";
		case 0x11: return "Security Manager Out of Band Flags";
		case 0x12: return "Slave Connection Interval Range";
		case 0x14: return "List of 16-bit Service Solicitation UUIDs";
		case 0x1F: return "List of 32-bit Service Solicitation UUIDs";
		case 0x15: return "List of 128-bit Service Solicitation UUIDs";
		case 0x16: return "Service Data - 16-bit UUID";
		case 0x20: return "Service Data - 32-bit UUID";
		case 0x21: return "Service Data - 128-bit UUID";
		case 0x22: return "LE Secure Connections Confirmation Value";
		case 0x23: return "LE Secure Connections Random Value";
		case 0x24: return "URI";
		case 0x25: return "Indoor Positioning";
		case 0x26: return "Transport Discovery Data";
		case 0x17: return "Public Target Address";
		case 0x18: return "Random Target Address";
		case 0x19: return "Appearance";
		case 0x1A: return "Advertising Interval";
		case 0x1B: return "LE Bluetooth Device Address";
		case 0x1C: return "LE Role";
		case 0x1D: return "Simple Pairing Hash C-256";
		case 0x1E: return "Simple Pairing Randomizer R-256";
		case 0x3D: return "3D Information Data";
		case 0xFF: return "Manufacturer Specific Data";
		
		default: 
			return "Unknown type";
	}
}