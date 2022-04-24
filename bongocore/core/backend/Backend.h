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
 * @brief Класс, ...
 */
class TBackend {
public:
    /**
     * Создание ...
     * @brief Конструктор.
     *
     * @param parameter Описание, зачем нужен.
     */
    TBackend(const Poco::Util::AbstractConfiguration& config);

    /**
     * @brief Делает ...
     * @param parameter Описание, что делает.
     * @return Возвращает ...
     */
    Common::TGetResult Get(const Common::TKey& key);
    /**
     * @brief Делает ...
     * @param parameter Описание, что делает.
     * @return Возвращает ...
     */
    Common::TRemoveResult Remove(const Common::TKey& key);
    /**
     * @brief Делает ...
     * @param parameter Описание, что делает.
     * @return Возвращает ...
     */
    Common::TTruncateResult Truncate();
    /**
     * @brief Делает ...
     * @param parameter Описание, что делает.
     * @return Возвращает ...
     */
    Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value);
    /**
     * @brief Делает ...
     * @param parameter Описание, что делает.
     * @return Возвращает ...
     */
    void Stream(std::unique_ptr<Common::IStreamCommand> command, Common::TVersion&& version);

    /**
     * @brief Делает ...
     * @param parameter Описание, что делает.
     * @return Возвращает ...
     */
    Clients::THttpResponse Process(Clients::THttpRequest&& request);

    /**
     * @brief Делает ...
     * @param parameter Описание, что делает.
     * @return Возвращает ...
     */
    bool IsReady();
    /**
     * @brief Делает ...
     * @param parameter Описание, что делает.
     * @return Возвращает ...
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
