#pragma once
#include <string>
#include <optional>
#include "common/types.h"

namespace bongodb::Common {
enum class EStreamCommandType {
    Truncate,
    Put,
    Remove
};

class IStreamCommand {
public:
    virtual std::optional<TKey> GetKey();
    virtual std::optional<TKey> ExtractKey();
    virtual std::optional<TValue> GetValue();
    virtual std::optional<TValue> ExtractValue();
    virtual EStreamCommandType GetType() = 0;
};

class TTruncateStreamCommand : public IStreamCommand {
public:
    TTruncateStreamCommand() = default;
    EStreamCommandType GetType() override;
};

class TRemoveStreamCommand : public IStreamCommand {
public:
    TRemoveStreamCommand(const TKey& Key);
    TRemoveStreamCommand(TKey&& Key);
    std::optional<TKey> GetKey() override;
    std::optional<TKey> ExtractKey() override;
    EStreamCommandType GetType() override;
private:
    TKey Key;
};

class TPutStreamCommand : public IStreamCommand {
public:
    TPutStreamCommand(const TKey& Key, const TValue& value);
    TPutStreamCommand(TKey&& Key, TValue&& value);
    std::optional<TKey> GetKey() override;
    std::optional<TKey> ExtractKey() override;
    std::optional<TValue> GetValue() override;
    std::optional<TValue> ExtractValue() override;
    EStreamCommandType GetType() override;
private:
    TKey Key;
    TValue Value;
};
} // namespace bongodb::Common
