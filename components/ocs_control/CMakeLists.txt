idf_component_register(
    SRCS
    "fsm_store.cpp"
    "fsm_block.cpp"
    "fsm_block_pipeline.cpp"
    "basic_led.cpp"
    "gpio_led.cpp"
    "led_locator.cpp"
    "gpio_button.cpp"
    "button_event_handler.cpp"
    "system_fsm.cpp"
    "flip_led_task.cpp"
    "config_fsr_handler.cpp"

    REQUIRES
    "freertos"
    "ocs_core"
    "ocs_scheduler"
    "ocs_storage"

    INCLUDE_DIRS
    ".."
)
