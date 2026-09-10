cmake_minimum_required(VERSION 3.16)

project(libftpp
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(libftpp
 srcs/DataStructure/DataBuffer/data_buffer.cpp
 srcs/DesignPatterns/Memento/memento.cpp
 srcs/IOStream/ThreadSafeIOStream/thread_safe_iostream.cpp
 srcs/Thread/Thread/thread.cpp
 srcs/Thread/WorkerPool/worker_pool.cpp
 srcs/Thread/PersistentWorker/persistent_worker.cpp
 srcs/Mathematics/IVector2/ivector2.cpp
 srcs/Mathematics/IVector3/ivector3.cpp
 srcs/Mathematics/Random2DCoordinateGenerator/random_2D_coordinate_generator.cpp
 srcs/Mathematics/PerlinNoise2D/perlin_noise_2D.cpp
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
    PUBLIC ${PROJECT_SOURCE_DIR}/includes
)


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
    tests/Mathematics/ivector2_test.cpp
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
