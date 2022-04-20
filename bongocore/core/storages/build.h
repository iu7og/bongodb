#pragma once

#include <Poco/Util/AbstractConfiguration.h>
#include <memory>

#include "storages/IStorage.h"
#include "storages/StdHashStorage.h"
#include "storages/ThreadSafeStorageWrapper.h"

namespace bongodb::DB {
template<template<typename> typename TPtr = std::shared_ptr>
TPtr<IStorage> buildStorage(const Poco::Util::AbstractConfiguration&) {
    return TPtr<TThreadSafeStorageWrapper>(new TThreadSafeStorageWrapper(std::make_unique<TStdHashStorage>()));
}
}  // namespace bongodb::DB
