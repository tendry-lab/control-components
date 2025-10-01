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

    "target_esp32/default_clock.cpp"
    "target_esp32/rebooter.cpp"
    "target_esp32/reset_reason_to_str.cpp"
    "target_esp32/rt_delayer.cpp"
    "target_esp32/platform_builder.cpp"
    "target_esp32/randomizer.cpp"
    "target_esp32/high_resolution_timer.cpp"

    REQUIRES
    "esp_timer"
    "driver"
    "ocs_security"

    INCLUDE_DIRS
    ".."
)
