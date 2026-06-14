//
// Created by dustyn on 6/14/26.
//

#ifndef LIBREVAULT_CONFIG_REPRESENTATION_H
#define LIBREVAULT_CONFIG_REPRESENTATION_H

enum class FileObfuscation;
enum class EncryptionMode;

#define ENCRYPTION_MODE_CONFIG_KEY "encryption_mode"
#define FILE_OBFUSCATION_CONFIG_KEY "filesystem_obfuscation"

struct ConfigRepresentation {
  FileObfuscation obfuscation;
    EncryptionMode encryption_mode;

};

enum class FileObfuscation {
    NO_OBFUSCATION,
    FILE_CONTENTS_SCRAMBLING, //Make it less obvious that its a KV map and possibly of value
    EMBEDDED, //Embed the KV map into another file like a dummy binary and hiding it in the heap mapping of the ELF executable etc
    FILE_LOCATION_RANDOMIZATION,//Everything normal but just put the kv file in an odd place
};

enum class EncryptionMode {
    AES_256_GCM,
    BLOWFISH_THEN_AES_256_GCM,
    CAMELLIA_256_THEN_AES_256_GCM
};



#endif //LIBREVAULT_CONFIG_REPRESENTATION_H