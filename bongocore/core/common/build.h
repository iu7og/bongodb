#pragma once

#include <Poco/Util/AbstractConfiguration.h>
#include "common/Shard.h"
#include "clients/build.h"

namespace bongodb::Common {
TShards buildShards(const Poco::Util::AbstractConfiguration& config);
} // namespace bongodb::Common
