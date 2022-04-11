#include "backend/Streamer.h"

#include <algorithm>

namespace bongodb::Backend {
TStreamer::TStreamer(std::shared_ptr<Common::TShard> currentShard, Common::TReplicaKey thisNodeKey,
                     std::shared_ptr<ICommandsBuffer> commandsBuffer)
    : CurrentShard(currentShard), ThisNodeKey(thisNodeKey), CommandsBuffer(commandsBuffer) {
    Run();
}

TStreamer::~TStreamer() {
    IsReady = false;
    if (Handle.joinable()) Handle.join();
}

void TStreamer::Run() {
    Handle = std::thread([this]() { RunCurrentThread(); });
}

void TStreamer::RunCurrentThread() {
    while (IsReady) {
        for (auto& replica : CurrentShard->Replicas) {
            if (replica->Key == ThisNodeKey) continue;
            auto& version = State[replica->Key];
            version = std::max(version, Common::TVersion(1));
            auto command = CommandsBuffer->GetByVersion(version);
            if (!command.IsOk()) continue;
            if (replica->Client->Stream(*command.ExtractValue(), version).IsOk()) ++version;
        }
    }
}
}  // namespace bongodb::Backend
