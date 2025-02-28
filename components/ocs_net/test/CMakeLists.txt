idf_component_register(
    SRCS
    "test_ip_addr_to_str.cpp"
    "test_rssi_to_signal_strength.cpp"

    "target_esp32/test_sta_network.cpp"
    "target_esp32/test_default_mdns_server.cpp"

    REQUIRES
    "unity"
    "ocs_net"
    "ocs_storage"
)
