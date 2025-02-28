idf_component_register(
    SRCS
    "test_fanout_suspender.cpp"

    "target_esp32/test_default_delayer.cpp"
    "target_esp32/test_busy_loop_delayer.cpp"
    "target_esp32/test_low_power_delayer.cpp"

    REQUIRES
    "unity"
    "ocs_system"
)
