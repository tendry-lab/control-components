idf_component_register(
    SRCS
    "fanout_reboot_handler.cpp"
    "delay_rebooter.cpp"
    "reboot_task.cpp"
    "task_state_to_str.cpp"
    "system_state_builder.cpp"
    "delayer_configuration.cpp"
    "fanout_suspender.cpp"
    "suspender_guard.cpp"
    "device_id.cpp"
    "device_info.cpp"

    "target_esp32/default_clock.cpp"
    "target_esp32/default_rebooter.cpp"
    "target_esp32/reset_reason_to_str.cpp"
    "target_esp32/basic_delayer.cpp"
    "target_esp32/default_delayer.cpp"
    "target_esp32/busy_loop_delayer.cpp"
    "target_esp32/low_power_delayer.cpp"
    "target_esp32/default_randomizer.cpp"

    REQUIRES
    "esp_timer"
    "driver"
    "ocs_security"

    INCLUDE_DIRS
    ".."
)
