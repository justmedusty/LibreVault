//
// Created by dustyn on 6/16/26.
//

#include "vault_handling.h"


void write_entry(std::string &key, std::string &value, ConfigRepresentation &config) {
    std::ifstream vault(config.vault_file_path);
    if (!vault.is_open()) {
        std::cerr << "Could not open vault file: " << config.vault_file_path << std::endl;
        exit(1);
    }

    if (std::filesystem::file_size(config.vault_file_path) > MAXIMUM_VAULT_SIZE) {
        std::cerr << std::filesystem::file_size(config.vault_file_path)
                << " is too large for LibreVault to handle. Please investigate." << std::endl;
        exit(1);
    }

    std::filesystem::path temp_path = config.vault_file_path;
    temp_path += ".tmp";
    std::ofstream temp(temp_path);
    if (!temp.is_open()) {
        std::cerr << "Could not create temp file" << std::endl;
        exit(1);
    }

    std::string line;
    bool found = false;

    while (std::getline(vault, line)) {
        std::string k = line.substr(0, line.find('='));
        temp << line << '\n';
    }

    temp << key << '=' << value << '\n';

    vault.close();
    temp.close();

    std::filesystem::remove(config.vault_file_path);
    // gives you one chance if you fuck up
    std::filesystem::rename(temp_path, config.vault_file_path);

}


void read_entry(std::string &key, std::string &value, ConfigRepresentation &config) {
    std::ifstream vault(config.vault_file_path);

    if (!vault.is_open()) {
        std::cerr << "Could not open fault file : " << config.vault_file_path << std::endl;
        exit(1);
    }

    if (std::filesystem::file_size(config.vault_file_path) > MAXIMUM_VAULT_SIZE) {
        std::cerr << std::filesystem::file_size(config.vault_file_path) <<
                " is too large for LibreVault to handle. Please investigate." << std::endl;
        exit(1);
    }

    std::string line;

    while (std::getline(vault, line)) {
        std::string k = line.substr(0, line.find('='));
        std::string v = line.substr(line.find('=') + 1, line.at(line.size() - 1));

        std::cout << k << ":" << v << std::endl;

        if (k == key) {
            value = v;
            return;
        }
    }
}

void delete_entry(std::string &key, ConfigRepresentation &config) {
    std::ifstream vault(config.vault_file_path);
    if (!vault.is_open()) {
        std::cerr << "Could not open vault file: " << config.vault_file_path << std::endl;
        exit(1);
    }

    if (std::filesystem::file_size(config.vault_file_path) > MAXIMUM_VAULT_SIZE) {
        std::cerr << std::filesystem::file_size(config.vault_file_path)
                << " is too large for LibreVault to handle. Please investigate." << std::endl;
        exit(1);
    }

    std::filesystem::path temp_path = config.vault_file_path;
    temp_path += ".tmp";
    std::ofstream temp(temp_path);
    if (!temp.is_open()) {
        std::cerr << "Could not create temp file" << std::endl;
        exit(1);
    }

    std::string line;
    bool found = false;

    while (std::getline(vault, line)) {
        std::string k = line.substr(0, line.find('='));

        if (k == key) {
            found = true;
            continue;
        }

        temp << line << '\n';
    }

    vault.close();
    temp.close();

    std::filesystem::rename(config.vault_file_path, config.vault_file_path + ".old");
    // gives you one chance if you fuck up
    std::filesystem::rename(temp_path, config.vault_file_path);

    if (!found) {
        std::cerr << "Key not found: " << key << std::endl;
    }
}
