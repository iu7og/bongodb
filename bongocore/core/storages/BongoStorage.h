#pragma once

#include <tsl/robin_map.h>

#include "storages/IStorage.h"

namespace bongodb::DB {
class TBongoStorage : public IStorage {
    using TInner = tsl::robin_map<Common::TKey, Common::TValue>;

public:
    Common::TGetResult Get(const Common::TKey& key) override;
    Common::TRemoveResult Remove(const Common::TKey& key) override;
    Common::TTruncateResult Truncate() override;
    Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value) override;

private:
    TInner State;
};
}  // namespace bongodb::DB
