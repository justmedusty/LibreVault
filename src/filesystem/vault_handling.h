//
// Created by dustyn on 6/16/26.
//

#ifndef LIBREVAULT_VAULT_HANDLING_H
#define LIBREVAULT_VAULT_HANDLING_H
#include <string>
#include "config/config_representation.h"
#include <sstream>

#include <fstream>

#include <iostream>

#define MAXIMUM_VAULT_SIZE (1024 * 1024 * 1024) // This is arbitrary but because we are reading the entire file into memory we need a sanity check, 1MB is absolutely massive so this should never happen


void write_entry(std::string &key, std::string &value, ConfigRepresentation &config);

bool read_entry(std::string &key, std::string &value, ConfigRepresentation &config);

void delete_entry(std::string &key, ConfigRepresentation &config);

void create_vault(std::filesystem::path &vault_path);
#endif //LIBREVAULT_VAULT_HANDLING_H
