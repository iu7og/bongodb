#pragma once
#include <string>
#include <optional>
#include "common/types.h"
#include "common/OperationResults.h"

namespace bongodb::DB {
class IStorage {
public:
    Common::TPutResult Put(const Common::TKey& key, const Common::TValue& value) {
        return Put(std::move(key), std::move(value));
    }
    virtual Common::TGetResult Get(const Common::TKey& key) = 0;
    virtual Common::TRemoveResult Remove(const Common::TKey& key) = 0;
    virtual Common::TTruncateResult Truncate() = 0;
    virtual Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value) = 0;
};
} // namespace DB
