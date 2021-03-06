#include <cstring>

#include "common/build.h"

namespace bongodb::Common {
namespace {
TShardFn buildShardFn(const std::size_t shardsCount) {
    return [shardsCount](const TKey key) { return std::hash<std::string>{}(key) % shardsCount; };
}

std::shared_ptr<TReplica> buildReplica(const Poco::Util::AbstractConfiguration& config) {
    auto replica = std::make_shared<TReplica>();
    replica->Key = config.getInt("key");
    replica->Client = Clients::buildClient(*config.createView("client"));
    return replica;
}

std::shared_ptr<TShard> buildShard(const Poco::Util::AbstractConfiguration& config) {
    auto shard = std::make_shared<TShard>();
    shard->Key = config.getInt("key");
    TReplicaKey masterReplicaKey = config.getInt("master_replica_key");

    Poco::Util::AbstractConfiguration::Keys keys;
    config.keys(keys);
    for (const auto& k : keys) {
        if (!strncmp(k.data(), "replica", strlen("replica"))) {
            shard->Replicas.push_back(buildReplica(*config.createView(k)));
            if (shard->Replicas.back()->Key == masterReplicaKey) shard->Master = shard->Replicas.back();
        }
    }

    return shard;
}

TShards::TShardsMap buildCluster(const Poco::Util::AbstractConfiguration& config) {
    TShards::TShardsMap shards;
    Poco::Util::AbstractConfiguration::Keys keys;
    config.keys(keys);
    for (const auto& k : keys) {
        if (!strncmp(k.data(), "shard", strlen("shard"))) {
            auto shard = buildShard(*config.createView(k));
            shards[shard->Key] = shard;
        }
    }

    return shards;
}
}  // namespace

TShards buildShards(const Poco::Util::AbstractConfiguration& config) {
    TShards shards;
    shards.Cluster = buildCluster(*config.createView("cluster"));
    const auto shardsCount = shards.Cluster.size();
    shards.ShardFn = buildShardFn(shardsCount);
    return shards;
}
}  // namespace bongodb::Common
