idf_component_register(
    SRCS
    "storage_builder.cpp"

    "target_esp32/flash_initializer.cpp"
    "target_esp32/nvs_storage.cpp"

    REQUIRES
    "nvs_flash"

    INCLUDE_DIRS
    ".."
)
