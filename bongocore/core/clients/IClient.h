#pragma once

#include "common/OperationResults.h"
#include "common/StreamCommands.h"
#include "common/types.h"

namespace bongodb::Clients {
/**
 * @brief Абстрактный класс, клиент работы с БД.
 */
class IClient {
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
    virtual Common::TStreamResult Stream(const Common::IStreamCommand& command, const Common::TVersion& version) = 0;

    /**
     * @brief Проверка готовности.
     * @return Возвращает true / false.
     */
    virtual bool IsReady() = 0;
    /**
     * @brief Подготовить данные перед работой.
     * @return Возвращает true / false.
     */
    virtual bool Prepare() = 0;
};
}  // namespace bongodb::Clients
