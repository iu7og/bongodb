#pragma once

#include <Poco/Util/AbstractConfiguration.h>
#include <memory>

#include "storages/IStorage.h"

namespace bongodb::DB {
template<template<typename> typename TPtr = std::shared_ptr>
TPtr<IStorage> buildStorage(const Poco::Util::AbstractConfiguration& config) {
    return TPtr<IStorage>();
}
}  // namespace bongodb::DB
