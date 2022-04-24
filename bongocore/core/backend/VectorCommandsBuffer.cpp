#include "backend/VectorCommandsBuffer.h"

namespace bongodb::Backend {
void VectorCommandsBuffer::Push(std::shared_ptr<Common::IStreamCommand> command) { Buffer.push_back(command); }

VectorCommandsBuffer::TCommandResult VectorCommandsBuffer::GetByVersion(const Common::TVersion& version) {
    if (version <= 0) {
        throw std::invalid_argument("VectorCommandsBuffer version can't be <= 0");
    }
    if (version > Buffer.size()) {
        return EError::TooNew;
    }

    return Buffer[version - 1];
}

}  // namespace bongodb::Backend
