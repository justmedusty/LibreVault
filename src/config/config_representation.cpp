//
// Created by dustyn on 6/14/26.
//

#include "config_representation.h"

#include <iostream>


void ConfigRepresentation::parse_command_line_args(std::vector<std::string> arguments) {
    for (auto arg = arguments.begin(); arg != arguments.end(); ++arg) {
        if (*arg == FLAG_HELP) {
            this->help();
        }
        if ((*arg) == FLAG_ENCRYPT) {
            this->decrypt = false;
            continue;
        }

        if ((*arg) == FLAG_DECRYPT) {
            this->decrypt = true;
            continue;
        }

        if (*arg == FLAG_VALUE) {
            if (arg == arguments.end()) {
                std::cerr <<
                        "You have passed a flag that requires a value, with no value given! You must provide a value when using the "
                        << FLAG_VALUE << " flag." << std::endl;
            }
            this->value = std::move(*(arg + 1));
            ++arg;
            continue;
        }

        if (*arg == FLAG_ENCRYPTION_ALGORITHM) {
            std::cout << "Encryption algorithm: this option is not yet supported. AES 256 CBC only. " << std::endl;
            exit(1);
        }

        if (*arg == FLAG_KEY) {
            if (arg == arguments.end()) {
                std::cerr <<
                        "You have passed a flag that requires a value, with no value given! You must provide a value when using the "
                        << FLAG_KEY << " flag." << std::endl;
                this->key = std::move(*(arg + 1));
                ++arg;
                continue;
            }
        }

        if (*arg == FLAG_VAULT_FILE_LOCATION) {
            if (arg == arguments.end()) {
                std::cerr <<
                        "You have passed a flag that requires a value, with no value given! You must provide a value when using the "
                        << FLAG_VAULT_FILE_LOCATION << " flag." << std::endl;
            }
            this->vault_file_path = std::move(*(arg + 1));
            ++arg;
            continue;
        }
    }

    if (this->key.empty()) {
        std::cerr << "You have not specified a key! You must provide a key! Try librevault -h for help!" << std::endl;
        exit(1);
    }

    if (this->decrypt == false && this->value.empty()) {
        std::cerr <<
                "You have no specified a value and are trying to encrypt! You must provide a value! Try librevault -h for help!"
                <<
                std::endl;
        exit(1);
    }

    if (!this->vault_file_path.empty() && !std::filesystem::exists(this->vault_file_path)) {
        std::cerr << "You have specified a vault file and the file does not exist! See : " << this->vault_file_path
                << "Try librevault -h for help!" <<
                std::endl;
    }
}

