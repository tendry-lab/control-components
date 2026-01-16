idf_component_register(
    SRCS
    "fanout_reboot_handler.cpp"
    "delay_rebooter.cpp"
    "reboot_task.cpp"
    "task_state_to_str.cpp"
    "system_state_builder.cpp"
    "fanout_suspender.cpp"
    "suspender_guard.cpp"
    "device_id.cpp"
    "device_info.cpp"
    "crc32_updater.cpp"
    "log_updater.cpp"

    "target_esp32/clock.cpp"
    "target_esp32/rebooter.cpp"
    "target_esp32/reset_reason_to_str.cpp"
    "target_esp32/rt_delayer.cpp"
    "target_esp32/platform_builder.cpp"
    "target_esp32/randomizer.cpp"
    "target_esp32/high_resolution_timer.cpp"
    "target_esp32/le_crc32_calculator.cpp"
    "target_esp32/native_updater.cpp"
    "target_esp32/partition_updater.cpp"
    "target_esp32/cache_updater.cpp"
    "target_esp32/spiffs_updater.cpp"
    "target_esp32/update_committer.cpp"

    REQUIRES
    "esp_timer"
    "app_update"

    "ocs_security"

    INCLUDE_DIRS
    ".."
)
