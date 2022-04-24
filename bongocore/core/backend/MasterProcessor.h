#pragma once

#include <Poco/Logger.h>
#include <Poco/Util/AbstractConfiguration.h>

#include <atomic>

#include "backend/ICommandsBuffer.h"
#include "backend/IProcessor.h"
#include "backend/Streamer.h"
#include "storages/IStorage.h"

namespace bongodb::Backend {
class TMasterProcessor : public IProcessor {
public:
    TMasterProcessor(const Poco::Util::AbstractConfiguration& config, const Common::TShards& shards);

    Common::TGetResult Get(const Common::TKey& key) override;
    Common::TRemoveResult Remove(const Common::TKey& key) override;
    Common::TTruncateResult Truncate() override;
    Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value) override;
    void Stream(std::unique_ptr<Common::IStreamCommand> command, Common::TVersion&& version) override;
    Common::TShardKey GetShardKey() override;
    bool IsMaster() override { return true; }

private:
    std::shared_ptr<Common::TShard> CurrentShard;
    Common::TReplicaKey ThisNodeKey;
    std::unique_ptr<DB::IStorage> Storage;
    /// TODO: The same for streamer and processor (consumer/producer)
    std::shared_ptr<ICommandsBuffer> CommandsBuffer;
    std::unique_ptr<TStreamer> Streamer;

    Poco::Logger& Logger = Poco::Logger::get("BackendLogger");
};
}  // namespace bongodb::Backend
