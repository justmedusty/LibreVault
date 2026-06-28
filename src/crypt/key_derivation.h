//
// Created by dustyn on 6/18/26.
//

#ifndef CITADEL_KEY_DERIVATION_H
#define CITADEL_KEY_DERIVATION_H
#define KDF_SALT_SIZE_BYTES 16
#define ARGON2_ROUNDS_BASE 6 // High base and it goes higher with each defcon level

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>

enum class Defcon;

int derive_key(
    const std::string &password,
    std::vector<uint8_t> &salt,
    std::vector<std::byte> &key,
    Defcon defcon);

void generate_salt(std::vector<uint8_t> &salt_buf);
#endif //CITADEL_KEY_DERIVATION_H
