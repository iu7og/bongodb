#include "storages/BongoStorage.h"

namespace bongodb::DB {
Common::TGetResult TBongoStorage::Get(const Common::TKey& key) {
    auto it = State.find(key);
    return it == State.end() ? Common::TGetResult(Common::EError::NotFound) : it->second;
}

Common::TRemoveResult TBongoStorage::Remove(const Common::TKey& key) {
    State.erase(key);
    return Common::TRemoveResult();
}

Common::TTruncateResult TBongoStorage::Truncate() {
    State.clear();
    return Common::TTruncateResult();
}

Common::TPutResult TBongoStorage::Put(Common::TKey&& key, Common::TValue&& value) {
    auto it = State.find(key);
    if (it == State.end())
        State.insert({std::move(key), std::move(value)});
    else
        State[std::move(key)] = std::move(value);
    return Common::TPutResult();
}
}  // namespace bongodb::DB
