#pragma once

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include <memory>
#include <sstream>
#include <type_traits>
#include <variant>

#include "common/OperationResults.h"
#include "common/StreamCommands.h"
#include "common/types.h"

namespace bongodb::Clients {
enum class EOperationType { Put = 0, Get, Delete, Truncate, Stream };

class THttpRequest {
public:
    using TStreamCommandAndVersion = std::pair<std::shared_ptr<Common::IStreamCommand>, Common::TVersion>;
    using TPath = std::string;
    using TMethod = decltype(Poco::Net::HTTPRequest::HTTP_GET);
    using TBody = std::string;
    using TPocoRequestData = std::pair<std::pair<TPath, TMethod>, TBody>;

    static THttpRequest PutRequest(Common::TKey&& key, Common::TValue&& value);
    static THttpRequest GetRequest(Common::TKey&& key);
    static THttpRequest DeleteRequest(Common::TKey&& key);
    static THttpRequest TruncateRequest();
    static THttpRequest StreamRequest(const Common::IStreamCommand& streamCommand, const Common::TVersion& version);

    static std::pair<TPath, TMethod> GetPathAndMethod(EOperationType operationType, const Common::TKey& key = "");
    TPocoRequestData ToPocoHttpRequest();
    static THttpRequest FromPocoHttpRequest(const Poco::Net::HTTPRequest& request, const TBody& body);

    EOperationType GetType();
    Common::TKey ExtractKey();
    Common::TValue ExtractValue();
    TStreamCommandAndVersion ExtractStreamCommandAndVersion();

private:
    THttpRequest(EOperationType operationType, std::optional<Common::TKey>&& key = std::nullopt,
                 std::optional<Common::TValue>&& value = std::nullopt,
                 std::optional<Common::TVersion>&& version = std::nullopt,
                 std::optional<Common::EStreamCommandType>&& streamCommandType = std::nullopt)
        : OperationType(operationType),
          Key(key),
          Value(value),
          Version(version),
          StreamCommandType(streamCommandType) {}

    EOperationType OperationType;
    std::optional<Common::TKey> Key;
    std::optional<Common::TValue> Value;
    std::optional<Common::TVersion> Version;
    std::optional<Common::EStreamCommandType> StreamCommandType;
};

struct THttpResponse {
    using TError = Common::EError;
    using THttpStatus = decltype(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);

    std::optional<TError> Error = std::nullopt;
    std::optional<Common::TValue> Value = std::nullopt;
    std::optional<THttpStatus> HttpStatus = std::nullopt;

    THttpResponse(const std::string& data);
    THttpResponse(TError error, std::optional<THttpStatus> status = std::nullopt);
    THttpResponse(Common::TValue&& value);

    template <typename TResult>
    TResult GetResult() {
        if (Error.has_value())
            return Error.value();
        else if (HttpStatus.has_value() && HttpStatus.value() != Poco::Net::HTTPResponse::HTTP_OK)
            return Common::EError::Other;

        if constexpr (std::is_same_v<Common::TVoidOperationResult, TResult>)
            return TResult();
        else
            return TResult(std::move(Value.value()));
    }
};

}  // namespace bongodb::Clients
