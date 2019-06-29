#include "adcutils.h"

uint32_t gb_voltage = 0;
static esp_adc_cal_characteristics_t characteristics;

void AdcInit()
{
    //Init ADC and Characteristics
//    adc_gpio_init(ADC_UNIT_1, POT_ADC_CHANNEL);
//    adc1_config_width(ADC_WIDTH_BIT_9);
//    adc1_config_channel_atten(POT_ADC_CHANNEL, ADC_ATTEN_DB_11);
//    esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_9, &characteristics);
}

void GetAdcValue_Task()
{
    uint32_t previousVoltage = 0;
    int16_t percentageChange = 0;
    AdcInit();
    while(1)
    {
        /*gb_voltage = adc1_to_voltage(POT_ADC_CHANNEL, &characteristics);
        if(gb_voltage == 0)
        {
            if(previousVoltage != gb_voltage)
                percentageChange = 1;
            else
                percentageChange = 0;
        }
        else
            percentageChange = (gb_voltage - previousVoltage) * 100 / gb_voltage;
        if(percentageChange >= ADC_ACCURACY || percentageChange <= -ADC_ACCURACY)
        {
            ESP_LOGI("GetAdcValue_Task", "%dmV changed to %dmV\n",previousVoltage, gb_voltage);
            previousVoltage = gb_voltage;
            xEventGroupSetBits(esp32_event_group, MQTT_INITIATE_PUBLISH_BIT);
        }*/
        //printf("prev:%dmV %dmV %d%%\n",previousVoltage, voltage, percentageChange);
        vTaskDelay(300/portTICK_RATE_MS);
    }
}