#pragma once
#include <string>
#include <optional>
#include "common/types.h"

namespace bongodb::DB {
class IStorage {
public:
    using TGetResponse = std::optional<Common::TValue>;

    bool Put(const Common::TKey& key, const Common::TValue& value) {
        return Put(std::move(key), std::move(value));
    }
    virtual TGetResponse Get(const Common::TKey& key) = 0;
    virtual bool Remove(const Common::TKey& key) = 0;
    virtual void Truncate() = 0;
    virtual bool Put(Common::TKey&& key, Common::TValue&& value) = 0;
};
} // namespace DB
