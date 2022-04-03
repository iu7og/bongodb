#pragma once

#include "clients/IClient.h"
#include <unordered_map>
#include <functional>
#include <memory>

namespace bongodb::Common {

using TShardKey = std::uint32_t;
using TReplicaKey = std::uint32_t;
using TShardFn = std::function<TShardKey(Common::TKey)>;


struct TReplica {
    TReplicaKey Key;
    std::unique_ptr<Clients::IClient> Client;
};

struct TShard {
    TShardKey Key;
    // NOTE: should replicas be identified by keys either (as shards)?
    std::vector<std::shared_ptr<TReplica>> Replicas;
    /// reference to master in `replicas` array (nullptr, if master is not known)
    std::shared_ptr<TReplica> Master;
};

/// it may be useful to store TShard for current shard
/// explicitly (the reason to use shared_ptr's)
struct TShards {
    std::unordered_map<TShardKey, std::shared_ptr<TShard>> Cluster;
    TShardFn ShardFn;
};

} // namespace bongodb::Common
