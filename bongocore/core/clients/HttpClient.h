#pragma once

#include <Poco/Logger.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Util/AbstractConfiguration.h>

#include "clients/HttpPrimitives.h"
#include "clients/IClient.h"

namespace bongodb::Clients {
/**
 * @brief Класс http-клиент БД.
 */
class THttpClient : public IClient {
public:
    /**
     * @brief Конструктор http-клиента БД.
     *
     * @param config Конфигурация для http-клиента.
     */
    THttpClient(const Poco::Util::AbstractConfiguration& config);

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
    Common::TStreamResult Stream(const Common::IStreamCommand& command, const Common::TVersion& version) override;

    /**
     * @brief Проверка готовности.
     * @return Возвращает true / false.
     */
    bool IsReady() override;
    /**
     * @brief Подготовить данные перед работой.
     * @return Возвращает true / false.
     */
    bool Prepare() override;

private:
    THttpResponse SendRequest(THttpRequest&& request);

    std::string Host;
    int Port;
    Poco::Net::HTTPClientSession Session;

    Poco::Logger& Logger = Poco::Logger::get("BackendLogger");
};
}  // namespace bongodb::Clients
