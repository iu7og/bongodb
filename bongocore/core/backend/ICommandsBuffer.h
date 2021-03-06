#pragma once

#include <memory>

#include "common/OperationResults.h"
#include "common/StreamCommands.h"
#include "common/types.h"

namespace bongodb::Backend {
/**
 * @brief Абстрактный класс для реализации буффер команд.
 */
class ICommandsBuffer {
public:
    enum class EError : std::uint8_t { TooOld, TooNew };
    using TCommandResult = Common::TOperationResult<std::shared_ptr<Common::IStreamCommand>, EError>;
    /**
     * @brief Виртуальный метод Push.
     * @param command Команда.
     */
    virtual void Push(std::shared_ptr<Common::IStreamCommand> command) = 0;
    /**
     * @brief Получение результата взависимости от версии.
     * @param version версия.
     * @return Возвращает результат выполнения запроса.
     */
    virtual TCommandResult GetByVersion(const Common::TVersion& version) = 0;
};
}  // namespace bongodb::Backend
