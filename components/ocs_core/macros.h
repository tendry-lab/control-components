/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

//! Structure's fields are packed.
#define OCS_ATTR_PACKED __attribute__((packed))

//! Get number of elements in array.
#define OCS_ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
