idf_component_register(
    SRCS
    "basic_counter.cpp"
    "time_counter.cpp"
    "counter_store.cpp"
    "basic_persistent_counter.cpp"
    "mem_persistent_counter.cpp"
    "acc_persistent_counter.cpp"

    REQUIRES
    "ocs_core"
    "ocs_system"
    "ocs_status"
    "ocs_diagnostic"

    INCLUDE_DIRS
    ".."
)
