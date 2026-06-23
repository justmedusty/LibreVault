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
void lock_memory();

namespace Encryption {
    struct EncryptionContext {
        std::string passphrase;
        std::vector<std::byte> key_material;
        std::vector<std::byte> iv;
        std::string secret;
        Defcon current_defcon;
        EncryptionMode mode;

        void receive_passphrase();

        void decrypt_string(std::string ciphertext);

        void encrypt_string(std::string secret);

        EncryptionContext(std::string &passphrase) {
            this->passphrase = std::move(passphrase);
            this->key_material = std::vector<std::byte>(0);
            this->iv = std::vector<std::byte>(0);
            this->mode = EncryptionMode::AES_256_GCM; //Default algo
            this->secret = "";
        }

        EncryptionContext();

        ~EncryptionContext() {
            OPENSSL_cleanse(&this->passphrase, this->passphrase.size());
            OPENSSL_cleanse(&this->key_material, this->key_material.size());
            OPENSSL_cleanse(&this->secret, this->secret.size());
        }
    };
};
#endif //LIBREVAULT_ENCRYPTION_H
