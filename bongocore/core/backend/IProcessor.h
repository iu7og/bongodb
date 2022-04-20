#pragma once

#include <atomic>

#include "common/OperationResults.h"
#include "common/Shard.h"
#include "common/StreamCommands.h"
#include "common/types.h"

namespace bongodb::Backend {
class IProcessor {
public:
    virtual Common::TGetResult Get(const Common::TKey& key) = 0;
    virtual Common::TRemoveResult Remove(const Common::TKey& key) = 0;
    virtual Common::TTruncateResult Truncate() = 0;
    virtual Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value) = 0;
    virtual void Stream(Common::IStreamCommand&& command, Common::TVersion&& version) = 0;
    virtual Common::TShardKey GetShardKey() = 0;
};
}  // namespace bongodb::Backend
