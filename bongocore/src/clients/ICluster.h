#pragma once
#include "IClient.h"

namespace bongodb::Clients {
// NOTE: currently imo ICluster should have the same interface as IClient,
// but it can change in the future, so I prefer to separate ICluster base class
class ICluster : public IClient {};
}  // namespace bongodb::Clients
