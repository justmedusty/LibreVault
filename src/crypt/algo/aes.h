//
// Created by dustyn on 6/15/26.
//

#ifndef LIBREVAULT_AES_H
#define LIBREVAULT_AES_H
#include <iostream>
#include "../encryption.h"
#include <string>
#include <openssl/evp.h>

#define AES_IV_LEN 16
bool aes_256_gcm_encrypt(const unsigned char *plaintext, int plaintext_len, const unsigned char *key,
                         const unsigned char *iv, unsigned char *ciphertext, int &ciphertext_len);

bool aes_256_gcm_decrypt(const unsigned char *ciphertext, int ciphertext_len, const unsigned char *key,
                         const unsigned char *iv, unsigned char *plaintext, int &plaintext_len);
#endif //LIBREVAULT_AES_H
