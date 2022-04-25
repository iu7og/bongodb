#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/Util/ServerApplication.h>
#include <backend/VectorCommandsBuffer.h>

#include <iostream>

#include "HandlerFactory.h"
#include "backend/Backend.h"

class WebServerApp : public Poco::Util::ServerApplication {
    void handleConfig(const std::string&, const std::string& value) { ConfigPath = value; }

    void defineOptions(Poco::Util::OptionSet& options) {
        Application::defineOptions(options);
        options.addOption(Poco::Util::Option("config", "c", "load configuration data from a file")
                              .required(true)
                              .repeatable(false)
                              .argument("config")
                              .callback(Poco::Util::OptionCallback<WebServerApp>(this, &WebServerApp::handleConfig)));
    }

    void initialize(Application& self) {
        loadConfiguration(ConfigPath);
        ServerApplication::initialize(self);
    }

    int main(const std::vector<std::string>&) {
        Poco::UInt16 port = static_cast<Poco::UInt16>(config().getUInt("server.port", 1234));

        Poco::Net::HTTPServerParams::Ptr parameters = new Poco::Net::HTTPServerParams();
        parameters->setMaxQueued(config().getInt("server.max_queued", 64));

        Poco::ThreadPool threadPool(config().getInt("server.min_threads", 2),
                                    config().getInt("server.max_threads", 16));
        Poco::Net::ServerSocket socket(port);
        Poco::Net::HTTPServer srv(new RequestHandlerFactory(*config().createView("backend")), threadPool, socket,
                                  parameters);
        srv.start();
        logger().information("HTTP Server started on port %hu.", port);
        waitForTerminationRequest();
        logger().information("Stopping HTTP Server...");
        srv.stop();

        return Application::EXIT_OK;
    }

    std::string ConfigPath;
};

POCO_SERVER_MAIN(WebServerApp)
