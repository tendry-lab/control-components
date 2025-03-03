idf_component_register(
    SRCS
    "gpio/default_gpio.cpp"
    "gpio/delay_gpio.cpp"
    "gpio/gpio_guard.cpp"
    "gpio/target_esp32/basic_gpio.cpp"

    "adc/target_esp32/oneshot_reader.cpp"
    "adc/target_esp32/oneshot_store.cpp"
    "adc/target_esp32/basic_converter.cpp"
    "adc/target_esp32/line_fitting_converter.cpp"

    "spi/target_esp32/master_store.cpp"
    "spi/target_esp32/master_transceiver.cpp"

    "i2c/target_esp32/master_store.cpp"
    "i2c/target_esp32/master_transceiver.cpp"
    "i2c/target_esp32/master_store_pipeline.cpp"

    REQUIRES
    "driver"
    "esp_adc"
    "ocs_scheduler"
    "ocs_core"
    "ocs_status"

    INCLUDE_DIRS
    ".."
)
