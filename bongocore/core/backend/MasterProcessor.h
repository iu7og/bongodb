#pragma once

#include <Poco/Logger.h>
#include <Poco/Util/AbstractConfiguration.h>

#include <atomic>

#include "backend/ICommandsBuffer.h"
#include "backend/IProcessor.h"
#include "backend/Streamer.h"
#include "storages/IStorage.h"

namespace bongodb::Backend {
/**
 * @brief Класс TMasterProcessor, используется на мастер-ноде
 * для обработки команд БД.
 */
class TMasterProcessor : public IProcessor {
public:
    /**
     * @brief Конструктор TMasterProcessor.
     *
     * @param config Конфигурация.
     * @param shards Шарды.
     */
    TMasterProcessor(const Poco::Util::AbstractConfiguration& config, const Common::TShards& shards);

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
     * @brief Стримит команды на реплики.
     * Подробнее: https://redis.io/docs/manual/replication/#how-redis-replication-works
     * @param command Команда.
     * @param version Версия.
     */
    void Stream(std::unique_ptr<Common::IStreamCommand> command, Common::TVersion&& version) override;
    /**
     * @brief Получить ключ шардирования.
     * @return Возвращает ключ шардирования.
     */
    Common::TShardKey GetShardKey() override;
    /**
     * @brief Проверить является ли узел мастером.
     * @return Возвращает true / false.
     */
    bool IsMaster() override { return true; }

private:
    std::shared_ptr<Common::TShard> CurrentShard;
    Common::TReplicaKey ThisNodeKey;
    std::unique_ptr<DB::IStorage> Storage;
    /// TODO: The same for streamer and processor (consumer/producer)
    std::shared_ptr<ICommandsBuffer> CommandsBuffer;
    std::unique_ptr<TStreamer> Streamer;

    Poco::Logger& Logger = Poco::Logger::get("BackendLogger");
};
}  // namespace bongodb::Backend
