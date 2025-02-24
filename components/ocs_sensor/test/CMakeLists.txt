idf_component_register(
    SRCS
    "test_analog_config.cpp"

    "ds18b20/test_store.cpp"
    "ds18b20/test_parse_configuration.cpp"

    REQUIRES
    "unity"
    "ocs_test"
    "ocs_sensor"
)
