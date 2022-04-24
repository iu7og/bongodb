template<template<typename> typename TPtr>
THttpRequest::TStreamCommandAndVersion THttpRequest::ExtractStreamCommandAndVersion() {
    if (!Version || !StreamCommandType)
        throw std::runtime_error("trying to extract null version or stream command");
    TPtr<Common::IStreamCommand> streamCommand;
    switch (StreamCommandType.value()) {
        case Common::EStreamCommandType::Truncate:
            streamCommand = TPtr<Common::TTruncateStreamCommand>(new Common::TTruncateStreamCommand());
            break;
        case Common::EStreamCommandType::Remove:
            if (!Key)
                throw std::runtime_error("trying to extract null key");
            streamCommand = TPtr<Common::TRemoveStreamCommand>(new Common::TRemoveStreamCommand(std::move(Key.value())));
            break;
        case Common::EStreamCommandType::Put:
            if (!Key || !Value)
                throw std::runtime_error("trying to extract null key or value");
            streamCommand = TPtr<Common::TPutStreamCommand>(new Common::TPutStreamCommand(std::move(Key.value()), std::move(Value.value())));
            break;
    }

    return { streamCommand, std::move(Version.value()) };
}

template <typename TResult>
TResult THttpResponse::GetResult() {
    if (Error.has_value())
        return Error.value();
    else if (HttpStatus.has_value() && HttpStatus.value() != Poco::Net::HTTPResponse::HTTP_OK)
        return Common::EError::Other;

    if constexpr (std::is_same_v<Common::TVoidOperationResult, TResult>)
        return TResult();
    else
        return TResult(std::move(Value.value()));
}
