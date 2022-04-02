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
