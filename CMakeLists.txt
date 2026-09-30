cmake_minimum_required(VERSION 3.16)

project(libftpp
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Werror)

add_library(libftpp
 srcs/DataStructure/DataBuffer/data_buffer.cpp
 srcs/DesignPatterns/Memento/memento.cpp
 srcs/IOStream/ThreadSafeIOStream/thread_safe_iostream.cpp
 srcs/IOStream/Log/log.cpp
 srcs/Thread/Thread/thread.cpp
 srcs/Thread/WorkerPool/worker_pool.cpp
 srcs/Thread/PersistentWorker/persistent_worker.cpp
 srcs/Mathematics/IVector2/ivector2.cpp
 srcs/Mathematics/IVector3/ivector3.cpp
 srcs/Mathematics/Random2DCoordinateGenerator/random_2D_coordinate_generator.cpp
 srcs/Mathematics/PerlinNoise2d/perlin_noise_2D.cpp
 srcs/Network/Client/client.cpp
 srcs/Network/Message/message.cpp
 srcs/Network/Server/server.cpp
 srcs/Tester/printer.cpp
 srcs/Tester/runner.cpp
 srcs/DataStructure/SharedMemoryIPC/shared_memory_ipc.cpp
)
# Helper function for all tests

function(add_libftpp_test TARGET_NAME SOURCE)

    add_executable(${TARGET_NAME} ${SOURCE})

    target_link_libraries(${TARGET_NAME}

        PRIVATE libftpp
    )

    set_target_properties(${TARGET_NAME}

        PROPERTIES

        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    )

endfunction()

target_include_directories(
    libftpp
    PUBLIC ${PROJECT_SOURCE_DIR}
    PUBLIC ${PROJECT_SOURCE_DIR}/includes
)

set_target_properties(libftpp PROPERTIES OUTPUT_NAME ftpp)


# =========================
# DataStructure Tests
# =========================

add_libftpp_test(
    DataBufferTest
    tests/DataStructure/data_buffer_test.cpp
)

add_libftpp_test(
    PoolTest
    tests/DataStructure/pool_test.cpp
)

add_libftpp_test(
    RingBufferTest
    tests/DataStructure/ring_buffer_test.cpp
)

add_libftpp_test(
    SharedMemoryIPC
    srcs/DataStructure/SharedMemoryIPC/shared_memory_ipc.cpp
)
# =========================
# DesignPatterns Tests
# =========================

add_libftpp_test(
    MementoTest
    tests/DesignPatterns/memento_test.cpp
)

add_libftpp_test(
    ObserverTest
    tests/DesignPatterns/observer_test.cpp
)

add_libftpp_test(
    SingletonTest
    tests/DesignPatterns/singleton_test.cpp
)

add_libftpp_test(
    StateMachineTest
    tests/DesignPatterns/state_machine_test.cpp
)

# =========================
# IOStream Tests
# =========================

add_libftpp_test(
    ThreadSafeIOStreamTest
    tests/IOStream/thread_safe_iostream_test.cpp
)

add_libftpp_test(
    LogTest
    tests/IOStream/log_test.cpp
)

# =========================
# Thread Tests
# =========================

add_libftpp_test(
    ThreadSafeQueueTest
    tests/Thread/thread_safe_queue_test.cpp
)

add_libftpp_test(
    ThreadTest
    tests/Thread/thread_test.cpp
)

add_libftpp_test(
    WorkerPoolTest
    tests/Thread/worker_pool_test.cpp
)

add_libftpp_test(
    PersistentWorkerTest
    tests/Thread/persistent_worker_test.cpp
)


# =========================
# Mathematics Tests
# =========================

add_libftpp_test(
    IVector2Test
    tests/Mathematics/IVector2_test.cpp
)

add_libftpp_test(
    IVector3Test
    tests/Mathematics/ivector3_test.cpp
)

add_libftpp_test(
    Random2DCoordinateGenerator
    tests/Mathematics/random_2D_coordinate_generator_test.cpp
)


add_libftpp_test(
    PerlinNoise2DTest
    tests/Mathematics/perlin_noise_2D_test.cpp
)

# =========================
# Network Tests
# =========================

add_libftpp_test(
    NetworkTest
    tests/Network/network_test.cpp
)

# =========================
# Tester Tests
# =========================

add_libftpp_test(
    PrinterTest
    tests/Tester/printer_test.cpp
)

# =========================
# Combined test runner
# =========================

set(LIBFTPP_TEST_SOURCES
    tests/DataStructure/data_buffer_test.cpp
    tests/DataStructure/pool_test.cpp
    tests/DataStructure/ring_buffer_test.cpp
    tests/DesignPatterns/memento_test.cpp
    tests/DesignPatterns/observer_test.cpp
    tests/DesignPatterns/singleton_test.cpp
    tests/DesignPatterns/state_machine_test.cpp
    tests/IOStream/thread_safe_iostream_test.cpp
    tests/IOStream/log_test.cpp
    tests/Thread/thread_test.cpp
    tests/Thread/worker_pool_test.cpp
    tests/Thread/persistent_worker_test.cpp
    tests/Thread/thread_safe_queue_test.cpp
    tests/Mathematics/IVector2_test.cpp
    tests/Mathematics/ivector3_test.cpp
    tests/Mathematics/random_2D_coordinate_generator_test.cpp
    tests/Mathematics/perlin_noise_2D_test.cpp
    tests/Network/network_test.cpp
    tests/Tester/printer_test.cpp
)

add_executable(AllTests
    tests/all_tests_main.cpp
    ${LIBFTPP_TEST_SOURCES}
)

target_compile_definitions(AllTests PRIVATE LIBFTPP_TEST_NO_MAIN)

target_link_libraries(AllTests PRIVATE libftpp)

set_target_properties(AllTests
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)
