#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>

#include "backend/Backend.h"

/**
 * @brief Класс отвечающий за обработку запросов.
 */
class RequestHandler : public Poco::Net::HTTPRequestHandler {
public:
    /**
     * @brief Конструктор класса обработающего запроса.
     *
     * @param Logger Логгер.
     * @param backed Бэкенд БД.
     */
    RequestHandler(Poco::Logger& Logger, bongodb::Backend::TBackend& backend);

private:
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;

    bongodb::Backend::TBackend& Backend;
    Poco::Logger& Logger;
};

/**
 * @brief Фабрика обработчиков запроса.
 */
class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    /**
     * @brief Конструктор фабрики.
     *
     * @param config Конфигурация фабрики.
     */
    RequestHandlerFactory(const Poco::Util::AbstractConfiguration& config);

private:
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&) override;

    std::unique_ptr<bongodb::Backend::TBackend> Backend;
    Poco::Logger& Logger = Poco::Logger::get("RequestHandler");
};
