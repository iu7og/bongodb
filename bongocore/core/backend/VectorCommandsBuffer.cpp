#include "backend/VectorCommandsBuffer.h"

namespace bongodb::Backend {
void VectorCommandsBuffer::Push(std::shared_ptr<Common::IStreamCommand> command) { Buffer.push_back(command); }

VectorCommandsBuffer::TCommandResult VectorCommandsBuffer::GetByVersion(const Common::TVersion& version) {
    if (version <= 0) {
        poco_error_f1(Logger, "Version can't be <= 0, got = %lu", version);

        throw std::invalid_argument("VectorCommandsBuffer version can't be <= 0");
    }
    if (version > Buffer.size()) {
        poco_warning_f1(Logger, "Requested a version that is too new, requested version is = %lu", version);

        return EError::TooNew;
    }

    return Buffer[version - 1];
}

}  // namespace bongodb::Backend
