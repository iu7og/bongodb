#pragma once

#include <atomic>
#include <chrono>
#include <thread>
#include <unordered_map>

#include "backend/ICommandsBuffer.h"
#include "common/Shard.h"
#include "storages/IStorage.h"

namespace bongodb::Backend {
class TStreamer {
    using TState = std::unordered_map<Common::TReplicaKey, Common::TVersion>;
    using TReplicaToDowntime = std::unordered_map<Common::TReplicaKey, std::chrono::steady_clock::time_point>;

public:
    TStreamer(std::shared_ptr<Common::TShard> CurrentShard, Common::TReplicaKey ThisNodeKey,
              std::shared_ptr<ICommandsBuffer> CommandsBuffer);

    void Run();
    ~TStreamer();

private:
    void RunCurrentThread();

    std::shared_ptr<Common::TShard> CurrentShard;
    std::chrono::milliseconds Downtime = std::chrono::milliseconds(2000);
    TReplicaToDowntime Downtimes;
    Common::TReplicaKey ThisNodeKey;
    std::shared_ptr<ICommandsBuffer> CommandsBuffer;

    std::thread Handle;
    std::atomic<bool> IsReady = true;
    TState State;
};
}  // namespace bongodb::Backend
