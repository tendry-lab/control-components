idf_component_register(
    SRCS
    "async_task.cpp"
    "async_task_scheduler.cpp"
    "fanout_task.cpp"
    "async_func_scheduler.cpp"
    "async_func.cpp"
    "periodic_task_scheduler.cpp"
    "constant_delay_estimator.cpp"
    "func_task.cpp"
    "async_isr_task.cpp"

    REQUIRES
    "freertos"

    "ocs_core"
    "ocs_system"

    INCLUDE_DIRS
    ".."
)
