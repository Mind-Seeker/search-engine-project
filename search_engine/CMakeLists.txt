cmake_minimum_required(VERSION 4.1)
project(search_engine)

set(CMAKE_CXX_STANDARD 20)

set(JSON_BuildTests OFF CACHE INTERNAL "")

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(search_engine)

add_subdirectory(nlohmann_json)
add_subdirectory(sources)

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

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json gtest_main)