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

/*
 *  Each section will have it's own password, this can let you pull more than 1 from a section in 1 operation.
 */
#define LIBREVAULT_NUM_SECTIONS 5
#define LIBREVAULT_DEFCON_1 "<DEFCON1>"
#define LIBREVAULT_DEFCON_2 "<DEFCON2>"
#define LIBREVAULT_DEFCON_3 "<DEFCON3>"
#define LIBREVAULT_DEFCON_4 "<DEFCON4>"
#define LIBREVAULT_DEFCON_5 "<DEFCON5>"

#define LIBREVAULT_VAULT_SIG_START "<-- sig="
//The encrypted string to be placed under each defcon header to be a source of truth for the correctness of a password against a DEFCON level
#define LIBREVAULT_ENCRYPTION_STRING "BECAUSE_I_CHOOSE_TO"
#define LIBREVAULT_VAULT_SIG_END " -->"

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

enum class Defcon {
    DEFCON1,
    DEFCON2,
    DEFCON3,
    DEFCON4,
    DEFCON5,
};


struct ConfigRepresentation {
    void parse_command_line_args(std::vector<std::string> args);

    ConfigRepresentation() {
        decrypt = false;
        encryption_mode = EncryptionMode::AES_256_CBC;
        obfuscation = FileObfuscation::NO_OBFUSCATION;
    }

    ~ConfigRepresentation() {
    }

    bool decrypt;
    std::string key;
    std::string value;
    FileObfuscation obfuscation;
    EncryptionMode encryption_mode;
    std::string vault_file_path;
    Defcon defcon;

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
                "-h -> display the help message you are currently reading." << std::endl <<
                "" << std::endl <<
                "This program has 5 separate levels of the vault:" << std::endl <<
                "DEFCON1 -> MOST SERIOUS SECRETS, SHOULD HAVE FEW ENTRIES AND MAXIMALLY COMPLEX PASSWORD" << std::endl
                <<
                "DEFCON2 -> VERY SERIOUS SECRETS, SHOULD ALSO HAVE FEW ENTRIES AND A MAXIMALLY COMPLEX PASSWORD" <<
                std::endl <<
                "DEFCON3 -> MID-LEVEL SERIOUS SECRETS, SHOULD HAVE A COMPLEX PASSWORD, COULD BE WRITTEN DOWN ON PAPER"
                << std::endl <<
                "DEFCON4 -> LESS-SERIOUS, ONLINE ACCOUNT PASSWORDS AND THINGS OF THIS NATURE, COULD BE WRITTEN DOWN ON PAPER" << std::endl <<
                "DEFCON5 -> LEAST SERIOUS, PASSWORD DOES NOT NEED TO BE VERY COMPLEX JUST ENOUGH TO KEEP LOOKY LOOS OUT, COULD BE WRITTEN DOWN ON PAPER"
                ""
                << std::endl;
        exit(0);
    }
};


#endif //LIBREVAULT_CONFIG_REPRESENTATION_H
