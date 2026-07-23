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

# DataStructure test
add_executable(

    DataStructureTest

    tests/data_structure_test.cpp

)

target_link_libraries(

    DataStructureTest

    PRIVATE

    libftpp

)

# DesignPattern test
add_executable(

    DesignPatternTest

    tests/design_patterns_test.cpp

)

target_link_libraries(

    DesignPatternTest

    PRIVATE

    libftpp

)
