#pragma once

#include <atomic>
#include <thread>
#include <unordered_map>

#include "backend/ICommandsBuffer.h"
#include "common/Shard.h"
#include "storages/IStorage.h"

namespace bongodb::Backend {
class TStreamer {
    using TState = std::unordered_map<Common::TReplicaKey, Common::TVersion>;

public:
    TStreamer(std::shared_ptr<Common::TShard> CurrentShard, Common::TReplicaKey ThisNodeKey,
              std::shared_ptr<ICommandsBuffer> CommandsBuffer);

    void Run();
    ~TStreamer();

private:
    void RunCurrentThread();

    std::shared_ptr<Common::TShard> CurrentShard;
    Common::TReplicaKey ThisNodeKey;
    std::shared_ptr<ICommandsBuffer> CommandsBuffer;

    std::thread Handle;
    std::atomic<bool> IsReady = true;
    TState State;
};
}  // namespace bongodb::Backend
