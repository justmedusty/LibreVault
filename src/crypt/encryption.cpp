//
// Created by dustyn on 6/14/26.
//

#include "encryption.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#if defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
#include <sys/mman.h>
#endif
#include <openssl/rand.h>

#include "base64.h"
#include "key_derivation.h"
#include "algo/aes.h"
#ifdef WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
namespace Encryption {
    /*
     *  This SHOULDNt be an issue generally speaking for an application like this that just runs does something interactive then exits, but
     *  we should be cautious anyway and ensure that it cannot be paged to disk with sensitive key material or plaintext password.
     */
    void lock_memory() {
#if defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
        //THIS SHOULD HAVE MCL_CURRENT AND MCL_FUTURE FOR NOW JUST MCL FUTURE SINCE IT NEEDS ADDDED CAPS FOR BOTH
        if (mlockall(MCL_FUTURE) < 0) {
            // lock all current and future pages into main memory
            std::cerr <<
                    "mlockall failed, exiting early for security purposes. Check memory availability/consumption and try again."
                    << std::endl;
            exit(1);
        }


#elif defined(_WIN32)
        VirtualLock(ptr, size);
        return 1;
#else
        std::cout <<
                "Memory lock cannot be attained, this is not a massive risk since this program is short lived, but paging to disk can compromise program security"
                << std::endl;
        return 0;
#endif
    }

    //Check if we are actually in a terminal environment
    bool stdin_terminal() {
#ifdef WIN32
        return _isatty(_fileno(stdin));
#else
        return isatty(fileno(stdin));
#endif
    }

    void set_stdin_echo(bool enable) {
        if (!stdin_terminal()) {
            return;
        }
#ifdef WIN32
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        if (!GetConsoleMode(hStdin, &mode)) {
            fputs("GetConsoleMode failed\n", stderr);
            return;
        }
        if (!enable) {
            mode &= ~ENABLE_ECHO_INPUT;
        } else {
            mode |= ENABLE_ECHO_INPUT;
        }
        if (!SetConsoleMode(hStdin, mode)) {
            fputs("SetConsoleMode failed\n", stderr);
        }
#else
        termios tty;
        if (tcgetattr(STDIN_FILENO, &tty) != 0) {
            fputs("tcgetattr failed\n", stderr);
            return;
        }
        if (!enable) {
            tty.c_lflag &= static_cast<decltype(tty.c_lflag)>(~ECHO);
        } else {
            tty.c_lflag |= ECHO;
        }
        if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) != 0) {
            fputs("tcsetattr failed\n", stderr);
        }
#endif
    }

    void EncryptionContext::generate_iv() {
        auto iv_ptr = reinterpret_cast<unsigned char *>(this->iv.data());
        RAND_bytes(iv_ptr, AES_GCM_IV_LEN);
    }

    std::string EncryptionContext::get_signature() const {
        std::string signature;
        std::string defcon;

        switch (this->current_defcon) {
            case Defcon::DEFCON1:
                defcon = std::string{LIBREVAULT_DEFCON_1};
                break;
            case Defcon::DEFCON2:
                defcon = std::string{LIBREVAULT_DEFCON_2};
                break;
            case Defcon::DEFCON3:
                defcon = std::string{LIBREVAULT_DEFCON_3};
                break;
            case Defcon::DEFCON4:
                defcon = std::string{LIBREVAULT_DEFCON_4};
                break;
            case Defcon::DEFCON5:
                defcon = std::string{LIBREVAULT_DEFCON_5};
                break;
            default:
                return "";
        }

        std::ifstream vault(this->configRepresentation.vault_file_path);

        std::string line;
        bool here = false;
        while (std::getline(vault, line)) {
            if (line == defcon) {
                here = true;
                continue;
            }

            if (here) {
                std::string sig = line.substr(sizeof(LIBREVAULT_VAULT_SIG_START),
                                              line.size() - sizeof(LIBREVAULT_VAULT_SIG_END));
                std::cout << "SIG :" << sig << std::endl;
                return sig;
            }
        }
        std::cerr << "End of get_signature reached, this is a major bug! Please report to developer" << std::endl;
        return "";
    }

    bool EncryptionContext::verify_defcon_signature() {
        std::string expected = LIBREVAULT_ENCRYPTION_STRING;

        std::string signature = this->get_signature();
        const std::string iv = Base64::base64_decode(signature).substr(0,AES_GCM_IV_LEN);
        std::string ciphertext = Base64::base64_decode(iv).substr(AES_GCM_IV_LEN + AES_GCM_AEAD_TAG_SIZE, iv.size());
        std::string tag = Base64::base64_decode(ciphertext).substr(AES_GCM_IV_LEN, AES_GCM_AEAD_TAG_SIZE);
        std::string plaintext = {50};


        int plaintext_len = 50;
        const auto plaintext_ptr = reinterpret_cast<unsigned char *>(plaintext.data());
        const auto *ciphertext_ptr = reinterpret_cast<unsigned char *>(ciphertext.data());
        const auto tag_ptr = reinterpret_cast<unsigned char *>(tag.data());
        const auto key_ptr = reinterpret_cast<unsigned char *>(this->key_material.data());

        const auto ret = aes_256_gcm_decrypt(ciphertext_ptr, ciphertext.size(), key_ptr,
                                             reinterpret_cast<const unsigned char *>(iv.c_str()), plaintext_ptr,
                                             plaintext_len, tag_ptr);

        if (ret != 0) {
            std::cerr << "Encryption verification failed" << std::endl;
            return false;
        }

        if (ciphertext != plaintext) {
            std::cerr << "verify_defcon_signature: Outputs do not much" << std::endl;
            return false;
        }

        return true;
    }


    void EncryptionContext::decrypt_string(std::string &ciphertext) {
        std::string decoded = Base64::base64_decode(ciphertext);
        std::string iv = decoded.substr(KDF_SALT_SIZE_BYTES, AES_GCM_IV_LEN);
        std::string tag = decoded.substr(KDF_SALT_SIZE_BYTES + AES_GCM_IV_LEN, AES_GCM_AEAD_TAG_SIZE);
        std::string cipherttxt = decoded.substr(KDF_SALT_SIZE_BYTES + AES_GCM_IV_LEN + AES_GCM_AEAD_TAG_SIZE,
                                                ciphertext.size());

        std::string salt = decoded.substr(0,KDF_SALT_SIZE_BYTES);

        std::vector<uint8_t> salt_vec(salt.begin(), salt.end());


        derive_key(this->passphrase, salt_vec, this->key_material);


        int i = 0;
        for (const auto &c: iv) {
            this->iv[i++] = static_cast<std::byte>(c);
        }

        std::string plaintext(ciphertext.length(), '\0');

        const auto plaintext_ptr = reinterpret_cast<unsigned char *>(plaintext.data());
        const auto ciphertext_ptr = reinterpret_cast<unsigned char *>(cipherttxt.data());
        const auto tag_ptr = reinterpret_cast<unsigned char *>(tag.data());
        const auto key_ptr = reinterpret_cast<unsigned char *>(this->key_material.data());
        const auto iv_ptr = reinterpret_cast<unsigned char *>(this->iv.data());

        int plaintext_len = ciphertext.length();
        auto ret = aes_256_gcm_decrypt(ciphertext_ptr, cipherttxt.size(), key_ptr, iv_ptr, plaintext_ptr, plaintext_len,
                                       tag_ptr);

        if (!ret) {
            std::cerr << "Decryption failed" << std::endl;
        }
    }


    void EncryptionContext::receive_passphrase() {
        set_stdin_echo(false);
        std::string current_DEFCON;

        switch (this->current_defcon) {
            case Defcon::DEFCON1:
                current_DEFCON = std::string{"DEFCON1"};
                break;
            case Defcon::DEFCON2:
                current_DEFCON = std::string{"DEFCON2"};
                break;
            case Defcon::DEFCON3:
                current_DEFCON = std::string{"DEFCON3"};
                break;
            case Defcon::DEFCON4:
                current_DEFCON = std::string{"DEFCON4"};
                break;
            case Defcon::DEFCON5:
                current_DEFCON = std::string{"DEFCON5"};
                break;
        }
        std::cout << "Please enter your password, the DEFCON level password you must provide is " << current_DEFCON <<
                ":" << std::endl;
        std::cin >> passphrase;

        set_stdin_echo(true);
    }
};
