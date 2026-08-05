cmake_minimum_required(VERSION 3.16)

project(libftpp
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(libftpp
 srcs/DataStructure/DataBuffer/data_buffer.cpp
 srcs/DesignPatterns/Memento/memento.cpp
 srcs/IOStream/ThreadSafeIOStream/thread_safe_iostream.cpp
 srcs/Thread/Thread/thread.cpp
 srcs/Thread/WorkerPool/worker_pool.cpp
 srcs/Thread/PersistentWorker/persistent_worker.cpp
)

target_include_directories(
    libftpp
    PUBLIC ${PROJECT_SOURCE_DIR}/includes
)

#DataStructure TESTS
add_executable(
    DataBufferTest
    tests/DataStructure/data_buffer_test.cpp
)

target_link_libraries(
    DataBufferTest
    PRIVATE libftpp
)

add_executable(
    PoolTest
    tests/DataStructure/pool_test.cpp
)

target_link_libraries(
    PoolTest
    PRIVATE libftpp
)

#DesignPatterns TESTS
add_executable(
    MementoTest
    tests/DesignPatterns/memento_test.cpp
)

target_link_libraries(
    MementoTest
    PRIVATE    libftpp
)

add_executable(
    ObserverTest
    tests/DesignPatterns/observer_test.cpp
)

target_link_libraries(
    ObserverTest
    PRIVATE libftpp
)

add_executable(
    SingletonTest
    tests/DesignPatterns/singleton_test.cpp
)

target_link_libraries(
    SingletonTest
    PRIVATE libftpp
)

add_executable(
    StateMachineTest
    tests/DesignPatterns/state_machine_test.cpp
)

target_link_libraries(
    StateMachineTest
    PRIVATE libftpp
)

#Threads TEST
add_executable(
    ThreadSafeIOStreamTest
    tests/IOStream/thread_safe_iostream_test.cpp
)

target_link_libraries(
    ThreadSafeIOStreamTest
    PRIVATE libftpp
)

add_executable(
    ThreadSafeQueueTest
    tests/Thread/thread_safe_queue_test.cpp
)

target_link_libraries(
    ThreadSafeQueueTest
    PRIVATE libftpp
)

add_executable(
    ThreadTest
    tests/Thread/thread_test.cpp
)

target_link_libraries(
    ThreadTest
    PRIVATE libftpp
)

add_executable(
    WorkerPoolTest
    tests/Thread/worker_pool_test.cpp
)

target_link_libraries(
    WorkerPoolTest
    PRIVATE libftpp
)

add_executable(
    PersistentWorkerTest
    tests/Thread/persistent_worker_test.cpp
)

target_link_libraries(
    PersistentWorkerTest
    PRIVATE libftpp
)
