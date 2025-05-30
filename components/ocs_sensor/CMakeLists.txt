idf_component_register(
    SRCS

    "analog_config.cpp"
    "analog_config_store.cpp"
    "analog_sample_reader.cpp"

    "soil/analog_sensor.cpp"
    "soil/analog_relay_sensor.cpp"
    "soil/analog_sensor_pipeline.cpp"
    "soil/analog_relay_sensor_pipeline.cpp"
    "soil/soil_status_to_str.cpp"

    "ldr/analog_sensor.cpp"
    "ldr/analog_sensor_pipeline.cpp"

    "ds18b20/store.cpp"
    "ds18b20/sensor.cpp"
    "ds18b20/scratchpad.cpp"
    "ds18b20/sensor_pipeline.cpp"
    "ds18b20/resolution_to_str.cpp"
    "ds18b20/parse_configuration.cpp"

    "sht41/sensor.cpp"
    "sht41/sensor_pipeline.cpp"
    "sht41/serial_number_to_str.cpp"

    "bme280/sensor.cpp"
    "bme280/spi_sensor_pipeline.cpp"
    "bme280/spi_transceiver.cpp"

    REQUIRES
    "ocs_io"
    "ocs_core"
    "ocs_status"
    "ocs_system"
    "ocs_storage"
    "ocs_scheduler"
    "ocs_diagnostic"
    "ocs_onewire"
    "ocs_algo"

    INCLUDE_DIRS
    ".."
)
