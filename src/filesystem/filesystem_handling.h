
//
// Created by dustyn on 6/14/26.
//

#ifndef LIBREVAULT_FILESYSTEM_HANDLING_H
#define LIBREVAULT_FILESYSTEM_HANDLING_H
#include <string>

#define DEFAULT_CONFIG_FILE_PATH /etc/librevault.conf
#define DEFAULT_VAULT_FILE_LOCATION /var/lib/librevault/lv.db

enum class FileObfuscation {
    NO_OBFUSCATION,
    FILE_CONTENTS_SCRAMBLING, //Make it less obvious that its a KV map and possibly of value
    EMBEDDED,//Embed the KV map into another file like a dummy binary and hiding it in the heap mapping of the ELF executable etc
    FILE_LOCATION_RANDOMIZATION,//Everything normal but just put the kv file in an odd place
};



struct FilesystemHandling {
    std::string config_file_path;
    std::string vault_file_location;
    FileObfuscation obfuscation;

    FilesystemHandling(
        const std::string &configPath,
        const std::string &vaultFileLocation,
        FileObfuscation obfuscationMethod
        ) {
        config_file_path = configPath;
        vault_file_location = vaultFileLocation;
        obfuscation = obfuscationMethod;
    }

    ~FilesystemHandling() {

    }

};
#endif //LIBREVAULT_FILESYSTEM_HANDLING_H
