#pragma once
#include <optional>
#include <string>

#include "common/OperationResults.h"
#include "common/types.h"

namespace bongodb::DB {
/**
 * @brief Абстрактный класс, описывающий хранилище.
 */
class IStorage {
public:
    Common::TPutResult Put(const Common::TKey& key, const Common::TValue& value) {
        Common::TKey keyCopy = key;
        Common::TValue valueCopy = value;
        return Put(std::move(keyCopy), std::move(valueCopy));
    }
    /**
     * @brief Получение данных по заданному ключу.
     * @param key Ключ, по которому будет искаться данные.
     * @return Возвращает результат выполнения запроса.
     */
    virtual Common::TGetResult Get(const Common::TKey& key) = 0;
    /**
     * @brief Удаление данных по заданному ключу.
     * @param key Ключ, по которму будут удаляться данные.
     * @return Возвращает результат выполнения запроса.
     */
    virtual Common::TRemoveResult Remove(const Common::TKey& key) = 0;
    /**
     * @brief Удаление всех записей.
     * @return Возвращает результат выполнения запроса.
     */
    virtual Common::TTruncateResult Truncate() = 0;
    /**
     * @brief Устанавливает значение по заданному ключу.
     * @param key Ключ, по которому добавиться значение.
     * @param value Значение, которое будет добавлено.
     * @return Возвращает результат выполнения запроса.
     */
    virtual Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value) = 0;
};
}  // namespace bongodb::DB
