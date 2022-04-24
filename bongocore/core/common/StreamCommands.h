#pragma once
#include <optional>
#include <string>

#include "common/types.h"

namespace bongodb::Common {
enum class EStreamCommandType { Truncate, Put, Remove };

class IStreamCommand {
public:
    virtual std::optional<TKey> GetKey() const;
    virtual std::optional<TKey> ExtractKey();
    virtual std::optional<TValue> GetValue() const;
    virtual std::optional<TValue> ExtractValue();
    virtual EStreamCommandType GetType() const = 0;
};

class TTruncateStreamCommand : public IStreamCommand {
public:
    TTruncateStreamCommand() = default;
    EStreamCommandType GetType() const override;
};

class TRemoveStreamCommand : public IStreamCommand {
public:
    TRemoveStreamCommand(const TKey& Key);
    TRemoveStreamCommand(TKey&& Key);
    std::optional<TKey> GetKey() const override;
    std::optional<TKey> ExtractKey() override;
    EStreamCommandType GetType() const override;

private:
    TKey Key;
};

class TPutStreamCommand : public IStreamCommand {
public:
    TPutStreamCommand(const TKey& Key, const TValue& value);
    TPutStreamCommand(TKey&& Key, TValue&& value);
    std::optional<TKey> GetKey() const override;
    std::optional<TKey> ExtractKey() override;
    std::optional<TValue> GetValue() const override;
    std::optional<TValue> ExtractValue() override;
    EStreamCommandType GetType() const override;

private:
    TKey Key;
    TValue Value;
};
}  // namespace bongodb::Common
