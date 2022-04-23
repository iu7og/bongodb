#include "backend/ReplicaProcessor.h"

namespace bongodb::Backend {
TReplicaProcessor::TStreamProcessor::TStreamProcessor(std::shared_ptr<DB::IStorage> storage)
    : Queue(), Storage(storage) {}

void TReplicaProcessor::TStreamProcessor::Push(Common::TVersion &&version, std::unique_ptr<Common::IStreamCommand> command) {
    Queue.push(std::make_shared<TQueueElement>(std::move(version), std::move(command)));
    FlushQueue();
}

void TReplicaProcessor::TStreamProcessor::SetVersion(const Common::TVersion &version) {
    Version = version;
    FlushQueue();
}

void TReplicaProcessor::TStreamProcessor::FlushQueue() {
    while (!Queue.empty() && Queue.top()->first == Version + 1) {
        auto element = Queue.top();
        Queue.pop();
        ++Version;
        /// TODO: check returned value?
        ApplyCommand(*element->second);
    }
}

void TReplicaProcessor::TStreamProcessor::Reset() {
    Version = {};
    Queue = TQueue();
}

bool TReplicaProcessor::TStreamProcessor::ApplyCommand(Common::IStreamCommand &command) {
    switch (command.GetType()) {
        case Common::EStreamCommandType::Truncate:
            return Storage->Truncate().IsOk();
        case Common::EStreamCommandType::Remove:
            return Storage->Remove(*command.GetKey()).IsOk();
        case Common::EStreamCommandType::Put:
            return Storage->Put(*command.ExtractKey(), *command.ExtractValue()).IsOk();
    }
    throw std::runtime_error("unknown stream command");
}


TReplicaProcessor::TReplicaProcessor(const Poco::Util::AbstractConfiguration& config, const Common::TShards& shards)
    : CurrentShard(shards.Cluster.at(config.getInt("shard"))), Storage(DB::buildStorage(*config.createView("storage"))), StreamProcessor(Storage) {}

Common::TGetResult TReplicaProcessor::Get(const Common::TKey &key) { return Storage->Get(key); }

Common::TRemoveResult TReplicaProcessor::Remove(const Common::TKey &key) {
    return CurrentShard->Master ? CurrentShard->Master->Client->Remove(key) : Common::EError::NotAvail;
}

Common::TTruncateResult TReplicaProcessor::Truncate() {
    return CurrentShard->Master ? CurrentShard->Master->Client->Truncate() : Common::EError::NotAvail;
}

Common::TPutResult TReplicaProcessor::Put(Common::TKey &&key, Common::TValue &&value) {
    return CurrentShard->Master ? CurrentShard->Master->Client->Put(std::move(key), std::move(value))
                               : Common::EError::NotAvail;
}

void TReplicaProcessor::Stream(std::unique_ptr<Common::IStreamCommand> command, Common::TVersion &&version) {
    StreamProcessor.Push(std::move(version), std::move(command));
}

Common::TShardKey TReplicaProcessor::GetShardKey() { return CurrentShard->Key; }
}  // namespace bongodb::Backend
