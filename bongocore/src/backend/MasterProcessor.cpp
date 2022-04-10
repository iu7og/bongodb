#include "backend/MasterProcessor.h"

namespace bongodb::Backend {
Common::TGetResult TMasterProcessor::Get(const Common::TKey& key) { return Storage->Get(key); }

Common::TRemoveResult TMasterProcessor::Remove(const Common::TKey& key) {
    auto result = Storage->Remove(key);
    if (!result.IsOk()) return result;

    auto command = std::make_shared<Common::TRemoveStreamCommand>(key);
    CommandsBuffer->Push(std::move(command));
    return result;
}

Common::TTruncateResult TMasterProcessor::Truncate() {
    auto result = Storage->Truncate();
    if (!result.IsOk()) return result;

    auto command = std::make_shared<Common::TTruncateStreamCommand>();
    CommandsBuffer->Push(command);
    return result;
}

Common::TPutResult TMasterProcessor::Put(Common::TKey&& key, Common::TValue&& value) {
    auto result = Storage->Put(key, value);
    if (!result.IsOk()) return result;

    auto command = std::make_shared<Common::TPutStreamCommand>(std::move(key), std::move(value));
    CommandsBuffer->Push(command);
    return result;
}

void TMasterProcessor::Stream(Common::IStreamCommand&& command, Common::TVersion&& version) {
    /// Master processor doesn't receive stream (only sends)
}

Common::TShardKey TMasterProcessor::GetShardKey() { return CurrentShard.Key; }
}  // namespace bongodb::Backend
