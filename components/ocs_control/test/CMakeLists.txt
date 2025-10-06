idf_component_register(
    SRCS
    "test_fsm_block.cpp"
    "test_fsm_store.cpp"
    "test_fsm_block_storage.cpp"
    "test_gpio_led.cpp"
    "test_led_locator.cpp"
    "test_button_event_handler.cpp"
    "test_reset_fsm.cpp"

    REQUIRES
    "unity"
    "ocs_control"
)
