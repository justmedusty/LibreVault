//
// Created by dustyn on 6/14/26.
//

#ifndef LIBREVAULT_CONFIG_REPRESENTATION_H
#define LIBREVAULT_CONFIG_REPRESENTATION_H
#include <filesystem>
#include <vector>
#include <iterator>
#include <getopt.h>
#include <iostream>

#include "config_representation.h"

enum class FileObfuscation;
enum class EncryptionMode;

#define ENCRYPTION_MODE_CONFIG_KEY "encryption_mode"
#define FILE_OBFUSCATION_CONFIG_KEY "filesystem_obfuscation"

#define STANDARD_VAULT_LOCATION ""

#define FLAG_HELP "-h"
#define FLAG_ENCRYPTION_ALGORITHM "-a"
#define FLAG_ENCRYPT "-e"
#define FLAG_DECRYPT "-d"
#define FLAG_KEY "-k"
#define FLAG_VALUE "-v"
#define FLAG_VAULT_FILE_LOCATION "-vf"


#define FILE_NO_OBFUSCATION_CONFIG_KEY "no_obfuscation"
#define FILE_SCRAMBLING_CONFIG_KEY "file_scrambling"
#define FILE_EMBEDDED_CONFIG_KEY "embedded-in-another-file"
#define FILE_LOCATION_RANDOMIZATION_CONFIG_KEY "file-location-randomization"


#define ENCRYPTION_OPTION_AES_256_CBC_KEY "aes_256_cbc"
#define ENCRYPTION_OPTION_AES_256_GCM_KEY "aes_256_gcm"
#define ENCRYPTION_OPTION_BLOWFISH_THEN_AES_256_CBC "blowfish_then_aes_256_cbc"
#define ENCRYPTION_OPTION_CAMELLIA_THEN_AES_256_CBC "camelia_then_aes_256_cbc"
#define ENCRYPTION_OPTION_BLOWFISH_THEN_AES_256_GCM "blowfish_then_aes_256_gcm"
#define ENCRYPTION_OPTION_CAMELLIA_THEN_AES_256_GCM "camelia_then_aes_256_gcm"


enum class EncryptionMode {
    AES_256_CBC,
    AES_256_GCM,
    BLOWFISH_THEN_AES_256_CBC,
    CAMELLIA_256_THEN_AES_256_CBC,
    BLOWFISH_THEN_AES_256_GCM,
    CAMELLIA_256_THEN_AES_256_GCM
};


enum class FileObfuscation {
    NO_OBFUSCATION,
    FILE_CONTENTS_SCRAMBLING, //Make it less obvious that its a KV map and possibly of value
    EMBEDDED,
    //Embed the KV map into another file like a dummy binary and hiding it in the heap mapping of the ELF executable etc
    FILE_LOCATION_RANDOMIZATION, //Everything normal but just put the kv file in an odd place
};

struct ConfigRepresentation {
    void parse_command_line_args(std::vector<std::string> args);

    ConfigRepresentation() {
        decrypt = false;
        encryption_mode = EncryptionMode::AES_256_CBC;
        obfuscation = FileObfuscation::NO_OBFUSCATION;
    }

    bool decrypt;
    std::string key;
    std::string value;
    FileObfuscation obfuscation;
    EncryptionMode encryption_mode;
    std::string vault_file_path;

private:
    static void help() {
        std::cout <<
                """This program is meant to be used as a key value vault for secure local password or other short strings."
                << std::endl <<
                "It is intended to be used with the following flags : " << std::endl <<
                "-e -> marks this operation as an ENCRYPT operation. This is default behavior. (requires a key and a value)"
                << std::endl <<
                "-d -> marks this operation a a DECRYPT operation, just requires a key and you will be prompted for your password to retrieve the value"
                << std::endl <<
                "-vf -> precedes a path string, points to a specific location for your vault file, main reason would be if your vault file is in a non-standard location such as a mounted flash drive"
                << std::endl <<
                "-k -> precedes the key to the value you are trying to retreive from the vault" << std::endl <<
                "-v -> precedes the value FOR ENCRYPTION OPERATION ONLY." << std::endl <<
                "-a -> precedes the encryption algorithm you wish to use. Not yet implemented." << std::endl <<
                "-h -> display the help message you are currently reading."""
                << std::endl;
        exit(0);
    }
};


#endif //LIBREVAULT_CONFIG_REPRESENTATION_H
