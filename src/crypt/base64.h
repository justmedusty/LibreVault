//
// Created by dustyn on 6/17/26.
//

#ifndef LIBREVAULT_BASE64_H
#define LIBREVAULT_BASE64_H
#include <string>
#include <openssl/evp.h>
std::string base64_encode(const ::std::string &bindata);
std::string base64_decode(const ::std::string &ascdata);

#endif //LIBREVAULT_BASE64_H