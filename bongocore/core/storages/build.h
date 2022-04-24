#pragma once

#include <Poco/Util/AbstractConfiguration.h>
#include <memory>

#include "storages/IStorage.h"
#include "storages/StdHashStorage.h"
//#include "storages/BongoStorage.h"
#include "storages/ThreadSafeStorageWrapper.h"

namespace bongodb::DB {
template<template<typename> typename TPtr = std::shared_ptr>
TPtr<IStorage> buildStorage(const Poco::Util::AbstractConfiguration& config) {
    std::unique_ptr<IStorage> storage;

    auto storageType = config.getString("type", "std");
    if (storageType == "std")
        storage = std::make_unique<TStdHashStorage>();
    //else if (storageType == "bongo")
        //storage = std::make_unique<TBongoStorage>();
    else
        throw std::runtime_error("Unknown storage type");

    return TPtr<TThreadSafeStorageWrapper>(new TThreadSafeStorageWrapper(std::move(storage)));
}
}  // namespace bongodb::DB
