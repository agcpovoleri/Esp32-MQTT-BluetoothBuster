deps_config := \
	/ESP32/esp-idf-v3.0.2/components/app_trace/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/aws_iot/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/bt/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/esp32/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/esp_adc_cal/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/ethernet/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/fatfs/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/freertos/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/heap/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/libsodium/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/log/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/lwip/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/mbedtls/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/openssl/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/pthread/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/spi_flash/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/spiffs/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/tcpip_adapter/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/wear_levelling/Kconfig \
	/ESP32/esp-idf-v3.0.2/components/bootloader/Kconfig.projbuild \
	/ESP32/esp-idf-v3.0.2/components/esptool_py/Kconfig.projbuild \
	/ESP32/esp32-mqtt-beaconbuster/main/Kconfig.projbuild \
	/ESP32/esp-idf-v3.0.2/components/partition_table/Kconfig.projbuild \
	/ESP32/esp-idf-v3.0.2/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
