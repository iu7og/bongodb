# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

cmake_minimum_required(VERSION 3.5)
project(bongo)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Poco REQUIRED Net Util)
find_package(tsl-robin-map REQUIRED)

include_directories(
    core/
    server/src
)

set (SRCS
    core/backend/Backend.cpp
    core/backend/Backend.h
    core/backend/backend_build.cpp
    core/backend/build.h
    core/backend/ICommandsBuffer.h
    core/backend/IProcessor.h
    core/backend/MasterProcessor.cpp
    core/backend/MasterProcessor.h
    core/backend/ReplicaProcessor.cpp
    core/backend/ReplicaProcessor.h
    core/backend/Streamer.cpp
    core/backend/Streamer.h
    core/clients/build.h
    core/clients/clients_build.cpp
    core/clients/HttpClient.cpp
    core/clients/HttpClient.h
    core/clients/HttpPrimitives.cpp
    core/clients/HttpPrimitives.h
    core/clients/IClient.h
    core/common/build.h
    core/common/common_build.cpp
    core/common/OperationResults.h
    core/common/Shard.h
    core/common/StreamCommands.cpp
    core/common/StreamCommands.h
    core/common/types.h
    core/storages/build.h
    core/storages/IStorage.h
    core/storages/StdHashStorage.cpp
    core/storages/StdHashStorage.h
    core/storages/storages_build.cpp
    core/storages/ThreadSafeStorageWrapper.cpp
    core/storages/ThreadSafeStorageWrapper.h
    server/examples/config.xml
    server/src/HandlerFactory.cpp
    server/src/HandlerFactory.h
    server/src/main.cpp
    core/storages/BongoStorage.h
    core/storages/BongoStorage.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC Poco::Util Poco::Net)
