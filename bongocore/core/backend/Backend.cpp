#include "Backend.h"

#include <cstdlib>

#include "backend/build.h"
#include "common/build.h"

namespace bongodb::Backend {
TBackend::TBackend(const Poco::Util::AbstractConfiguration& config) {
    Shards = Common::buildShards(*config.createView("shards"));
    Processor = buildProcessor(*config.createView("processor"), Shards);
    Logger.information("Is master - " + std::to_string(Processor->IsMaster()));
}

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

bool TBackend::IsForCurrentShard(const Common::TKey& key) {
    const auto result = Processor->GetShardKey() == Shards.ShardFn(key);
    if (result) Logger.debug("Operation is for current shard, processing..");
    return result;
}

void TBackend::Stream(std::unique_ptr<Common::IStreamCommand> command, Common::TVersion&& version) {
    Processor->Stream(std::move(command), std::move(version));
}

Clients::THttpResponse TBackend::Process(Clients::THttpRequest&& request) {
    switch (request.GetType()) {
        case Clients::EOperationType::Get:
            Logger.trace((Processor->IsMaster() ? "Master" : "Replica") + std::string(" get"));
            return Clients::THttpResponse(Get(request.ExtractKey()));
        case Clients::EOperationType::Put:
            Logger.trace((Processor->IsMaster() ? "Master" : "Replica") + std::string(" put"));
            return Clients::THttpResponse(Put(request.ExtractKey(), request.ExtractValue()));
        case Clients::EOperationType::Delete:
            Logger.trace((Processor->IsMaster() ? "Master" : "Replica") + std::string(" delete"));
            return Clients::THttpResponse(Remove(request.ExtractKey()));
        case Clients::EOperationType::Truncate:
            Logger.trace((Processor->IsMaster() ? "Master" : "Replica") + std::string(" truncate"));
            return Clients::THttpResponse(Remove(request.ExtractKey()));
        case Clients::EOperationType::Stream:
            auto [command, version] = request.ExtractStreamCommandAndVersion<std::unique_ptr>();
            Logger.trace((Processor->IsMaster() ? "Master" : "Replica") +
                         std::string(" stream (type: " + std::to_string(static_cast<int>(command->GetType())) + ")"));
            Stream(std::move(command), std::move(version));
            return Clients::THttpResponse();
    }

    return Common::EError::NotAvail;
}

bool TBackend::IsReady() { return Ready; }

bool TBackend::Prepare() { return Ready = true; }

std::shared_ptr<Common::TReplica> TBackend::ChooseReplica(const Common::TKey& key) {
    auto shard = Shards.Cluster[Shards.ShardFn(key)];
    if (shard->Master) {
        Logger.debug("Proxying to master");
        return shard->Master;
    }

    auto replicaKey = std::rand() % shard->Replicas.size();
    Logger.debug("Proxying to replica with key=" + std::to_string(replicaKey));
    return shard->Replicas[replicaKey];
}
}  // namespace bongodb::Backend
