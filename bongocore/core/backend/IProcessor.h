#pragma once

#include <atomic>

#include "common/OperationResults.h"
#include "common/Shard.h"
#include "common/StreamCommands.h"
#include "common/types.h"

namespace bongodb::Backend {
/**
 * @brief Абстрактный класс, задающий интерфейс работы с данными.
 */
class IProcessor {
public:
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
    /**
     * @brief Стримит команды на реплики.
     * Подробнее: https://redis.io/docs/manual/replication/#how-redis-replication-works
     * @param command Команда.
     * @param version Версия.
     */
    virtual void Stream(std::unique_ptr<Common::IStreamCommand> command, Common::TVersion&& version) = 0;
    /**
     * @brief Получить ключ шардирования.
     * @return Возвращает ключ шардирования.
     */
    virtual Common::TShardKey GetShardKey() = 0;
    /**
     * @brief Проверить является ли узел мастером.
     * @return Возвращает true / false.
     */
    virtual bool IsMaster() { return false; }
};
}  // namespace bongodb::Backend
