//
// Created by dustyn on 6/17/26.
//

#ifndef CITADEL_BASE64_H
#define CITADEL_BASE64_H
#include <string>
#include <openssl/evp.h>

namespace Base64 {
    std::string base64_encode(const ::std::string &bindata);

    std::string base64_decode(const ::std::string &ascdata);
}
#endif //CITADEL_BASE64_H
