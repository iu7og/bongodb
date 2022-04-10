#pragma once

#include <atomic>

#include "backend/ICommandsBuffer.h"
#include "backend/IProcessor.h"
#include "backend/IStreamer.h"
#include "storages/IStorage.h"

namespace bongodb::Backend {
class TMasterProcessor : public IProcessor {
public:
    Common::TGetResult Get(const Common::TKey& key) override;
    Common::TRemoveResult Remove(const Common::TKey& key) override;
    Common::TTruncateResult Truncate() override;
    Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value) override;
    void Stream(Common::IStreamCommand&& command, Common::TVersion&& version) override;
    Common::TShardKey GetShardKey() override;

private:
    Common::TShard CurrentShard;
    Common::TReplicaKey ThisNodeKey;
    std::unique_ptr<DB::IStorage> Storage;
    /// TODO: implement
    std::unique_ptr<IStreamer> Streamer;
    /// TODO: The same for streamer and processor? (should also maintain versions)
    std::shared_ptr<ICommandsBuffer> CommandsBuffer;
};
}  // namespace bongodb::Backend
