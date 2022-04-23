#pragma once

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Util/AbstractConfiguration.h>

#include "clients/IClient.h"
#include "clients/HttpPrimitives.h"

namespace bongodb::Clients {
class THttpClient : public IClient {
public:
    THttpClient(const Poco::Util::AbstractConfiguration& config);
    /// Protocol procedures
    /// 1. proxy request
    Common::TGetResult Get(const Common::TKey& key) override;
    Common::TRemoveResult Remove(const Common::TKey& key) override;
    Common::TTruncateResult Truncate() override;
    Common::TPutResult Put(Common::TKey&& key, Common::TValue&& value) override;
    /// 2. stream data to replicas (maybe some kind of `handle` should be returned)
    Common::TStreamResult Stream(const Common::IStreamCommand& command, const Common::TVersion& version) override;

    bool IsReady() override;
    bool Prepare() override;
private:
    THttpResponse SendRequest(THttpRequest&& request);

    Poco::Net::HTTPClientSession Session;
};
}  // namespace bongodb::Clients
