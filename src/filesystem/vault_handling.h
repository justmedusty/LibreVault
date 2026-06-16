//
// Created by dustyn on 6/16/26.
//

#ifndef LIBREVAULT_VAULT_HANDLING_H
#define LIBREVAULT_VAULT_HANDLING_H
#include <string>
#include "config/config_representation.h"

void write_entry(std::string&key, std::string& value);
void read_entry(std::string&key, std::string& value, ConfigRepresentation& config);
#endif //LIBREVAULT_VAULT_HANDLING_H
