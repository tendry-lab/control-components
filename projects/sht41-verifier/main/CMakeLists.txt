idf_component_register(
    SRCS
    "main.cpp"

    REQUIRES
    "freertos"
    "ocs_sensor"
    "ocs_system"

    INCLUDE_DIRS
    "."
)
