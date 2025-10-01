idf_component_register(
    SRCS
    "test_fanout_suspender.cpp"

    "target_esp32/test_rt_delayer.cpp"
    "target_esp32/test_high_resolution_timer.cpp"

    REQUIRES
    "unity"
    "ocs_system"
)
