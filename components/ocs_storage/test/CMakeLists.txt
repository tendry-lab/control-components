idf_component_register(
    SRCS
    "target_esp32/test_storage_builder.cpp"
    "target_esp32/test_flash_initializer.cpp"
    "target_esp32/test_nvs_storage.cpp"

    REQUIRES
    "unity"
    "ocs_storage"
)
