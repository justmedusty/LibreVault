//
// Created by dustyn on 6/14/26.
//

#ifndef LIBREVAULT_ENCRYPTION_H
#define LIBREVAULT_ENCRYPTION_H
#include <string>
#include <vector>
#include "openssl/aes.h"
#include "openssl/crypto.h"
#include "openssl/blowfish.h"
#include "openssl/pkcs12.h"
#include "openssl/camellia.h"
#include "config/config_representation.h"
#include <unistd.h>

//Tell the OS not to page this memory to disk
uint32_t lock_pointer(void *ptr, uint32_t size);


namespace Encryption {
    struct EncryptionContext {
        std::string passphrase;
        std::vector<std::byte> key_material;
        std::vector<std::byte> iv;

        void receive_passphrase();

        void decrypt_string(std::string secret);

        void encrypt_string(std::string secret);

        EncryptionContext() {

        }

        ~EncryptionContext() {
            OPENSSL_cleanse(&this->passphrase, this->passphrase.size());
            OPENSSL_cleanse(&this->key_material, this->key_material.size());
        }
    };



};
#endif //LIBREVAULT_ENCRYPTION_H
