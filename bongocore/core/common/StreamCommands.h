#pragma once
#include <optional>
#include <string>

#include "common/types.h"

namespace bongodb::Common {
enum class EStreamCommandType { Truncate, Put, Remove };

/**
 * @brief Класс IStreamCommad.
 */
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
    /**
     * @brief Конструктор.
     *
     * @param Key Ключ.
     * @param value значение.
     */
    TPutStreamCommand(const TKey& Key, const TValue& value);
    /**
     * @brief Константный конструктор.
     *
     * @param Key Ключ.
     * @param value значение.
     */
    TPutStreamCommand(TKey&& Key, TValue&& value);

    /**
     * @brief Получить ключ.
     *
     * @return Ключ.
     */
    std::optional<TKey> GetKey() const override;
    /**
     * @brief Достать ключ.
     *
     * @return Ключ.
     */
    std::optional<TKey> ExtractKey() override;
    /**
     * @brief Получить значение.
     *
     * @return Значение.
     */
    std::optional<TValue> GetValue() const override;
    /**
     * @brief Достать значение.
     *
     * @return Значение.
     */
    std::optional<TValue> ExtractValue() override;
    /**
     * @brief Получить тип.
     *
     * @return Stream-тип команды.
     */
    EStreamCommandType GetType() const override;

private:
    TKey Key;
    TValue Value;
};
}  // namespace bongodb::Common
