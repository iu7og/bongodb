cmake_minimum_required(VERSION 3.5)
project(bongo)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Poco REQUIRED Net Util)
# find_package(tsl-robin-map REQUIRED)

include_directories(
    core/
    server/src
)

set (LIB_FILES
    core/backend/Backend.cpp
    core/backend/Backend.h
    core/backend/backend_build.cpp
    core/backend/build.h
    core/backend/ICommandsBuffer.h
    core/backend/VectorCommandsBuffer.h
    core/backend/VectorCommandsBuffer.cpp
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
    server/src/HandlerFactory.cpp
    server/src/HandlerFactory.h
    core/storages/BongoStorage.h
    core/storages/BongoStorage.cpp
)

add_library(bongodbcore STATIC ${LIB_FILES})

set (SERVER_MAIN
    server/src/main.cpp
)

set (TESTS_MAIN
    tests/src/main.cpp
)

set (POCO_NEEDED
    Poco::Util
    Poco::Net
)

add_executable(${CMAKE_PROJECT_NAME} ${SERVER_MAIN})

add_executable(test ${TESTS_MAIN})

target_link_libraries(${CMAKE_PROJECT_NAME} bongodbcore ${POCO_NEEDED})
target_link_libraries(test bongodbcore ${POCO_NEEDED})
