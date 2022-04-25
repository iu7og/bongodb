#pragma once

#include <unordered_map>

#include "storages/IStorage.h"

namespace bongodb::DB {
/**
 * @brief Класс хэш-хранилище.
 */
class TStdHashStorage : public IStorage {
    using TInner = std::unordered_map<Common::TKey, Common::TValue>;

public:
    /**
     * @brief Получение данных по заданному ключу.
     * @param key Ключ, по которому будет искаться данные.
     * @return Возвращает результат выполнения запроса.
     */
    Common::TGetResult Get(const Common::TKey& key) override;
    /**
     * @brief Удаление данных по заданному ключу.
     * @param key Ключ, по которму будут удаляться данные.
     * @return Возвращает результат выполнения запроса.
     */
    Common::TRemoveResult Remove(const Common::TKey& key) override;
    /**
     * @brief Удаление всех записей.
     * @return Возвращает результат выполнения запроса.
     */
    Common::TTruncateResult Truncate() override;
    /**
     * @brief Устанавливает значение по заданному ключу.
     * @param key Ключ, по которому добавиться значение.
     * @param value Значение, которое будет добавлено.
     * @return Возвращает результат выполнения запроса.
     */
    Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value) override;

private:
    TInner State;
};
}  // namespace bongodb::DB
