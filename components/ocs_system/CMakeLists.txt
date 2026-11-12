idf_component_register(
    SRCS
    "fanout_reboot_handler.cpp"
    "task_state_to_str.cpp"
    "fanout_suspender.cpp"
    "suspender_guard.cpp"
    "device_info.cpp"
    "crc32_blob_writer.cpp"
    "log_blob_writer.cpp"
    "async_rebooter.cpp"
    "heap_arena.cpp"
    "guard_arena.cpp"
    "func_rebooter.cpp"
    "freertos_timer.cpp"
    "freertos_timer_builder.cpp"

    "target_esp32/clock.cpp"
    "target_esp32/device_id.cpp"
    "target_esp32/rebooter.cpp"
    "target_esp32/reset_reason_to_str.cpp"
    "target_esp32/rt_delayer.cpp"
    "target_esp32/randomizer.cpp"
    "target_esp32/high_resolution_timer.cpp"
    "target_esp32/high_resolution_timer_builder.cpp"
    "target_esp32/le_crc32_calculator.cpp"
    "target_esp32/ota_blob_writer.cpp"
    "target_esp32/partition_blob_writer.cpp"
    "target_esp32/cache_blob_writer.cpp"
    "target_esp32/spiffs_blob_writer.cpp"
    "target_esp32/update_committer.cpp"
    "target_esp32/heap_caps_arena.cpp"

    REQUIRES
    "esp_timer"
    "app_update"

    "ocs_security"

    INCLUDE_DIRS
    ".."
)
