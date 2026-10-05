idf_component_register(
    SRCS
    "async_storage.cpp"

    "target_esp32/flash_initializer.cpp"
    "target_esp32/nvs_storage.cpp"
    "target_esp32/spiffs_initializer.cpp"

    REQUIRES
    "nvs_flash"
    "spiffs"

    "ocs_scheduler"
    "ocs_core"
    "ocs_system"

    INCLUDE_DIRS
    ".."
)
