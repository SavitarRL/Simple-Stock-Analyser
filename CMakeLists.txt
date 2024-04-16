cmake_minimum_required(VERSION 3.15)
project(StockAnalyser VERSION 0.1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
set(SOURCES
    main.cpp
    analyser.cpp
)

# main executable
add_executable(analyser ${SOURCES})

# GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_TARGET analyser_tests)

add_executable(${TEST_TARGET} 
               analyser_tests.cpp)

target_link_libraries(${TEST_TARGET}  
                      gtest_main)

include(GoogleTest)

gtest_discover_tests(${TEST_TARGET} )
