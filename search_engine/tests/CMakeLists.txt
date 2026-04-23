cmake_minimum_required(VERSION 4.1)
project(search_test)

add_executable(search_test)

target_sources(search_test
        PUBLIC
        ${INCLUDE_DIR}/FunctionsForUniqueWords.h
        ${INCLUDE_DIR}/ConverterJSON.h
        ${CMAKE_SOURCE_DIR}/sources/ConverterJSON.cpp
        ${INCLUDE_DIR}/InvertedIndex.h
        ${CMAKE_SOURCE_DIR}/sources/InvertedIndex.cpp
        ${INCLUDE_DIR}/SearchServer.h
        ${CMAKE_SOURCE_DIR}/sources/SearchServer.cpp
        ${CMAKE_SOURCE_DIR}/tests/index_tests.cpp
        ${CMAKE_SOURCE_DIR}/tests/server_tests.cpp)

target_include_directories(search_test
        PUBLIC
        ${INCLUDE_DIR}
        ${gtest_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

include_directories(${gtest_SOURCE_DIR}/include${gtest_SOURCE_DIR})

set(gtest_disable_pthreadson)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

target_link_libraries(search_test PRIVATE nlohmann_json::nlohmann_json gtest_main)