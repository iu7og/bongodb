#pragma once

#include <unordered_map>

#include "storages/IStorage.h"

namespace bongodb::DB {
class TStdHashStorage : public IStorage {
    using TInner = std::unordered_map<Common::TKey, Common::TValue>;

public:
    Common::TGetResult Get(const Common::TKey& key) override;
    Common::TRemoveResult Remove(const Common::TKey& key) override;
    Common::TTruncateResult Truncate() override;
    Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value) override;

private:
    TInner State;
};
}  // namespace bongodb::DB
