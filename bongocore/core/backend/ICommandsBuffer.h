#pragma once

#include <memory>

#include "common/OperationResults.h"
#include "common/StreamCommands.h"
#include "common/types.h"

namespace bongodb::Backend {
class ICommandsBuffer {
public:
    enum class EError : std::uint8_t { TooOld, TooNew };
    using TCommandResult = Common::TOperationResult<std::shared_ptr<Common::IStreamCommand>, EError>;
    virtual void Push(std::shared_ptr<Common::IStreamCommand> command) = 0;
    virtual TCommandResult GetByVersion(const Common::TVersion& version) = 0;
};
}  // namespace bongodb::Backend
