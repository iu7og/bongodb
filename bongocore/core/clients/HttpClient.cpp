#include "clients/HttpClient.h"
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include "common/OperationResults.h"
#include <sstream>
#include <iostream>

namespace bongodb::Clients {
THttpClient::THttpClient(const Poco::Util::AbstractConfiguration& config) : Session(config.getString("host"), config.getInt("port")) {
    Session.setKeepAlive(true);
    Session.setKeepAliveTimeout(Poco::Timespan(config.getInt("keep_alive_seconds", 3600)));
}

Common::TGetResult THttpClient::Get(const Common::TKey& key) {
    auto response = SendRequest(THttpRequest::GetRequest(Common::TKey(key)));
}

Common::TRemoveResult THttpClient::Remove(const Common::TKey& key) {
    
    return Common::EError::NotAvail;
}

Common::TTruncateResult THttpClient::Truncate() {
    return Common::EError::NotAvail;
}

Common::TPutResult THttpClient::Put(Common::TKey&& key, Common::TValue&& value) {
    return Common::EError::NotAvail;
}

Common::TStreamResult THttpClient::Stream(const Common::IStreamCommand& command, const Common::TVersion& version) {
    return Common::EError::NotAvail;
}

THttpResponse THttpClient::SendRequest(THttpRequest&& request) {
    try {
        auto [pocoRequest, body] = request.ToPocoHttpRequest();
        Session.sendRequest(pocoRequest) << body;
        Poco::Net::HTTPResponse response;
        std::istream& is = Session.receiveResponse(response);
        std::stringstream ss;
        Poco::StreamCopier::copyStream(is, ss);
        //if (response.getStatus() != Poco::Net::HTTPResponse::HTTP_OK)
        return THttpResponse(Common::EError::NotAvail);
    } catch (Poco::Exception &ex) {
        // TODO: заменить на логгер
        std::cout << ex.displayText() << std::endl;
        return THttpResponse(Common::EError::NotAvail);
    }
}

bool IsReady() {
    return true;
}

bool Prepare() {
    return true;
}

}  // namespace bongodb::Clients
