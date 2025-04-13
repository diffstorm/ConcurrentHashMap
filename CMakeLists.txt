cmake_minimum_required(VERSION 3.14)
project(ConcurrentHashMap LANGUAGES CXX)

# Set C++ standard requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# Basic Demo executable
add_executable(demo_basic 
    demo_basic.cpp
)
target_link_libraries(demo_basic 
    PRIVATE 
    Threads::Threads
)

# Advanced Demo executable
add_executable(demo_advanced 
    demo_advanced.cpp
)
target_link_libraries(demo_advanced 
    PRIVATE 
    Threads::Threads
)

# Google Test executable
add_executable(ConcurrentHashMapTest 
    ConcurrentHashMapTest.cpp
)
target_link_libraries(ConcurrentHashMapTest 
    PRIVATE 
    GTest::GTest 
    GTest::Main 
    Threads::Threads
)

enable_testing()
add_test(NAME ConcurrentHashMapTest COMMAND ConcurrentHashMapTest)