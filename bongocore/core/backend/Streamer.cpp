#include "backend/Streamer.h"

#include <Poco/Logger.h>

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
        auto now = std::chrono::steady_clock::now();
        for (auto& replica : CurrentShard->Replicas) {
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - Downtimes[replica->Key]);
            if (replica->Key == ThisNodeKey || diff < Downtime) continue;
            auto& version = State[replica->Key];
            version = std::max(version, Common::TVersion(1));
            auto command = CommandsBuffer->GetByVersion(version);
            if (!command.IsOk()) {
                continue;
            }
            if (replica->Client->Stream(*command.ExtractValue(), version).IsOk())
                ++version;
            else {
                Poco::Logger::get("Streamer").debug("downtime for " + std::to_string(replica->Key));
                Downtimes[replica->Key] = now;
            }
        }
    }
}
}  // namespace bongodb::Backend
