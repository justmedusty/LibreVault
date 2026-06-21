//
// Created by dustyn on 6/16/26.
//

#include "vault_handling.h"

std::string get_decfon_signature(std::string &vault_file_path, Defcon defcon) {
}

void is_vault_setup(std::string &vault_file_path) {
    std::ifstream vault(vault_file_path);
    if (!vault.is_open()) {
        std::cerr << "Could not open vault file : " << vault_file_path << std::endl;
        exit(1);
    }

    std::string line;
    int found = 0;
    int sig = 0;

    while (std::getline(vault, line)) {
        if (line.starts_with('<')) {
            if (line == LIBREVAULT_DEFCON_1) {
                found++;
                continue;
            }

            if (line == LIBREVAULT_DEFCON_2) {
                found++;
                continue;
            }

            if (line == LIBREVAULT_DEFCON_3) {
                found++;
                continue;
            }

            if (line == LIBREVAULT_DEFCON_4) {
                found++;
                continue;
            }

            if (line == LIBREVAULT_DEFCON_5) {
                found++;
                continue;
            }
        }

        if (line.starts_with(LIBREVAULT_VAULT_SIG_START)) {
            /*
             *  We won't check that the whole signature is there , it will be unless someone is playing around with their own vault file
             */
            sig++;
        }
    }

    if (found > 0 && found != 5) {
        std::cerr << vault_file_path <<
                " contains a vault file, but it is invalid! You will need to fix it or reset your vault!" <<
                std::endl;
        exit(1);
    }

    if (found > 0 || sig > 0) {
        std::cerr <<
                "Not all defcon levels have an associated verification signature. This will not prevent this program from running, but you will need to set a password for any DEFCON section without an associated signature."
                << std::endl;
    }
}

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
        if (k == key) {
            std::cerr << k << " clashes with your new key : " << key << ". Aborting." << '\n';
            exit(1);
        }
        temp << line << '\n';
    }

    temp << key << '=' << value << '\n';

    vault.close();
    temp.close();

    std::filesystem::remove(config.vault_file_path);
    std::filesystem::rename(temp_path, config.vault_file_path);
}

/*
 *  Finds an entry and reads it into value.
 */
bool read_entry(std::string &key, std::string &value, ConfigRepresentation &config) {
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
        if (line.starts_with('#')) // support comments
            continue;

        std::string k = line.substr(0, line.find('='));
        std::string v = line.substr(line.find('=') + 1, line.at(line.size() - 1));

        std::cout << k << ":" << v << std::endl;

        if (k == key) {
            value = v;
            return true;
        }
    }
    return false;
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
        if (line.starts_with('#')) {
            // support comments
            temp << line << '\n';
            continue;
        }
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
        exit(1);
    }

    std::cout << "Key : " << key << "removed." <<
            "This deletion is recoverable until your next deletion and the backup is located at " << config.
            vault_file_path << ".old" << std::endl;
}

void create_vault(std::filesystem::path &vault_path) {
    if (std::filesystem::exists(vault_path)) {
        std::cerr << vault_path << " already exists!" << std::endl;
        exit(1);
    }
    std::ofstream vault(vault_path);
    if (!vault.is_open()) {
        std::cerr << "Could not open vault file: " << vault_path << std::endl;
        exit(1);
    }

    vault << "#LibreVault vault file" << std::endl;
    vault << LIBREVAULT_DEFCON_1 << std::endl;
    vault << LIBREVAULT_DEFCON_2 << std::endl;
    vault << LIBREVAULT_DEFCON_3 << std::endl;
    vault << LIBREVAULT_DEFCON_4 << std::endl;
    vault << LIBREVAULT_DEFCON_5 << std::endl;
    vault.close();
}
