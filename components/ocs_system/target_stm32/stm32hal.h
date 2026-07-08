/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#if __has_include(<stm32g4xx_hal.h>)
#include <stm32g4xx_hal.h>
#else
#error "ocs_system: unsupported STM32 family"
#endif
