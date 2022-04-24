#pragma once

#include <Poco/Logger.h>

#include <stdexcept>
#include <vector>

#include "backend/ICommandsBuffer.h"

namespace bongodb::Backend {
class TVectorCommandsBuffer : public ICommandsBuffer {
public:
    using EError = ICommandsBuffer::EError;
    virtual void Push(std::shared_ptr<Common::IStreamCommand> command) override;
    virtual TCommandResult GetByVersion(const Common::TVersion& version) override;

private:
    using TBuffer = std::vector<std::shared_ptr<Common::IStreamCommand>>;
    TBuffer Buffer;

    Poco::Logger& Logger = Poco::Logger::get("BackendLogger");
};
}  // namespace bongodb::Backend
