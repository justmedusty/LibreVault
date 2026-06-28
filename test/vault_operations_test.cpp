//
// Created by dustyn on 6/28/26.
//
#define BOOST_TEST_MODULE VaultOperationsTest
#include <boost/test/unit_test.hpp>
#include "filesystem/vault_handling.h"
#include "crypt/encryption.h"
#include "crypt/base64.h"
#include "config/config_representation.h"

BOOST_AUTO_TEST_CASE(arg_parsing_check_with_vault_creation) {
    std::filesystem::path vault_file_path = "/tmp/vault";
    std::filesystem::remove(vault_file_path); //if tests fail it can leave the dead files there so we must do this
    create_vault(vault_file_path);
    ConfigRepresentation config_representation(vault_file_path);
    const std::vector<std::string> args = {
        FLAG_ENCRYPT,
        FLAG_KEY,
        "my_secret",
        FLAG_VALUE,
        "test_secret123",
        FLAG_DEFCON_LEVEL_TO_ENCRYPT,
        "1",
    };

    config_representation.parse_command_line_args(args);


    BOOST_CHECK_EQUAL(config_representation.vault_file_path, "/tmp/vault");
    BOOST_CHECK_EQUAL(config_representation.decrypt, false);
    BOOST_CHECK_EQUAL(config_representation.value, "test_secret123");
    BOOST_CHECK_EQUAL(config_representation.key, "my_secret");

    std::filesystem::remove(vault_file_path);
}

BOOST_AUTO_TEST_CASE(arg_parsing_check2_with_vault_creation) {
    std::filesystem::path vault_file_path = "/tmp/vault";
    std::filesystem::remove(vault_file_path); //if tests fail it can leave the dead files there so we must do this
    create_vault(vault_file_path);
    ConfigRepresentation config_representation(vault_file_path);
    const std::vector<std::string> args = {
        FLAG_DECRYPT,
        FLAG_KEY,
        "this_secret",
    };

    config_representation.parse_command_line_args(args);


    BOOST_CHECK_EQUAL(config_representation.vault_file_path, "/tmp/vault");
    BOOST_CHECK_EQUAL(config_representation.decrypt, true);
    BOOST_CHECK_EQUAL(config_representation.key, "this_secret");
    BOOST_CHECK(config_representation.value.empty());

    std::filesystem::remove(vault_file_path);
}

