## HTTP API

All examples below are run from the terminal and use the [httpie](https://httpie.io/docs/cli) CLI tool to make HTTP requests.

**Receive telemetry data**

http "bonsai-firmware.local/api/v1/telemetry"

```json
{
    "c_sys_lifetime": 4293423,
    "c_sys_uptime": 78,
    "outside_temp": 26.25,
    "sensor_bme280_humidity": 27.54,
    "sensor_bme280_pressure": 1022.82,
    "sensor_bme280_temperature": 24.81,
    "sensor_ldr_lightness": 100,
    "sensor_ldr_raw": 977,
    "sensor_ldr_voltage": 3060,
    "sensor_sht41_humidity": 0,
    "sensor_sht41_temperature": 0,
    "sensor_soil_curr_status": "Dry",
    "sensor_soil_curr_status_dur": 78,
    "sensor_soil_moisture": 24,
    "sensor_soil_prev_status": "Depletion",
    "sensor_soil_prev_status_dur": 1,
    "sensor_soil_raw": 466,
    "sensor_soil_status_progress": 75,
    "sensor_soil_voltage": 1653,
    "sensor_soil_write_count": 1854,
    "soil_temp": 23,
    "system_memory_heap": 186720,
    "system_memory_heap_internal": 186632,
    "system_memory_heap_min": 185844,
    "system_reset_reason": "RST_SW"
}
```

**Receive registration data**

http "bonsai-firmware.local/api/v1/registration"

*WiFi STA mode*

```json
{
    "device_id": "4729289878299275A131A71065E3865F39587987",
    "toolchain_name": "ESP-IDF",
    "toolchain_version": "v5.2.1",
    "fw_description": "firmware",
    "fw_name": "firmware",
    "fw_version": "0.0.0",
    "network_sta_ip": "192.168.1.22",
    "network_sta_rssi": -63,
    "network_sta_signal_strength": "fair",
    "network_sta_ssid": "SSID",
    "product_name": "product",
    "timestamp": 1758007003
}
```

*WiFi AP Mode*

```json
{
    "device_id": "4729289878299275A131A71065E3865F39587987",
    "toolchain_name": "ESP-IDF",
    "toolchain_version": "v5.2.1",
    "fw_description": "firmware",
    "fw_name": "firmware",
    "fw_version": "0.0.0",
    "network_ap_channel": 1,
    "network_ap_cur_conn": 1,
    "network_ap_max_conn": 5,
    "product_name": "product",
    "timestamp": 1758011517
}
```

**Receive system report**

http "bonsai-firmware.local/api/v1/system/report"

```json
{
    "tasks": [
        {
            "core_id": 2147483647,
            "name": "httpd",
            "number": 11,
            "priority": 5,
            "runtime_abs": 228479,
            "runtime_rel": 0.01,
            "stack_free": 1432,
            "state": "Running"
        },
        {
            "core_id": 2147483647,
            "name": "tiT",
            "number": 8,
            "priority": 18,
            "runtime_abs": 540341679,
            "runtime_rel": 17.25,
            "stack_free": 2036,
            "state": "Blocked"
        },
        {
            "core_id": 0,
            "name": "main",
            "number": 4,
            "priority": 1,
            "runtime_abs": 3453028217,
            "runtime_rel": 110.23,
            "stack_free": 1772,
            "state": "Blocked"
        }
    ]
}
```

**Reboot system**

http "bonsai-firmware.local/api/v1/system/reboot"

```txt
Rebooting...
```

**Get system time**

http "bonsai-firmware.local/api/v1/system/time"

```txt
1733233869
```

**Set system time**

http "bonsai-firmware.local/api/v1/system/time?value=1733233869"

```txt
OK
```

**Set mDNS configuration**

http "bonsai-firmware.local/api/v1/config/mdns?hostname=test-lab"

```txt
OK
```

**Reset mDNS configuration**

http "bonsai-firmware.local/api/v1/config/mdns?reset=1

```txt
OK
```

**Get WiFi AP configuration**

http "bonsai-firmware.local/api/v1/config/wifi/ap"

```json
{
    "channel": 6,
    "max_conn": 7,
    "password": ""
}
```

**Set WiFi AP configuration**

http "bonsai-firmware.local/api/v1/config/wifi/ap?password=hQ2BvY2a&channel=1&max_conn=3"

```txt
OK
```

- password - WiFi AP password. min length is 8 symbols, max length is 63 symbols.
- channel - WiFi AP channel. min is 1, max is 13.
- max_conn - maximum number of simultaneous STA connections. min is 1, max is 7. More connections - less performance.

Each parameter can be configured separately.

**Reset WiFi AP configuration**

http "bonsai-firmware.local/api/v1/config/wifi/ap?reset=1"

```txt
OK
```

**Get WiFi STA configuration**

http "bonsai-firmware.local/api/v1/config/wifi/sta"

```json
{
    "retry": 4,
    "ssid": "router-ssid",
    "password": ""
}
```

**Set WiFi STA configuration**

http "bonsai-firmware.local/api/v1/config/wifi/sta?ssid=router-ssid&password=hQ2BvY2a&retry=3"

```txt
OK
```

- ssid - WiFi AP SSID. min length is 1 symbols, max length is 31 symbols.
- password - WiFi STA password. min length is 8 symbols, max length is 63 symbols.
- retry - maximum number of attempts to connect to a WiFi AP. min is 1, max is 5.

Each parameter can be configured separately.

**Reset WiFi STA configuration**

http "bonsai-firmware.local/api/v1/config/wifi/sta?reset=1"

```txt
OK
```

**Sensors APIs**

- [DS18B20](sensors/ds18b20.md#HTTP-API)
- [SHT41](sensors/sht41.md#HTTP-API)
- [Soil Analog](sensors/soil_analog.md#HTTP-API)
- [LDR Analog](sensors/ldr_analog.md#HTTP-API)

## Firmware Configuration Options

- BONSAI_FIRMWARE_HTTP_TELEMETRY_BUFFER_SIZE
- BONSAI_FIRMWARE_HTTP_REGISTRATION_BUFFER_SIZE
