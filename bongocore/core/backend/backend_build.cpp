#include "backend/build.h"
#include "backend/VectorCommandsBuffer.h"

namespace bongodb::Backend {
std::unique_ptr<IProcessor> buildProcessor(const Poco::Util::AbstractConfiguration& config, const Common::TShards& shards) {
    const auto shardKey = config.getUInt("shard");
    const auto replicaKey = config.getInt("replica");

    auto shardIt = shards.Cluster.find(shardKey);
    if (shardIt == shards.Cluster.end())
        throw std::runtime_error("shard not found");

    if (shardIt->second->Master->Key == replicaKey)
        return std::make_unique<TMasterProcessor>(config, shards);
    else
        return std::make_unique<TReplicaProcessor>(config, shards);
}

std::shared_ptr<ICommandsBuffer> buildCommandsBuffer(const Poco::Util::AbstractConfiguration& config) {
    return std::make_shared<TVectorCommandsBuffer>();
}
} // namespace bongodb::Backend
