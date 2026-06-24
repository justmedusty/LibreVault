//
// Created by dustyn on 6/15/26.
//

#ifndef LIBREVAULT_AES_H
#define LIBREVAULT_AES_H

#define AES_GCM_IV_LEN 12
#define AES_GCM_AEAD_TAG_SIZE 16

bool aes_256_gcm_encrypt(unsigned char *plaintext, int plaintext_len, const unsigned char *key,
                         const unsigned char *iv, unsigned char *ciphertext, int *ciphertext_len, unsigned char *tag);

bool aes_256_gcm_decrypt(const unsigned char *ciphertext, int ciphertext_len, const unsigned char *key,
                         const unsigned char *iv, unsigned char *plaintext, int &plaintext_len,
                         unsigned char *tag);
#endif //LIBREVAULT_AES_H
