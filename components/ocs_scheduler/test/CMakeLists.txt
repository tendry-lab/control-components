idf_component_register(
    SRCS
    "test_fanout_task.cpp"
    "test_async_task.cpp"
    "test_async_task_scheduler.cpp"
    "test_async_func_scheduler.cpp"
    "test_periodic_task_scheduler.cpp"

    "target_esp32/test_high_resolution_timer.cpp"

    REQUIRES
    "unity"
    "ocs_scheduler"
)
