#pragma once

#include <Poco/Logger.h>

#include <stdexcept>
#include <vector>

#include "backend/ICommandsBuffer.h"

namespace bongodb::Backend {
/**
 * @brief Класс вектор буффер команд.
 */
class TVectorCommandsBuffer : public ICommandsBuffer {
public:
    using EError = ICommandsBuffer::EError;
    /**
     * @brief Добавляет команду и задает ей версию.
     * @param command Команда.
     */
    virtual void Push(std::shared_ptr<Common::IStreamCommand> command) override;
    /**
     * @brief Возвращает указатель на указанную версию команды.
     * @param version Уникальная для каждой команды версии.
     * @return Возвращает результат выполнения запроса.
     */
    virtual TCommandResult GetByVersion(const Common::TVersion& version) override;

private:
    using TBuffer = std::vector<std::shared_ptr<Common::IStreamCommand>>;
    TBuffer Buffer;

    Poco::Logger& Logger = Poco::Logger::get("BackendLogger");
};
}  // namespace bongodb::Backend
