//
// Created by dustyn on 6/15/26.
//

#include "aes.h"

void handleOpenSSLErrors(std::string *where) {
    std::cout << "OpenSSL Error: " << where << std::endl;
    exit(1);
}

size_t calcDecodeLength(char *b64input) {
    size_t len = strlen(b64input), padding = 0;

    if (b64input[len - 1] == '=' && b64input[len - 2] == '=') //last two chars are =
        padding = 2;
    else if (b64input[len - 1] == '=') //last char is =
        padding = 1;
    return (len * 3) / 4 - padding;
}

void Base64Decode(char *b64message, unsigned char **buffer, size_t *length) {
    BIO *bio, *b64;

    int decodeLen = calcDecodeLength(b64message);
    *buffer = (unsigned char *) malloc(decodeLen + 1);
    (*buffer)[decodeLen] = '\0';

    bio = BIO_new_mem_buf(b64message, -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    *length = BIO_read(bio, *buffer, strlen(b64message));
    BIO_free_all(bio);
}

bool aes_256_gcm_encrypt(const unsigned char *plaintext, int plaintext_len, const unsigned char *key, const unsigned char *iv, unsigned char *ciphertext, int &ciphertext_len) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cerr << "Failed to create EVP_CIPHER_CTX" << std::endl;
        return false;
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, key, iv) != 1) {
        std::cerr << "Failed to initialize encryption" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    if (EVP_EncryptUpdate(ctx, ciphertext, &ciphertext_len, plaintext, plaintext_len) != 1) {
        std::cerr << "Failed to encrypt data" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

bool aes_256_gcm_decrypt(const unsigned char *ciphertext, int ciphertext_len, const unsigned char *key, const unsigned char *iv, unsigned char *plaintext, int &plaintext_len) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cerr << "Failed to create EVP_CIPHER_CTX" << std::endl;
        return false;
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, key, iv) != 1) {
        std::cerr << "Failed to initialize decryption" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    if (EVP_DecryptUpdate(ctx, plaintext, &plaintext_len, ciphertext, ciphertext_len) != 1) {
        std::cerr << "Failed to decrypt data" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    EVP_CIPHER_CTX_free(ctx);
    return true;
}