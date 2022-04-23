#include "clients/build.h"
#include "clients/HttpClient.h"

namespace bongodb::Clients {
std::unique_ptr<IClient> buildClient(const Poco::Util::AbstractConfiguration& config) {
    return std::make_unique<THttpClient>(config);
}
} // namespace bongodb::Clients
