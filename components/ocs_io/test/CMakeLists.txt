idf_component_register(
    SRCS
    "gpio/test_gpio_guard.cpp"

    "adc/target_esp32/test_oneshot_store.cpp"

    REQUIRES
    "unity"
    "ocs_io"
    "ocs_test"
)
