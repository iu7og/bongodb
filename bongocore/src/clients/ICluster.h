#pragma once
#include "IClient.h"

namespace bongodb::Clients {
class ICluster : public IClient {
    using IClient::EError;
    using IClient::TGetResponse;
};
} // namespace bongodb::Clients
