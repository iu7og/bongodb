#pragma once
#include <memory>
#include <mutex>
#include <shared_mutex>

#include "storages/IStorage.h"

namespace bongodb::DB {
/**
 * @brief Класс safe-thread обертка, вызывающая методы IStorage.
 */
class TThreadSafeStorageWrapper : public IStorage {
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

    /**
     * @brief Конструктор.
     *
     * @param inner Указатель на хранилище.
     */
    TThreadSafeStorageWrapper(std::unique_ptr<IStorage> inner) : Inner(std::move(inner)) {}

private:
    std::shared_mutex Mutex;
    std::unique_ptr<IStorage> Inner;
};
}  // namespace bongodb::DB
