#pragma once
#include "IStorage.h"
#include <shared_mutex>

namespace bongodb::DB {
class TThreadSafeStorageWrapper : public IStorage {
public:
    using IStorage::TGetResponse;

    TGetResponse Get(const Common::TKey& key) override;
    bool Remove(const Common::TKey& key) override;
    void Truncate() override;
    bool Put(Common::TKey&& key, Common::TValue&& value) override;

    TThreadSafeStorageWrapper(std::unique_ptr<IStorage> inner) : Inner(std::move(inner)) {}
private:
    std::shared_mutex Mutex;
    std::unique_ptr<IStorage> Inner;
};
} // namespace bongodb::DB
