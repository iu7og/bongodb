#pragma once

#include <variant>
#include "common/types.h"

namespace bongodb::Common {
enum class EError : std::uint8_t {
    NotFound,
    NotAvail,
    Other
};

template <typename TResultValue>
class TOperationResult {
public:
    using TInnerValue = std::variant<TResultValue, EError>;
    static constexpr auto OK_VALUE_INDEX = 0;

    TOperationResult(EError error) : InnerValue(error) {}
    TOperationResult(TResultValue&& value) : InnerValue(value) {}
    TOperationResult(const TResultValue& value) : InnerValue(value) {}

    bool IsOk() const { return InnerValue.index() == OK_VALUE_INDEX; }
    TResultValue ExtractValue() { return std::move(std::get<TResultValue>(InnerValue)); }
    EError GetError() { return std::get<EError>(InnerValue); }
private:
     TInnerValue InnerValue;
};

class TVoidOperationResult {
public:
    using TInnerValue = std::optional<EError>;

    TVoidOperationResult(EError error) : InnerValue(error) {}
    TVoidOperationResult() : InnerValue(std::nullopt) {}

    bool IsOk() const { return InnerValue == std::nullopt; }
    EError GetError() { return InnerValue.value(); }
private:
     TInnerValue InnerValue;
};

using TGetResult = TOperationResult<TValue>;
using TPutResult = TVoidOperationResult;
using TTruncateResult = TVoidOperationResult;
using TRemoveResult = TVoidOperationResult;
} // namespace bongodb::Common
