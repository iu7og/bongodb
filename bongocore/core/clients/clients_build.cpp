#include "clients/HttpClient.h"
#include "clients/build.h"

namespace bongodb::Clients {
std::unique_ptr<IClient> buildClient(const Poco::Util::AbstractConfiguration& config) {
    return std::make_unique<THttpClient>(config);
}
}  // namespace bongodb::Clients
