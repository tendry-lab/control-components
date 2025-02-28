idf_component_register(
    SRCS
    "mdns_to_str.cpp"
    "mdns_service.cpp"
    "basic_mdns_server.cpp"
    "ip_addr_to_str.cpp"
    "rssi_to_signal_strength.cpp"
    "fanout_network_handler.cpp"
    "wait_network_runner.cpp"
    "reset_network_runner.cpp"
    "ap_network_config.cpp"
    "sta_network_config.cpp"
    "mdns_config.cpp"

    "target_esp32/sta_network.cpp"
    "target_esp32/ap_network.cpp"
    "target_esp32/default_mdns_server.cpp"
    "target_esp32/netif_builder.cpp"

    REQUIRES
    "esp_netif"
    "esp_wifi"
    "mdns"
    "ocs_core"
    "ocs_system"

    INCLUDE_DIRS
    ".."
)
