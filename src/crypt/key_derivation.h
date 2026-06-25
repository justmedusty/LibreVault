//
// Created by dustyn on 6/18/26.
//

#ifndef LIBREVAULT_KEY_DERIVATION_H
#define LIBREVAULT_KEY_DERIVATION_H
#define KDF_SALT_SIZE_BYTES 16
#define ARGON2_ROUNDS 14 // absolute tin foil hat-tery but that is good

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>

int derive_key(
    const std::string &password,
    std::vector<uint8_t> &salt,
    std::vector<std::byte> &key);

std::vector<uint8_t> generate_salt();
#endif //LIBREVAULT_KEY_DERIVATION_H
