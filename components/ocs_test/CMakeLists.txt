idf_component_register(
    SRCS
    "test_clock.cpp"
    "test_counter.cpp"
    "test_task.cpp"
    "test_timer.cpp"
    "test_gpio.cpp"
    "memory_storage.cpp"
    "status_storage.cpp"

    REQUIRES
    "unity"

    "ocs_io"
    "ocs_core"
    "ocs_status"
    "ocs_storage"
    "ocs_diagnostic"
    "ocs_scheduler"

    INCLUDE_DIRS
    ".."
)
