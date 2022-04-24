#pragma once

#include <Poco/Logger.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Util/AbstractConfiguration.h>

#include "clients/HttpPrimitives.h"
#include "clients/IClient.h"

namespace bongodb::Clients {
class THttpClient : public IClient {
public:
    THttpClient(const Poco::Util::AbstractConfiguration& config);

    Common::TGetResult Get(const Common::TKey& key) override;
    Common::TRemoveResult Remove(const Common::TKey& key) override;
    Common::TTruncateResult Truncate() override;
    Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value) override;
    Common::TStreamResult Stream(const Common::IStreamCommand& command, const Common::TVersion& version) override;

    bool IsReady() override;
    bool Prepare() override;

private:
    THttpResponse SendRequest(THttpRequest&& request);

    Poco::Net::HTTPClientSession Session;

    Poco::Logger& Logger = Poco::Logger::get("BackendLogger");
};
}  // namespace bongodb::Clients
