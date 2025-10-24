/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_security/sha_generator.h"
#include "ocs_algo/sha_engine_ops.h"

namespace ocs {
namespace security {

ShaGenerator::ShaGenerator(IShaEngine& engine)
    : engine_(engine) {
    sha_.resize(algo::ShaEngineOps::hash_lenght(engine_.get_algorithm()));
}

const ShaGenerator::Data& ShaGenerator::get_sha() const {
    return sha_;
}

void ShaGenerator::add(const uint8_t* buf, unsigned size) {
    for (unsigned n = 0; n < size; ++n) {
        src_.push_back(buf[n]);
    }
}

status::StatusCode ShaGenerator::generate() {
    return engine_.generate(sha_.data(), src_.data(), src_.size());
}

} // namespace security
} // namespace ocs
