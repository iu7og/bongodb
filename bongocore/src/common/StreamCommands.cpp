#include "common/StreamCommands.h"

namespace bongodb::Common {
std::optional<TKey> TPutStreamCommand::GetKey() {
    return std::make_optional(Key);
}

std::optional<TKey> TPutStreamCommand::ExtractKey() {
    return std::make_optional(std::move(Key));
}

std::optional<TValue> TPutStreamCommand::GetValue() {
    return std::make_optional(Value);
}

std::optional<TValue> TPutStreamCommand::ExtractValue() {
    return std::make_optional(std::move(Value));
}

TRemoveStreamCommand::TRemoveStreamCommand(const TKey& key) : Key(key) {}
TRemoveStreamCommand::TRemoveStreamCommand(TKey&& key) : Key(std::move(key)) {}

TPutStreamCommand::TPutStreamCommand(const TKey& key, const TValue& value)
    : Key(key)
    , Value(value)
{
}

TPutStreamCommand::TPutStreamCommand(TKey&& key, TValue&& value)
    : Key(std::move(key))
    , Value(std::move(value))
{
}

std::optional<TKey> TRemoveStreamCommand::GetKey() {
    return std::make_optional(Key);
}

std::optional<TKey> TRemoveStreamCommand::ExtractKey() {
    return std::make_optional(std::move(Key));
}

EStreamCommandType TTruncateStreamCommand::GetType() { return EStreamCommandType::Truncate; }
EStreamCommandType TRemoveStreamCommand::GetType() { return EStreamCommandType::Remove; }
EStreamCommandType TPutStreamCommand::GetType() { return EStreamCommandType::Put; }

std::optional<TKey> IStreamCommand::GetKey() { return std::nullopt; }
std::optional<TKey> IStreamCommand::ExtractKey() { return std::nullopt; }
std::optional<TValue> IStreamCommand::GetValue() { return std::nullopt; }
std::optional<TValue> IStreamCommand::ExtractValue() { return std::nullopt; }
} // namespace bongodb::Common
