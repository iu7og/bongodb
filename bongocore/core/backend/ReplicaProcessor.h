#pragma once

#include <Poco/Util/AbstractConfiguration.h>

#include <atomic>
#include <queue>

#include "backend/ICommandsBuffer.h"
#include "backend/IProcessor.h"
#include "storages/IStorage.h"
#include "storages/build.h"

namespace bongodb::Backend {
/**
 * @brief Класс, отвечающий за управление репликами.
 */
class TReplicaProcessor : public IProcessor {
    /**
     * @brief Класс, который управляет стримами.
     */
    class TStreamProcessor {
        using TQueueElement = std::pair<Common::TVersion, std::unique_ptr<Common::IStreamCommand>>;
        using TQueueElementPtr = std::shared_ptr<TQueueElement>;
        using TQueue = std::priority_queue<TQueueElementPtr>;

    public:
        /**
         * @brief Конструктор.
         *
         * @param storage Хранилище.
         */
        TStreamProcessor(std::shared_ptr<DB::IStorage> storage);

        /**
         * @brief Добавить в очередь.
         * @param version Версия.
         * @param command Команда.
         */
        void Push(Common::TVersion &&version, std::unique_ptr<Common::IStreamCommand> command);
        /**
         * @brief Очистить очередь.
         */
        void FlushQueue();
        /**
         * @brief Установить версию.
         * @param version Версия.
         */
        void SetVersion(const Common::TVersion &version);
        /**
         * @brief Метод reset.
         */
        void Reset();

    private:
        bool ApplyCommand(Common::IStreamCommand &command);

        TQueue Queue;
        Common::TVersion Version;
        std::shared_ptr<DB::IStorage> Storage;
    };

public:
    /**
     * @brief Конструктор.
     *
     * @param config Конфигурация.
     * @param shards Шарды.
     */
    TReplicaProcessor(const Poco::Util::AbstractConfiguration &config, const Common::TShards &shards);

    /**
     * @brief Получение данных по заданному ключу.
     * @param key Ключ, по которому будет искаться данные.
     * @return Возвращает результат выполнения запроса.
     */
    Common::TGetResult Get(const Common::TKey &key) override;
    /**
     * @brief Удаление данных по заданному ключу.
     * @param key Ключ, по которму будут удаляться данные.
     * @return Возвращает результат выполнения запроса.
     */
    Common::TRemoveResult Remove(const Common::TKey &key) override;
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
    Common::TPutResult Put(Common::TKey &&key, Common::TValue &&value) override;
    /**
     * @brief Стримит команды на реплики.
     * Подробнее: https://redis.io/docs/manual/replication/#how-redis-replication-works
     * @param command Команда.
     * @param version Версия.
     */
    void Stream(std::unique_ptr<Common::IStreamCommand> command, Common::TVersion &&version) override;
    /**
     * @brief Получить ключ шардирования.
     * @return Возвращает ключ шардирования.
     */
    Common::TShardKey GetShardKey() override;

private:
    std::shared_ptr<Common::TShard> CurrentShard;
    std::shared_ptr<DB::IStorage> Storage;
    TStreamProcessor StreamProcessor;
};
}  // namespace bongodb::Backend
