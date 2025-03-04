idf_component_register(
    SRCS
    "chunk_stream_writer.cpp"
    "status_code_to_str.cpp"
    "router.cpp"

    "target_esp32/server.cpp"
    "target_esp32/client_builder.cpp"
    "target_esp32/client_reader.cpp"
    "target_esp32/response_writer.cpp"
    "target_esp32/request.cpp"
    "target_esp32/header.cpp"

    REQUIRES
    "esp_http_server"
    "esp_http_client"
    "ocs_core"
    "ocs_net"

    INCLUDE_DIRS
    ".."
)
