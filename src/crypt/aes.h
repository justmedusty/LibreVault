//
// Created by dustyn on 6/15/26.
//

#ifndef LIBREVAULT_AES_H
#define LIBREVAULT_AES_H
#include <iostream>
#include "encryption.h"
#include <string>
#include <openssl/evp.h>


std::string decrypt_aes_256_gcm(const unsigned char *ciphertext,
                                int ciphertext_len,
                                unsigned char *key,
                                unsigned char *iv);


void initAES_256_CBC(std::string &pass, unsigned char *salt, unsigned char *key, unsigned char *iv);

void initAES_256_GCM(std::string &pass, unsigned char *salt, unsigned char *key, unsigned char *iv);

std::string decrypt_aes_256_cbc(const unsigned char *ciphertext,
                                int ciphertext_len,
                                unsigned char *key,
                                unsigned char *iv);

std::string decrypt_aes_256_gcm(const unsigned char *ciphertext,
                                int ciphertext_len,
                                unsigned char *key,
                                unsigned char *iv);

#endif //LIBREVAULT_AES_H
