#pragma once

#include <Poco/Util/AbstractConfiguration.h>

#include "clients/build.h"
#include "common/Shard.h"

namespace bongodb::Common {
TShards buildShards(const Poco::Util::AbstractConfiguration& config);
}  // namespace bongodb::Common
