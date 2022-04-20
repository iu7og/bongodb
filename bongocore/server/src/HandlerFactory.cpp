#include "HandlerFactory.h"

RequestHandler::RequestHandler(bongodb::Backend::TBackend* backend) {
    Backend = backend;
}

void RequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
    // Poco::Util::Application& app = Poco::Util::Application::instance();
    // app.logger().debug("Request from %s", request.clientAddress().toString());

    response.setChunkedTransferEncoding(true);
    response.setContentType("text/json");

    // TODO: write processing
    response.send() << Backend->GetMockResponse();
}

RequestHandlerFactory::RequestHandlerFactory(const Poco::Util::AbstractConfiguration& config) {
    Backend = std::make_unique<bongodb::Backend::TBackend>(config);
}

Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest&)
{
    return new RequestHandler(Backend.get());
}
