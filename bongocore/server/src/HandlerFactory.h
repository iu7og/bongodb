#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>

#include "backend/Backend.h"

class RequestHandler: public Poco::Net::HTTPRequestHandler
{
public:
    RequestHandler(bongodb::Backend::TBackend* backend);
private:
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;

    bongodb::Backend::TBackend* Backend;
};

class RequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
{
public:
    RequestHandlerFactory(const Poco::Util::AbstractConfiguration& config);

private:
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&) override;

    std::unique_ptr<bongodb::Backend::TBackend> Backend;
};
