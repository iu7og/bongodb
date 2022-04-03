#pragma once

#include "common/types.h"
#include "common/StreamCommands.h"
#include "common/OperationResults.h"

namespace bongodb::Clients {
class IClient {
public:
    /// Protocol procedures
    /// 1. proxy request
    virtual Common::TGetResult Get(const Common::TKey& key) = 0;
    virtual Common::TRemoveResult Remove(const Common::TKey& key) = 0;
    virtual Common::TTruncateResult Truncate() = 0;
    virtual Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value) = 0;
    /// 2. stream data to replicas (maybe some kind of `handle` should be returned)
    virtual void Stream(const Common::IStreamCommand&& command, Common::TVersion&& version) = 0;

    virtual bool IsReady() = 0;
    virtual bool Prepare() = 0;
};
} // namespace bongodb::Clients
