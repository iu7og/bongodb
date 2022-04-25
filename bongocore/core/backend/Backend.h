/*!
    \file Backend.h
    \brief угугуг
*/
#pragma once

#include <Poco/Logger.h>
#include <Poco/Util/AbstractConfiguration.h>

#include <atomic>

#include "backend/IProcessor.h"
#include "clients/HttpPrimitives.h"
#include "common/OperationResults.h"
#include "common/Shard.h"
#include "common/StreamCommands.h"
#include "common/types.h"

namespace bongodb::Backend {
/**
 * @brief Класс бэкенд работы с БД.
 */
class TBackend {
public:
    /**
     * @brief Конструктор бэкенда.
     *
     * @param config Конфигурация для бэкенда.
     */
    TBackend(const Poco::Util::AbstractConfiguration& config);

    /**
     * @brief Получение данных по заданному ключу.
     * @param key Ключ, по которому будет искаться данные.
     * @return Возвращает результат выполнения запроса.
     */
    Common::TGetResult Get(const Common::TKey& key);
    /**
     * @brief Удаление данных по заданному ключу.
     * @param key Ключ, по которму будут удаляться данные.
     * @return Возвращает результат выполнения запроса.
     */
    Common::TRemoveResult Remove(const Common::TKey& key);
    /**
     * @brief Удаление всех записей.
     * @return Возвращает результат выполнения запроса.
     */
    Common::TTruncateResult Truncate();
    /**
     * @brief Устанавливает значение по заданному ключу.
     * @param key Ключ, по которому добавиться значение.
     * @param value Значение, которое будет добавлено.
     * @return Возвращает результат выполнения запроса.
     */
    Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value);
    /**
     * @brief Стримит команды на реплики.
     * Подробнее: https://redis.io/docs/manual/replication/#how-redis-replication-works
     * @param command Команда.
     * @param version Версия.
     */
    void Stream(std::unique_ptr<Common::IStreamCommand> command, Common::TVersion&& version);

    /**
     * @brief Обработчик http-запроса.
     * @param request http-запрос.
     * @return Возвращает http-ответ.
     */
    Clients::THttpResponse Process(Clients::THttpRequest&& request);

    /**
     * @brief Проверка готовности.
     * @return Возвращает true / false.
     */
    bool IsReady();
    /**
     * @brief Подготовить данные перед работой.
     * @return Возвращает true / false.
     */
    bool Prepare();

private:
    bool IsForCurrentShard(const Common::TKey& key);
    std::shared_ptr<Common::TReplica> ChooseReplica(const Common::TKey& key);

    Common::TShards Shards;
    std::unique_ptr<IProcessor> Processor;
    /// State can change in time (shard master changed and etc) and prepare needs to be called
    std::atomic<bool> Ready = false;
    Poco::Logger& Logger = Poco::Logger::get("BackendLogger");
};
}  // namespace bongodb::Backend
