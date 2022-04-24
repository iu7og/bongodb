#pragma once
#include <memory>
#include <mutex>
#include <shared_mutex>

#include "storages/IStorage.h"

namespace bongodb::DB {
class TThreadSafeStorageWrapper : public IStorage {
public:
    Common::TGetResult Get(const Common::TKey& key) override;
    Common::TRemoveResult Remove(const Common::TKey& key) override;
    Common::TTruncateResult Truncate() override;
    Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value) override;

    TThreadSafeStorageWrapper(std::unique_ptr<IStorage> inner) : Inner(std::move(inner)) {}

private:
    std::shared_mutex Mutex;
    std::unique_ptr<IStorage> Inner;
};
}  // namespace bongodb::DB
