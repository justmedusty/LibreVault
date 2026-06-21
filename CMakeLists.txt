cmake_minimum_required(VERSION 4.2)
project(LibreVault)

set(CMAKE_CXX_STANDARD 26)

find_package(OpenSSL REQUIRED)

add_executable(LibreVault src/main.cpp
        src/config/config_representation.cpp
        src/config/config_representation.h
        src/crypt/encryption.cpp
        src/crypt/encryption.h
        src/crypt/algo/aes.cpp
        src/crypt/algo/aes.h
        src/filesystem/vault_handling.cpp
        src/filesystem/vault_handling.h
        src/crypt/base64.cpp
        src/crypt/base64.h
        test/test.cpp
        src/crypt/key_derivation.cpp
        src/crypt/key_derivation.h)

target_link_libraries(LibreVault PRIVATE OpenSSL::Crypto)
target_include_directories(LibreVault PRIVATE ${CMAKE_SOURCE_DIR}/src)