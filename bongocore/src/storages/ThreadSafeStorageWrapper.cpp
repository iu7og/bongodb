#include "ThreadSafeStorageWrapper.h"

namespace bongodb::DB {
Common::TGetResult TThreadSafeStorageWrapper::Get(const Common::TKey& key) {
    std::shared_lock lock(Mutex);
    return Inner->Get(key);
}

Common::TRemoveResult TThreadSafeStorageWrapper::Remove(const Common::TKey& key) {
    std::unique_lock lock(Mutex);
    return Inner->Remove(key);
}

Common::TTruncateResult TThreadSafeStorageWrapper::Truncate() {
    std::unique_lock lock(Mutex);
    return Inner->Truncate();
}

Common::TPutResult TThreadSafeStorageWrapper::Put(Common::TKey&& key, Common::TValue&& value) {
    std::unique_lock lock(Mutex);
    return Inner->Put(std::move(key), std::move(value));
}
} // namespace bongodb::DB
