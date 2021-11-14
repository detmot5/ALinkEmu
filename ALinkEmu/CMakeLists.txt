cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE Sources CONFIGURE_DEPENDS Src/*.cpp)
file(GLOB_RECURSE Headers CONFIGURE_DEPENDS ALinkEmu.hpp Src/*.hpp)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.x
)

FetchContent_MakeAvailable(spdlog)


add_library(ALinkEmu ${Sources} ${Headers})
target_include_directories(ALinkEmu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ALinkEmu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Src)
target_link_libraries(ALinkEmu PUBLIC spdlog::spdlog)

enable_testing()

add_executable(
        test
        Test/Test.cpp
)
target_link_libraries(
        test
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(test)


