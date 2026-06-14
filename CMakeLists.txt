cmake_minimum_required(VERSION 4.2)
project(LibreVault)

set(CMAKE_CXX_STANDARD 26)

add_executable(LibreVault src/main.cpp
        src/filesystem/filesystem_handling.cpp
        src/filesystem/filesystem_handling.h)
