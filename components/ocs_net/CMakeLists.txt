idf_component_register(
    SRCS
    "sta_network.cpp"
    "ap_network.cpp"
    "mdns_to_str.cpp"
    "mdns_service.cpp"
    "basic_mdns_server.cpp"
    "default_mdns_server.cpp"
    "netif_builder.cpp"
    "ip_addr_to_str.cpp"
    "rssi_to_signal_strength.cpp"
    "fanout_network_handler.cpp"
    "wait_network_runner.cpp"
    "ap_network_config.cpp"

    REQUIRES
    "esp_netif"
    "esp_wifi"
    "mdns"
    "ocs_core"
    "ocs_system"

    INCLUDE_DIRS
    ".."
)
