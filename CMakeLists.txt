cmake_minimum_required(VERSION 3.16)

project(libftpp
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(libftpp
 srcs/DataStructure/DataBuffer/data_buffer.cpp
 srcs/DesignPatterns/Memento/memento.cpp
)

target_include_directories(
    libftpp
    PUBLIC ${PROJECT_SOURCE_DIR}/includes
)

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
    test/DesignPatterns/state_machine_test.cpp
)

target_link_libraries(
    StateMachineTest
    PRIVATE libftpp
)

