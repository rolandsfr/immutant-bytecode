cmake_minimum_required(VERSION 3.16)

project(Immutant LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(immutant
    src/main.cpp
    src/Chunk.cpp
    src/Compiler.cpp
    src/debug.cpp
    src/interpret.cpp
    src/repl.cpp
    src/runFile.cpp
    src/Scanner.cpp
    src/Stack.cpp
    src/VM.cpp
)

target_include_directories(immutant PRIVATE src)

enable_testing()

add_executable(
  immutant_test
  tests/Chunk.test.cpp
  tests/debug.test.cpp
  tests/Scanner.test.cpp
  tests/Stack.test.cpp
  tests/VM.test.cpp
  src/Chunk.cpp
  src/debug.cpp
  src/Scanner.cpp
  src/Stack.cpp
  src/VM.cpp
)

target_include_directories(immutant_test PRIVATE src)

target_link_libraries(
  immutant_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(immutant_test)