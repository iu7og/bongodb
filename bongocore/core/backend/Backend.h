#pragma once

#include <Poco/Logger.h>
#include <Poco/Util/AbstractConfiguration.h>

#include <atomic>

#include "backend/IProcessor.h"
#include "clients/HttpPrimitives.h"
#include "common/OperationResults.h"
#include "common/Shard.h"
#include "common/StreamCommands.h"
#include "common/types.h"

namespace bongodb::Backend {
class TBackend {
public:
    TBackend(const Poco::Util::AbstractConfiguration& config);

    Common::TGetResult Get(const Common::TKey& key);
    Common::TRemoveResult Remove(const Common::TKey& key);
    Common::TTruncateResult Truncate();
    Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value);
    void Stream(std::unique_ptr<Common::IStreamCommand> command, Common::TVersion&& version);

    Clients::THttpResponse Process(Clients::THttpRequest&& request);

    bool IsReady();
    bool Prepare();

private:
    bool IsForCurrentShard(const Common::TKey& key);
    std::shared_ptr<Common::TReplica> ChooseReplica(const Common::TKey& key);

    Common::TShards Shards;
    std::unique_ptr<IProcessor> Processor;
    /// State can change in time (shard master changed and etc) and prepare needs to be called
    std::atomic<bool> Ready = false;
    Poco::Logger& Logger = Poco::Logger::get("BackendLogger");
};
}  // namespace bongodb::Backend
