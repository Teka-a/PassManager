cmake_minimum_required(VERSION 3.16)
project(file LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(file
    main.cpp
    tst_file.cpp
)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

target_link_libraries(file
    PRIVATE
        gtest
        gmock
        Threads::Threads
        Qt6::Core
        OpenSSL::SSL
        OpenSSL::Crypto
)

add_test(NAME file COMMAND file)
