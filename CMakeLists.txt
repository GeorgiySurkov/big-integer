cmake_minimum_required(VERSION 3.21)

project(biginteger)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# dependencies
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
        biginteger
        main.cpp
        biginteger.cpp
        biginteger.h
        Vector.h
        helpers.h
        helpers.cpp)

add_executable(
        biginteger_test
        tests.cpp
        biginteger.cpp
        biginteger.h
        Vector.h
        helpers.h
        helpers.cpp)
target_link_libraries(
        biginteger_test
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(biginteger_test)
