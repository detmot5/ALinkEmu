cmake_minimum_required(VERSION 3.15)
project(ALinkEmulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DEMU_DEBUG=1)

add_subdirectory(ALinkEmu)
add_subdirectory(Sandbox)
