//
// Created by dustyn on 6/14/26.
//

#ifndef LIBREVAULT_CONFIG_REPRESENTATION_H
#define LIBREVAULT_CONFIG_REPRESENTATION_H
#include <filesystem>


enum class FileObfuscation;
enum class EncryptionMode;

#define ENCRYPTION_MODE_CONFIG_KEY "encryption_mode"
#define FILE_OBFUSCATION_CONFIG_KEY "filesystem_obfuscation"

struct ConfigRepresentation {
    void parseCommandLineArgs(int argc, char *argv[]);

    ConfigRepresentation(FileObfuscation obfuscation, EncryptionMode encryption_mode, FILE *vault_file)
        : obfuscation(obfuscation),
          encryption_mode(encryption_mode)
          , vault_file(vault_file) {
    }

    FileObfuscation obfuscation;
    EncryptionMode encryption_mode;
    FILE *vault_file;

private:
    void parseEncryptionModeArgs(int numFlag,char *argTuple[]);
    void parseFileObfuscationArgs(int numFlag,char *argTuple[]);
};

#define FILE_NO_OBFUSCATION_CONFIG_KEY "no_obfuscation"
#define FILE_SCRAMBLING_CONFIG_KEY "file_scrambling"
#define FILE_EMBEDDED_CONFIG_KEY "embedded-in-another-file"
#define FILE_LOCATION_RANDOMIZATION_CONFIG_KEY "file-location-randomization"

enum class FileObfuscation {
    NO_OBFUSCATION,
    FILE_CONTENTS_SCRAMBLING, //Make it less obvious that its a KV map and possibly of value
    EMBEDDED,
    //Embed the KV map into another file like a dummy binary and hiding it in the heap mapping of the ELF executable etc
    FILE_LOCATION_RANDOMIZATION, //Everything normal but just put the kv file in an odd place
};

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


#endif //LIBREVAULT_CONFIG_REPRESENTATION_H
