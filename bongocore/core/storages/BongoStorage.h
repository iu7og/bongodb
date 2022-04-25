#pragma once

#include <tsl/robin_map.h>

#include "storages/IStorage.h"

namespace bongodb::DB {
/**
 * @brief Класс хранилище BongoDB, хранящее внутренний статус системы и
 * предоставляющий методы для управления сиситемой.
 * Основано на fast hash-хранилище (robin_map).
 */
class TBongoStorage : public IStorage {
    using TInner = tsl::robin_map<Common::TKey, Common::TValue>;

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
