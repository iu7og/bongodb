#pragma once

#include <atomic>
#include <queue>
#include <Poco/Util/AbstractConfiguration.h>

#include "backend/ICommandsBuffer.h"
#include "backend/IProcessor.h"
#include "storages/IStorage.h"
#include "storages/build.h"

namespace bongodb::Backend {
class TReplicaProcessor : public IProcessor {
    class TStreamProcessor {
        using TQueueElement = std::pair<Common::TVersion, std::unique_ptr<Common::IStreamCommand>>;
        using TQueueElementPtr = std::shared_ptr<TQueueElement>;
        using TQueue = std::priority_queue<TQueueElementPtr>;

    public:
        TStreamProcessor(std::shared_ptr<DB::IStorage> storage);
        void Push(Common::TVersion &&version, std::unique_ptr<Common::IStreamCommand> command);
        void FlushQueue();
        void SetVersion(const Common::TVersion &version);
        void Reset();

    private:
        bool ApplyCommand(Common::IStreamCommand &command);

        TQueue Queue;
        Common::TVersion Version;
        std::shared_ptr<DB::IStorage> Storage;
    };

public:
    TReplicaProcessor(const Poco::Util::AbstractConfiguration& config, const Common::TShards& shards);

    Common::TGetResult Get(const Common::TKey &key) override;
    Common::TRemoveResult Remove(const Common::TKey &key) override;
    Common::TTruncateResult Truncate() override;
    Common::TPutResult Put(Common::TKey &&key, Common::TValue &&value) override;
    void Stream(std::unique_ptr<Common::IStreamCommand> command, Common::TVersion &&version) override;
    Common::TShardKey GetShardKey() override;

private:
    std::shared_ptr<Common::TShard> CurrentShard;
    std::shared_ptr<DB::IStorage> Storage;
    TStreamProcessor StreamProcessor;
};
}  // namespace bongodb::Backend
