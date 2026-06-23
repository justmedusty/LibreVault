//
// Created by dustyn on 6/18/26.
//

#ifndef LIBREVAULT_KEY_DERIVATION_H
#define LIBREVAULT_KEY_DERIVATION_H
#define RANDOM_BITS_FILE "/dev/random"
#define DEFAULT_HASH_ITER

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>

std::vector<uint8_t> generate_salt();
#endif //LIBREVAULT_KEY_DERIVATION_H
