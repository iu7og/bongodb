#pragma once

#include <variant>

#include "common/types.h"

namespace bongodb::Common {
enum class EError : std::uint8_t { NotFound = 1, NotAvail, Other };

template <typename TResultValue, typename TError = EError>
/**
 * @brief Класс описывающий результат выполнения операций и работы с данными.
 */
class TOperationResult {
public:
    using TInnerValue = std::variant<TResultValue, TError>;
    static constexpr auto OK_VALUE_INDEX = 0;

    TOperationResult(TError error) : InnerValue(error) {}
    TOperationResult(TResultValue&& value) : InnerValue(value) {}
    TOperationResult(const TResultValue& value) : InnerValue(value) {}

    bool IsOk() const { return InnerValue.index() == OK_VALUE_INDEX; }
    TResultValue ExtractValue() { return std::move(std::get<TResultValue>(InnerValue)); }
    TError GetError() { return std::get<TError>(InnerValue); }

private:
    TInnerValue InnerValue;
};

/**
 * @brief Класс описывающий void результат выполнения операций и работы с данными.
 */
class TVoidOperationResult {
public:
    using TInnerValue = std::optional<EError>;
    /**
     * @brief Конструктор.
     *
     * @param error Ошибка.
     */
    TVoidOperationResult(EError error) : InnerValue(error) {}
    /**
     * @brief Конструктор.
     */
    TVoidOperationResult() : InnerValue(std::nullopt) {}

    /**
     * @brief Проверка InnerValue на корректность.
     * @return Возвращает true / false.
     */
    bool IsOk() const { return InnerValue == std::nullopt; }
    /**
     * @brief Получить ошибку.
     * @return Возвращает ошибку.
     */
    EError GetError() { return InnerValue.value(); }

private:
    TInnerValue InnerValue;
};

using TGetResult = TOperationResult<TValue>;
using TPutResult = TVoidOperationResult;
using TTruncateResult = TVoidOperationResult;
using TRemoveResult = TVoidOperationResult;
using TStreamResult = TVoidOperationResult;
}  // namespace bongodb::Common
