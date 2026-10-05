idf_component_register(
    SRCS
    "test_async_storage.cpp"

    "target_esp32/test_flash_initializer.cpp"
    "target_esp32/test_nvs_storage.cpp"

    REQUIRES
    "unity"
    "ocs_storage"
    "ocs_scheduler"
    "ocs_system"
    "ocs_test"
)
