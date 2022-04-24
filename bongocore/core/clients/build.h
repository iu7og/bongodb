#pragma once

#include <Poco/Util/AbstractConfiguration.h>

#include <memory>

#include "clients/IClient.h"

namespace bongodb::Clients {
std::unique_ptr<IClient> buildClient(const Poco::Util::AbstractConfiguration& config);
}  // namespace bongodb::Clients
