#include "storages/StdHashStorage.h"

namespace bongodb::DB {
Common::TGetResult TStdHashStorage::Get(const Common::TKey& key) {
    auto it = State.find(key);
    return it == State.end() ? Common::TGetResult(Common::EError::NotFound) : it->second;
}

Common::TRemoveResult TStdHashStorage::Remove(const Common::TKey& key) {
    State.erase(key);
    return Common::TRemoveResult();
}

Common::TTruncateResult TStdHashStorage::Truncate() {
    State.clear();
    return Common::TTruncateResult();
}

Common::TPutResult TStdHashStorage::Put(Common::TKey&& key, Common::TValue&& value) {
    auto it = State.find(key);
    if (it == State.end())
        State.insert({std::move(key), std::move(value)});
    else
        it->second = std::move(value);
    return Common::TPutResult();
}
}  // namespace bongodb::DB
