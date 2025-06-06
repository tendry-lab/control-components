idf_component_register(
    SRCS
    "httpserver/http_pipeline.cpp"
    "httpserver/ds18b20_handler.cpp"
    "httpserver/sht41_handler.cpp"
    "httpserver/data_handler.cpp"
    "httpserver/system_handler.cpp"
    "httpserver/system_state_handler.cpp"
    "httpserver/mdns_handler.cpp"
    "httpserver/web_gui_pipeline.cpp"
    "httpserver/time_handler.cpp"
    "httpserver/time_pipeline.cpp"
    "httpserver/ap_network_handler.cpp"
    "httpserver/sta_network_handler.cpp"
    "httpserver/analog_config_store_handler.cpp"

    "jsonfmt/data_pipeline.cpp"
    "jsonfmt/ap_network_formatter.cpp"
    "jsonfmt/sta_network_formatter.cpp"
    "jsonfmt/system_formatter.cpp"
    "jsonfmt/counter_formatter.cpp"
    "jsonfmt/telemetry_formatter.cpp"
    "jsonfmt/registration_formatter.cpp"
    "jsonfmt/version_formatter.cpp"
    "jsonfmt/system_state_formatter.cpp"
    "jsonfmt/sht41_sensor_formatter.cpp"
    "jsonfmt/bme280_sensor_formatter.cpp"
    "jsonfmt/ds18b20_sensor_formatter.cpp"
    "jsonfmt/ldr_analog_sensor_formatter.cpp"
    "jsonfmt/soil_analog_sensor_formatter.cpp"

    "basic/system_pipeline.cpp"
    "basic/system_counter_pipeline.cpp"
    "basic/select_network_pipeline.cpp"

    REQUIRES
    "spiffs"

    "ocs_io"
    "ocs_net"
    "ocs_http"
    "ocs_core"
    "ocs_status"
    "ocs_sensor"
    "ocs_onewire"
    "ocs_diagnostic"
    "ocs_storage"
    "ocs_fmt"
    "ocs_control"
    "ocs_algo"

    INCLUDE_DIRS
    ".."
)
