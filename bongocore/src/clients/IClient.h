#pragma once

#include <variant>

#include "common/types.h"
#include "common/StreamCommands.h"

namespace bongodb::Clients {
class IClient {
public:
    enum class EError : std::uint8_t {
        NotFound,
        NotAvail,
        Other
    };

    class TGetResponse;

    /// Protocol procedures
    /// 1. proxy request
    virtual TGetResponse Get(const Common::TKey& key) = 0;
    virtual bool Remove(const Common::TKey& key) = 0;
    virtual void Truncate() = 0;
    virtual bool Put(Common::TKey&& key, Common::TValue&& value) = 0;
    /// 2. stream data to replicas
    virtual bool Stream(const Common::IStreamCommand&& command, Common::TVersion&& version) = 0;

    virtual bool IsReady() = 0;
    virtual bool Prepare() = 0;

    class TGetResponse {
    public:
        using TInnerValue = std::variant<Common::TValue, EError>;
        static constexpr auto OK_VALUE_INDEX = 0;

        TGetResponse(EError error) : InnerValue(error) {}
        TGetResponse(const Common::TValue& value) : InnerValue(value) {}
        TGetResponse(Common::TValue&& value) : InnerValue(value) {}

        bool IsOk() { return InnerValue.index() == OK_VALUE_INDEX; }
        Common::TValue ExtractValue() { return std::move(std::get<Common::TValue>(InnerValue)); }
        EError GetError() { return std::get<EError>(InnerValue); }
    private:
         TInnerValue InnerValue;
    };
};
} // namespace bongodb::Clients
