#include "ThreadSafeStorageWrapper.h"

namespace bongodb::DB {
TThreadSafeStorageWrapper::TGetResponse TThreadSafeStorageWrapper::Get(const Common::TKey& key) {
    std::shared_lock lock(Mutex);
    return Inner->Get(key);
}

bool TThreadSafeStorageWrapper::Remove(const Common::TKey& key) {
    std::unique_lock lock(Mutex);
    return Inner->Remove(key);
}

void TThreadSafeStorageWrapper::Truncate() {
    std::unique_lock lock(Mutex);
    return Inner->Truncate();
}

bool TThreadSafeStorageWrapper::Put(Common::TKey&& key, Common::TValue&& value) {
    std::unique_lock lock(Mutex);
    return Inner->Put(std::move(key), std::move(value));
}
} // namespace bongodb::DB
