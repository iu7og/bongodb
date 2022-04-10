#include "Backend.h"

#include <cstdlib>

namespace bongodb::Backend {
Common::TGetResult TBackend::Get(const Common::TKey& key) {
    if (!IsReady() && !Prepare()) return Common::EError::NotAvail;
    return IsForCurrentShard(key) ? Processor->Get(key) : ChooseReplica(key)->Client->Get(key);
}

Common::TRemoveResult TBackend::Remove(const Common::TKey& key) {
    if (!IsReady() && !Prepare()) return Common::EError::NotAvail;
    return IsForCurrentShard(key) ? Processor->Remove(key) : ChooseReplica(key)->Client->Remove(key);
}

Common::TTruncateResult TBackend::Truncate() {
    if (!IsReady() && !Prepare()) return Common::EError::NotAvail;
    return Processor->Truncate();
}

Common::TPutResult TBackend::Put(Common::TKey&& key, Common::TValue&& value) {
    if (!IsReady() && !Prepare()) return Common::EError::NotAvail;
    return IsForCurrentShard(key) ? Processor->Put(std::move(key), std::move(value))
                                  : ChooseReplica(key)->Client->Put(std::move(key), std::move(value));
}

bool TBackend::IsForCurrentShard(const Common::TKey& key) { return Processor->GetShardKey() == Shards.ShardFn(key); }

void TBackend::Stream(Common::IStreamCommand&& command, Common::TVersion&& version) {
    Processor->Stream(std::move(command), std::move(version));
}

bool TBackend::IsReady() { return Ready; }

bool TBackend::Prepare() {
    /// TODO: maybe prepare for all clients should be called here?
    return Ready = true;
}

std::shared_ptr<Common::TReplica> TBackend::ChooseReplica(const Common::TKey& key) {
    auto shard = Shards.Cluster[Shards.ShardFn(key)];
    return shard->Master ? shard->Master : shard->Replicas[std::rand() % shard->Replicas.size()];
}
}  // namespace bongodb::Backend
