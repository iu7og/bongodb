#include "clients/HttpPrimitives.h"
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include "common/StreamCommands.h"

namespace bongodb::Clients {

THttpRequest THttpRequest::GetRequest(Common::TKey&& key) {
    return THttpRequest(EOperationType::Get, std::make_optional(std::move(key)));
}

THttpRequest THttpRequest::PutRequest(Common::TKey&& key, Common::TValue&& value) {
    return THttpRequest(EOperationType::Put, std::make_optional(std::move(key)), std::make_optional(std::move(value)));
}

THttpRequest THttpRequest::DeleteRequest(Common::TKey&& key) {
    return THttpRequest(EOperationType::Delete, std::make_optional(std::move(key)));
}

THttpRequest THttpRequest::TruncateRequest() {
    return THttpRequest(EOperationType::Truncate);
}

THttpRequest THttpRequest::StreamRequest(const Common::IStreamCommand& streamCommand, const Common::TVersion& version) {
    auto streamCommandType = std::optional{ streamCommand.GetType() };
    auto key = streamCommand.GetKey();
    auto value = streamCommand.GetValue();
    return THttpRequest(EOperationType::Stream, std::move(key), std::move(value), std::move(version), std::move(streamCommandType));
}

THttpRequest THttpRequest::FromPocoHttpRequest(const Poco::Net::HTTPRequest& request, const THttpRequest::TBody& body) {
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var var = parser.parse(body);
    Poco::JSON::Object::Ptr jsonObject = var.extract<Poco::JSON::Object::Ptr>();

    // TODO: возможно следует добавить валидацию на наличие нужных значений
    auto operationType = EOperationType(jsonObject->get("operation_type").convert<int>());
    auto key = jsonObject->has("key") ? std::optional{ jsonObject->get("key").convert<Common::TKey>() } : std::nullopt;
    auto value = jsonObject->has("value") ? std::optional{ jsonObject->get("value").convert<Common::TValue>() } : std::nullopt;
    auto version = jsonObject->has("version") ? std::optional{ jsonObject->get("version").convert<Common::TVersion>() } : std::nullopt;
    auto streamCommandType = jsonObject->has("stream_command_type") ? std::optional{ Common::EStreamCommandType(jsonObject->get("stream_command_type").convert<int>()) } : std::nullopt;
    return THttpRequest(operationType, std::move(key), std::move(value), std::move(version), std::move(streamCommandType));
}

THttpRequest::TPocoRequestData THttpRequest::ToPocoHttpRequest() {
    Poco::JSON::Object obj;
    obj.set("operation_type", static_cast<int>(OperationType));
    if (Key)
        obj.set("key", Key.value());
    if (Value)
        obj.set("value", Value.value());
    if (Version)
        obj.set("version", Version.value());
    if (StreamCommandType)
        obj.set("stream_command_type", StreamCommandType.value());

    std::ostringstream bodyStream;
    obj.stringify(bodyStream);
    TBody body = std::move(bodyStream).str();

    return TPocoRequestData(GetPathAndMethod(OperationType), body);
}

EOperationType THttpRequest::GetType() {
    return OperationType;
}

Common::TKey THttpRequest::ExtractKey() {
    if (!Key)
        throw std::runtime_error("trying to extract null key");
    return std::move(Key.value());
}

Common::TValue THttpRequest::ExtractValue() {
    if (!Value)
        throw std::runtime_error("trying to extract null value");
    return std::move(Value.value());
}

std::pair<THttpRequest::TPath, THttpRequest::TMethod> THttpRequest::GetPathAndMethod(EOperationType operationType, const Common::TKey& key) {
    switch(operationType) {
        case EOperationType::Get: return {  "/" + key, Poco::Net::HTTPRequest::HTTP_GET};
        case EOperationType::Put: return {  "/", Poco::Net::HTTPRequest::HTTP_POST};
        case EOperationType::Delete: return {  "/", Poco::Net::HTTPRequest::HTTP_DELETE};
        case EOperationType::Truncate: return {  "/", Poco::Net::HTTPRequest::HTTP_POST};
        case EOperationType::Stream: return {  "/stream", Poco::Net::HTTPRequest::HTTP_POST};
        default: throw std::runtime_error("wrong operation type");
    }
}

THttpResponse::THttpResponse(const std::string& data) {
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var var = parser.parse(data);
    Poco::JSON::Object::Ptr jsonObject = var.extract<Poco::JSON::Object::Ptr>();

    if (jsonObject->has("error"))
        Error = std::optional{ TError(jsonObject->get("error").convert<int>()) };
    else if (jsonObject->has("value"))
        Value = std::optional{ jsonObject->get("value").convert<std::string>() };
}

THttpResponse::THttpResponse(TError error, std::optional<THttpStatus> status) : Error(error), HttpStatus(status)
{
}

THttpResponse::THttpResponse(Common::TValue&& value) : Value(std::move(value))
{
}

THttpResponse::TBody THttpResponse::GetBody() {
    Poco::JSON::Object obj;
    if (Error)
        obj.set("error", static_cast<int>(Error.value()));
    if (Value)
        obj.set("value", Value.value());

    std::ostringstream bodyStream;
    obj.stringify(bodyStream);
    return std::move(bodyStream).str();
}

THttpResponse::THttpStatus THttpResponse::GetStatus() {
    if (HttpStatus)
        return HttpStatus.value();
    else if (Error)
        return Poco::Net::HTTPResponse::HTTPStatus::HTTP_BAD_REQUEST;
    return Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK;

}  // namespace bongodb::Clients
