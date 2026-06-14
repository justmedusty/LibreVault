//
// Created by dustyn on 6/14/26.
//

#include "filesystem_handling.h"
#include <cstdint>
#include <iostream>
#include <ostream>
#include <cstdio>
#include <sstream>

#include "../config/config_representation.h"

std::int32_t parseEncryptionConfig(std::string &value, ConfigRepresentation *config) {

    return 0;
}

std::int32_t parseFileObfuscationConfig(std::string &value, ConfigRepresentation *config) {
    return 0;
}

int32_t parseConfigurationFile(std::byte *contents, std::int32_t length, ConfigRepresentation *config) {
    if (config == nullptr) {
        return -1;
    }

    if (contents == nullptr) {
        return -1;
    }

    std::string_view stringView(reinterpret_cast<const char *>(contents), length);
    std::istringstream sstream(stringView);

    std::string line;
    while (std::getline(sstream, line)) {
        if (line.empty() || line[0] == '#') // skip blanks/comments
            continue;

        auto delimiter = line.find('=');

        if (delimiter == std::string::npos) {
            continue;
        }

        std::string key = line.substr(0, delimiter);
        std::string value = line.substr(delimiter + 1);

        if (key == ENCRYPTION_MODE_CONFIG_KEY) {
            auto ret = parseEncryptionConfig(value, config);
            if (ret == -1) {
                std::cout << "Failed to parse encryption config file. The offending substring is " << value <<
                        std::endl;
                exit(1);
            }
            continue;
        }

        if (key == FILE_OBFUSCATION_CONFIG_KEY) {
            auto ret = parseFileObfuscationConfig(value, config);\
            if (ret == -1) {
                std::cout << "Failed to parse encryption config file. The offending substring is " << value <<
                        std::endl;
                exit(1);
            }
            continue;
        }
    }
}

void FilesystemHandling::openConfig() {
    auto file = std::fopen(this->config_file_path.c_str(), "r");
    if (file == nullptr) {
        std::cout << "Failed to open config file " << this->config_file_path << std::endl;
        exit(1);
    }
    configFile = file;
    this->configOpen = true;
}

void FilesystemHandling::openVaultFile() {
    auto file = std::fopen(this->vault_file_location.c_str(), "rw");

    if (file == nullptr) {
        std::cout << "Failed to open vault file " << this->config_file_path << std::endl;
        exit(1);
    }
    configFile = file;
    this->vaultOpen = true;
}

ConfigRepresentation *FilesystemHandling::parseConfigFile() const {
    if (this->configOpen == false) {
        return nullptr;
    }


    ConfigRepresentation *config = new ConfigRepresentation();


    return nullptr;
}
