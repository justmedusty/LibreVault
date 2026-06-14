
//
// Created by dustyn on 6/14/26.
//

#ifndef LIBREVAULT_FILESYSTEM_HANDLING_H
#define LIBREVAULT_FILESYSTEM_HANDLING_H
#include <string>

#include "../config/config_representation.h"

#define DEFAULT_CONFIG_FILE_PATH /etc/librevault.conf
#define DEFAULT_VAULT_FILE_LOCATION /var/lib/librevault/lv.db



struct FilesystemHandling {
    std::string config_file_path;
    std::string vault_file_location;

    bool configOpen = false;
    bool vaultOpen = false;


    /* Null by default */
    std::FILE *configFile = nullptr;
    std::FILE *vaultFile = nullptr;

    FilesystemHandling(
        const std::string &configPath,
        const std::string &vaultFileLocation
        ) {
        config_file_path = configPath;
        vault_file_location = vaultFileLocation;
    }

    ~FilesystemHandling() =default;

    void openConfig();
    void openVaultFile();
    [[nodiscard]] ConfigRepresentation *parseConfigFile() const;


};
#endif //LIBREVAULT_FILESYSTEM_HANDLING_H
