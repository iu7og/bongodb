#include "HandlerFactory.h"
#include <Poco/Net/HTTPRequest.h>
#include <Poco/StreamCopier.h>
#include <sstream>
#include "clients/HttpPrimitives.h"

RequestHandler::RequestHandler(Poco::Logger& logger, bongodb::Backend::TBackend& backend)
    : Backend(backend)
    , Logger(logger)
{
}

void RequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
    Poco::Net::HTTPRequest pocoRequest = Poco::Net::HTTPRequest(request.getMethod(), request.getURI());
    std::ostringstream ss;
    Poco::StreamCopier::copyStream(request.stream(), ss);

    auto httpRequest = bongodb::Clients::THttpRequest::FromPocoHttpRequest(pocoRequest, ss.str());
    Logger.debug("Request from %s", request.clientAddress().toString());
    try {
        auto backendResponse = Backend.Process(std::move(httpRequest));

        response.setChunkedTransferEncoding(true);
        response.setContentType("text/json");
        response.setStatus(backendResponse.GetStatus());

        response.send() << backendResponse.GetBody();
    } catch (...) {
        Logger.debug("Not processed(((");
    }
}

RequestHandlerFactory::RequestHandlerFactory(const Poco::Util::AbstractConfiguration& config) {
    Backend = std::make_unique<bongodb::Backend::TBackend>(config);
}

Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest&)
{
    return new RequestHandler(Logger, *Backend);
}
