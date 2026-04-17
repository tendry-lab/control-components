## Release 0.2.0

### New Features

- Add OTA
- Add locate on/locate off
- Add Factory Settings Reset
- Automate release process
- Speed up ADC sampling reading

### Bug Fixes

- Refine default WiFi SSID logic (make sure enough device ID symbols are used in case of long firmware name - bonsai-zero-a-1-k as example)
- Explore random mDNS unavailability (updated toolchain stabilized the mDNS connectivity)
- Resolve issue with frequent status change on the status border (saturated -> wet and then back again to saturated)
- Round moisture to one digit (to reduce noise)
- Fix warning: `W (838) wifi:Password length matches WPA2 standards, authmode threshold changes from OPEN to WPA2`
- Fix warning: `spi_flash: Detected boya flash chip but using generic driver. For optimal functionality, enable SPI_FLASH_SUPPORT_BOYA_CHIP in menuconfig`
- Properly handle POST HTTP requests

### Refactoring

- Unify `enum class` usage: use `uint8_t` if possible
- Unify `uint32_t`, `size_t`, `uint8_t`, `char` usage
- Unify constexpr const usage: use only constexpr for non `const char*`
- Use private inheritance for non-copyable
- Remove ISR version for ISR flag
- Move timer to system
- Move clock to system
- Abstract FS from web gui
- Rework SHT sensor components (handle all sensors from SHT4x family)
- Simplify counter formater (remove inheritance)
- Simplify GPIO types

### Build System

- Add ESP32S2 and ESP32S3 targets

### Legal/IP

- Update license: MPL -> Apache

### Toolchain

- Update mDNS library: [1.4.0](https://components.espressif.com/components/espressif/mdns/versions/1.4.0/readme) -> [1.9.0](https://components.espressif.com/components/espressif/mdns/versions/1.9.0/readme)
- ESP-IDF version: [v5.3.1](https://github.com/espressif/esp-idf/releases/tag/v5.2.1) -> [v5.2.1](https://github.com/espressif/esp-idf/releases/tag/v5.2.1)

## Release 0.1.0

**New Features**

- Add analog soil sensor
- Add analog relay soil sensor
- Add analog lightness sensor
- Add SHT41 sensor
- Add BME280 SPI sensor
- Add DS18B20 temperature sensor
- Add support for 1-Wire devices
- Add SHT41 sensor verification
- Add DS18B20 sensor verification
- Add DS rom code scanner
- Add basic Web GUI
- Add HTTP Web server
- Add WiFi AP mode
- Add WiFi STA mode
- Add mDNS server
- Add GitHub CI actions

**Toolchain**

- ESP-IDF version: [v5.3.1](https://github.com/espressif/esp-idf/releases/tag/v5.3.1)
