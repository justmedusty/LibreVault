cmake_minimum_required(VERSION 4.2)
project(LibreVault LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_library(LibreVaultCore
        src/config/config_representation.cpp
        src/crypt/encryption.cpp
        src/crypt/algo/aes.cpp
        src/filesystem/vault_handling.cpp
        src/crypt/base64.cpp
        src/crypt/key_derivation.cpp
)

target_include_directories(LibreVaultCore PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(LibreVaultCore PUBLIC OpenSSL::Crypto)

add_executable(LibreVault
        src/main.cpp
)
target_link_libraries(LibreVault PRIVATE LibreVaultCore)

add_executable(LibreVaultParseTests
        test/parse_test.cpp
)
add_executable(LibreVaultEncryptionTests
        test/encryption_test.cpp
)

add_executable(LibreVaultKeyDerivationTests
        test/key_derivation_test.cpp
)

target_link_libraries(LibreVaultKeyDerivationTests PRIVATE
        LibreVaultCore
        Boost::unit_test_framework
)

target_link_libraries(LibreVaultEncryptionTests PRIVATE
        LibreVaultCore
        Boost::unit_test_framework
)

target_link_libraries(LibreVaultParseTests  PRIVATE
        LibreVaultCore
        Boost::unit_test_framework
)

target_compile_options(LibreVaultCore PUBLIC -fsanitize=address -g)
target_link_options(LibreVaultCore PUBLIC -fsanitize=address)

enable_testing()
add_test(NAME LibreVaultParseTests COMMAND LibreVaultParseTests)
add_test(NAME LibreVaultEncryptionTests COMMAND LibreVaultEncryptionTests)
add_test(NAME LibreVaultKeyDerivationTests COMMAND LibreVaultKeyDerivationTests)