#pragma once

#include <Poco/Util/AbstractConfiguration.h>

#include "backend/ICommandsBuffer.h"
#include "backend/MasterProcessor.h"
#include "backend/ReplicaProcessor.h"

namespace bongodb::Backend {
std::unique_ptr<IProcessor> buildProcessor(const Poco::Util::AbstractConfiguration& config,
                                           const Common::TShards& shards);
std::shared_ptr<ICommandsBuffer> buildCommandsBuffer(const Poco::Util::AbstractConfiguration& config);
}  // namespace bongodb::Backend
