
//
// Created by dustyn on 6/14/26.
//

#ifndef LIBREVAULT_FILESYSTEM_HANDLING_H
#define LIBREVAULT_FILESYSTEM_HANDLING_H
#include <string>

enum class FileObfuscation {
    NO_OBFUSCATION,

};
struct FilesystemHandling {
    FilesystemHandling(
        const std::string &path,
        const std::string &extension
        );
    ~FilesystemHandling();
}
#endif //LIBREVAULT_FILESYSTEM_HANDLING_H
