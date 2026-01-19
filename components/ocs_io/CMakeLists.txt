set(COMPONENT_SRCS
    "gpio/target_esp32/gpio.cpp"
    "gpio/target_esp32/isr_event_service.cpp"
    "adc/target_esp32/oneshot_reader.cpp"
    "adc/target_esp32/oneshot_store.cpp"
    "adc/target_esp32/basic_converter.cpp"
    "spi/target_esp32/master_store.cpp"
    "spi/target_esp32/master_transceiver.cpp"
    "i2c/target_esp32/master_bus.cpp"
    "i2c/target_esp32/master_transceiver.cpp"
)

if(IDF_TARGET STREQUAL "esp32")
    list(APPEND COMPONENT_SRCS "adc/target_esp32/line_fitting_converter.cpp")
endif()

idf_component_register(
    SRCS ${COMPONENT_SRCS}

    REQUIRES
    "driver"
    "esp_adc"

    "ocs_scheduler"
    "ocs_core"
    "ocs_status"

    INCLUDE_DIRS ".."
)
