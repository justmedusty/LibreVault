cmake_minimum_required(VERSION 4.2)
project(LibreVault)

set(CMAKE_CXX_STANDARD 26)

find_package(OpenSSL REQUIRED)

add_executable(LibreVault src/main.cpp
        src/config/config_representation.cpp
        src/config/config_representation.h
        src/crypt/encryption.cpp
        src/crypt/encryption.h
        src/crypt/aes.cpp
        src/crypt/aes.h)

target_link_libraries(LibreVault PRIVATE OpenSSL::Crypto)