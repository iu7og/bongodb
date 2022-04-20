#include "backend/build.h"

namespace bongodb::Backend {
std::unique_ptr<IProcessor> buildProcessor(const Poco::Util::AbstractConfiguration& config, const Common::TShards& shards) {
    // TODO: implement
    return std::unique_ptr<IProcessor>();
}

std::shared_ptr<ICommandsBuffer> buildCommandsBuffer(const Poco::Util::AbstractConfiguration& config) {
    // TODO: implement
    return std::shared_ptr<ICommandsBuffer>();
}
} // namespace bongodb::Backend
