## Introduction

Firmware foundation for ESP32-based devices built with modern C++ (C++17/C++20). mDNS server, HTTP server, WiFi configuration, sensor drivers.

## Features

- mDNS service discovery
- WiFi AP and STA modes
- System and network monitoring
- Builtin HTTP server
- Analog and digital sensors
- Factory reset and device locating

## How To Use

See the detailed [instructions](docs/install/esp32.md) on how to use the `control-components` in your project.

## Contribution

- `master` - stable and ready-to-use branch. [Semver](https://semver.org/) is used for versioning.
- Try to keep PR small.
- New code should be similar to existing code. Use the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
- Ensure the code is properly formatted and includes the licence header. Use [`clang-format`](https://clang.llvm.org/docs/ClangFormat.html) and [`verify_license.py`](tools/scripts/verify_license.py).

## Build Status

- [![ESP32](https://github.com/tendry-lab/esp-components/actions/workflows/esp32.yml/badge.svg)](https://github.com/tendry-lab/esp-components/actions/workflows/esp32.yml)

## License

This project is licensed under the MPL 2.0 License - see the LICENSE file for details.
