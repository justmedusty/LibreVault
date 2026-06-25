//
// Created by dustyn on 6/24/26.
//
#define BOOST_TEST_MODULE EncryptionTest
#include <boost/test/unit_test.hpp>
#include "crypt/algo/aes.h"
#include "crypt/encryption.h"
#include <openssl/rand.h>

BOOST_AUTO_TEST_CASE(aes_256_gcm_e) {
    const char *plaintext = "Here is some plaintext for AES 256 GCM testing";
    int len = strlen(plaintext);
    auto *key = static_cast<uint8_t *>(malloc(32));
    auto *iv = static_cast<uint8_t *>(malloc(AES_GCM_IV_LEN));
    auto *ciphertext = static_cast<uint8_t *>(malloc(len));
    memset(ciphertext, '\0', len);
    auto *tag = static_cast<uint8_t *>(malloc(AES_GCM_AEAD_TAG_SIZE));
    std::cout << "len is " << len << std::endl;
    RAND_bytes(key, 32);
    RAND_bytes(iv, AES_GCM_IV_LEN);
    auto ret = aes_256_gcm_encrypt((unsigned char *) plaintext, len, key, iv, ciphertext, &len, tag);
    std::cout << "ret is : " << ret << std::endl;
    std::string cipher(len, '\0');

    std::copy_n(ciphertext, len, cipher.begin());
    std::string b64_encrypted = Base64::base64_encode(cipher);
    std::cout << "len is " << len << std::endl;
    std::cout << "ciphertext is : " << b64_encrypted << std::endl;

    free(key);
    free(iv);
    free(ciphertext);
    free(tag);


    BOOST_CHECK_EQUAL(ret, 1);
}

BOOST_AUTO_TEST_CASE(aes_256_gcm_e_then_d) {
    const char *plaintext = "Here is some plaintext for AES 256 GCM testing";
    int len = strlen(plaintext);
    auto *key = static_cast<uint8_t *>(malloc(32));
    auto *iv = static_cast<uint8_t *>(malloc(AES_GCM_IV_LEN));
    auto *ciphertext = static_cast<uint8_t *>(malloc(len));
    memset(ciphertext, '\0', len);
    auto *tag = static_cast<uint8_t *>(malloc(AES_GCM_AEAD_TAG_SIZE));
    std::cout << "len is " << len << std::endl;
    RAND_bytes(key, 32);
    RAND_bytes(iv, AES_GCM_IV_LEN);
    auto ret = aes_256_gcm_encrypt((unsigned char *) plaintext, len, key, iv, ciphertext, &len, tag);
    std::cout << "ret is : " << ret << std::endl;
    std::string cipher(len, '\0');

    std::copy_n(ciphertext, len, cipher.begin());
    std::string b64_encrypted = Base64::base64_encode(cipher);
    std::cout << "len is " << len << std::endl;
    std::cout << "ciphertext is : " << b64_encrypted << std::endl;

    uint8_t *plaintext2 = static_cast<uint8_t *>(malloc(len));

    auto ret2 = aes_256_gcm_decrypt(ciphertext, len, key, iv, plaintext2, len, tag);

    std::cout << "ret2 is : " << ret2 << std::endl;

    free(key);
    free(iv);
    free(ciphertext);
    free(tag);
    free(plaintext2);

    BOOST_CHECK_EQUAL(ret2, 1);
}

BOOST_AUTO_TEST_CASE(aes_256_gcm_e_then_d_then_check_plaintext) {
    const char *plaintext = "Here is some plaintext for AES 256 GCM testing";
    int len = strlen(plaintext);
    auto *key = static_cast<uint8_t *>(malloc(32));
    auto *iv = static_cast<uint8_t *>(malloc(AES_GCM_IV_LEN));
    auto *ciphertext = static_cast<uint8_t *>(malloc(len));
    memset(ciphertext, '\0', len);
    auto *tag = static_cast<uint8_t *>(malloc(AES_GCM_AEAD_TAG_SIZE));
    std::cout << "len is " << len << std::endl;
    RAND_bytes(key, 32);
    RAND_bytes(iv, AES_GCM_IV_LEN);
    auto ret = aes_256_gcm_encrypt((unsigned char *) plaintext, len, key, iv, ciphertext, &len, tag);
    std::cout << "ret is : " << ret << std::endl;
    std::string cipher(len, '\0');

    std::copy_n(ciphertext, len, cipher.begin());
    std::string b64_encrypted = Base64::base64_encode(cipher);
    std::cout << "len is " << len << std::endl;
    std::cout << "ciphertext is : " << b64_encrypted << std::endl;

    const auto plaintext2 = static_cast<char *>(malloc(len));

    auto ret2 = aes_256_gcm_decrypt(ciphertext, len, key, iv, (unsigned char *) plaintext2, len, tag);
    std::cout << "ret2 is : " << ret2 << std::endl;
    BOOST_CHECK_EQUAL(ret2, 1);

    for (auto i = 0; i < len; i++) {
        BOOST_CHECK_EQUAL(plaintext[i], plaintext2[i]);
    }

    free(key);
    free(iv);
    free(ciphertext);
    free(tag);
    free(plaintext2);
}
