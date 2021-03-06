#include "clients/HttpClient.h"

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>

#include <iostream>
#include <sstream>

#include "common/OperationResults.h"

namespace bongodb::Clients {
THttpClient::THttpClient(const Poco::Util::AbstractConfiguration& config)
    : Host(config.getString("host")), Port(config.getInt("port")), Session(Host, Port) {
    Session.setKeepAlive(true);
    Session.setKeepAliveTimeout(Poco::Timespan(config.getInt("keep_alive_seconds", 3600)));
}

Common::TGetResult THttpClient::Get(const Common::TKey& key) {
    auto response = SendRequest(THttpRequest::GetRequest(Common::TKey(key)));
    return response.GetResult<Common::TGetResult>();
}

Common::TRemoveResult THttpClient::Remove(const Common::TKey& key) {
    auto response = SendRequest(THttpRequest::DeleteRequest(Common::TKey(key)));
    return response.GetResult<Common::TRemoveResult>();
}

Common::TTruncateResult THttpClient::Truncate() {
    auto response = SendRequest(THttpRequest::TruncateRequest());
    return response.GetResult<Common::TTruncateResult>();
}

Common::TPutResult THttpClient::Put(Common::TKey&& key, Common::TValue&& value) {
    auto response = SendRequest(THttpRequest::PutRequest(std::move(key), std::move(value)));
    return response.GetResult<Common::TPutResult>();
}

Common::TStreamResult THttpClient::Stream(const Common::IStreamCommand& command, const Common::TVersion& version) {
    auto response = SendRequest(THttpRequest::StreamRequest(command, version));
    return response.GetResult<Common::TPutResult>();
}

THttpResponse THttpClient::SendRequest(THttpRequest&& request) {
    try {
        auto [pathAndMethod, body] = request.ToPocoHttpRequest();
        auto pocoRequest =
            Poco::Net::HTTPRequest(pathAndMethod.first, pathAndMethod.second, Poco::Net::HTTPRequest::HTTP_1_1);
        pocoRequest.setContentLength(body.size());
        Session.sendRequest(pocoRequest) << body;

        Poco::Net::HTTPResponse response;
        std::stringstream ss;
        {
            // std::lock_guard<std::mutex> lock(Mutex);
            std::istream& is = Session.receiveResponse(response);
            Poco::StreamCopier::copyStream(is, ss);
        }

        return response.getStatus() != Poco::Net::HTTPResponse::HTTP_OK
                   ? THttpResponse(Common::EError::NotAvail, response.getStatus())
                   : THttpResponse(ss.str());
    } catch (Poco::Exception& ex) {
        poco_warning_f3(Logger, "SendRequest exception: %s (%s:%d)", std::string(ex.what()), Host, Port);
    }
    return THttpResponse(Common::EError::NotAvail);
}

bool THttpClient::IsReady() { return true; }

bool THttpClient::Prepare() { return true; }

}  // namespace bongodb::Clients
