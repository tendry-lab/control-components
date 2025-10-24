/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>
#include <utility>
#include <vector>

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace net {

class MdnsService : private core::NonCopyable<> {
public:
    //! Known mDNS services.
    //!
    //! @references
    //!  See common services: http://www.dns-sd.org/serviceTypes.html
    enum class ServiceType : uint8_t {
        //! HTTP mDNS service.
        Http,
    };

    //! Known transport protocols.
    enum class Proto : uint8_t {
        //! For application protocols that run over TCP.
        Tcp,
    };

    using TxtRecord = std::pair<std::string, std::string>;
    using TxtRecordList = std::vector<TxtRecord>;

    //! Initialize.
    //!
    //! @params
    //!  - @p instance_name is the mDNS service instance name, e.g. "Bonsai GrowLab".
    //!  - @p service is the mDNS service type, e.g. "_http".
    //!  - @p proto is the mDNS transport protocol, e.g. "_tcp".
    //!  - @p hostname is the machine DNS name, e.g. "bonsai-growlab".
    //!  - @p port is the mDNS service port, e.g. 80.
    //!  - @p domain is the mDNS service domain, e.g. "local".
    MdnsService(const char* instance_name,
                ServiceType service_type,
                Proto proto,
                const char* domain,
                const char* hostname,
                uint16_t port);

    const char* get_instance_name() const;
    const char* get_service_type() const;
    const char* get_proto() const;
    const char* get_domain() const;
    const char* get_hostname() const;
    uint16_t get_port() const;
    const TxtRecordList& get_txt_records() const;

    //! Return URI scheme based on service type.
    const char* get_scheme() const;

    //! Append txt record to the service.
    void add_txt_record(const char* key, const char* value);

private:
    const std::string instance_name_;
    const ServiceType service_type_;
    const Proto proto_;
    const std::string domain_;
    const std::string hostname_;
    const uint16_t port_ { 0 };

    TxtRecordList txt_records_;
};

} // namespace net
} // namespace ocs
