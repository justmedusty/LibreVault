//
// Created by dustyn on 6/24/26.
//
#define BOOST_TEST_MODULE KeyDerivationTest
#include <boost/test/unit_test.hpp>
#include "crypt/key_derivation.h"

#include "config/config_representation.h"
#include "crypt/base64.h"
#include "crypt/algo/aes.h"

BOOST_AUTO_TEST_CASE(check_salt_Size) {
    auto salt = generate_salt();
    BOOST_CHECK_EQUAL(salt.size(), KDF_SALT_SIZE_BYTES);
}

BOOST_AUTO_TEST_CASE(check_salt_contents) {
    const auto salt = generate_salt();
    const auto salt2 = generate_salt();

    BOOST_CHECK_EQUAL(salt.size(), salt2.size());
}


BOOST_AUTO_TEST_CASE(check_salt_difference) {
    const auto salt = generate_salt();
    const auto salt2 = generate_salt();

    auto equal = true;
    for (int i = 0; i < salt2.size(); i++) {
        if (salt2[i] != salt[i]) equal = false;
        std::cout << salt[i] << ':' << salt2[i] << std::endl;
    }

    BOOST_CHECK_EQUAL(equal, false);
}

BOOST_AUTO_TEST_CASE(base64_test) {
    std::string rand(100, '\0');
    const auto rand_ptr = reinterpret_cast<unsigned char *>(rand.data());
    RAND_bytes(rand_ptr, 99);

    std::string str1 = Base64::base64_encode(rand);
    std::string str2 = Base64::base64_encode(rand);

    std::cout << str1 << " : " << str2 << std::endl;

    BOOST_CHECK_EQUAL(str1, str2);
}


BOOST_AUTO_TEST_CASE(base64_encoding) {
    std::string data = "HELLO WOW THIS IS A TEST STRING";


    std::string str1 = Base64::base64_encode(data);
    std::string str2 = Base64::base64_decode(str1);


    std::cout << data << " : " << str1 << " : " << str2 << std::endl;

    BOOST_CHECK_EQUAL(data, str2);
}

BOOST_AUTO_TEST_CASE(argon2) {
    std::string password = "superSecretPassWoRd!@#$%^&";
    std::vector<uint8_t> salt = generate_salt();
    std::vector<std::byte> derived_key(32);
    auto ret1 = derive_key(password, salt, derived_key, Defcon::DEFCON5);

    std::string key_str(derived_key.begin(), derived_key.end());
    std::string b64_key = Base64::base64_encode(key_str);
    std::cout << "Base64 key: " << b64_key << std::endl;

    BOOST_CHECK_GT(derived_key.size(), 0);
}

BOOST_AUTO_TEST_CASE(argon2_diff_passwd_check) {
    std::string password = "SuperSecretPassWoRd!@#$%^&";
    std::string password2 = "superSecretPassWoRd!@#$%^&";
    std::vector<uint8_t> salt = generate_salt();
    std::vector<std::byte> derived_key(32);
    std::vector<std::byte> derived_key2(32);

    auto ret1 = derive_key(password, salt, derived_key, Defcon::DEFCON5);
    auto ret2 = derive_key(password2, salt, derived_key2, Defcon::DEFCON5);

    std::string key_str(derived_key.begin(), derived_key.end());
    std::string key_str2(derived_key2.begin(), derived_key2.end());
    std::string b64_key = Base64::base64_encode(key_str);
    std::string b64_key2 = Base64::base64_encode(key_str2);
    std::cout << "Base64 key 1: " << b64_key << std::endl;
    std::cout << "Base64 key 2: " << b64_key2 << std::endl;


    BOOST_CHECK_NE(b64_key, b64_key2);
}

BOOST_AUTO_TEST_CASE(argon2_same_passwd_check) {
    std::string password = "SuperSecretPassWoRd!@#$%^&";
    std::vector<uint8_t> salt = generate_salt();

    std::vector<std::byte> derived_key(32);
    std::vector<std::byte> derived_key2(32);
    auto ret1 = derive_key(password, salt, derived_key, Defcon::DEFCON5);
    auto ret2 = derive_key(password, salt, derived_key2, Defcon::DEFCON5);

    std::string key_str(derived_key.begin(), derived_key.end());
    std::string key_str2(derived_key2.begin(), derived_key2.end());
    std::string b64_key = Base64::base64_encode(key_str);
    std::string b64_key2 = Base64::base64_encode(key_str2);
    std::cout << "Base64 key 1: " << b64_key << std::endl;
    std::cout << "Base64 key 2: " << b64_key2 << std::endl;


    BOOST_CHECK_EQUAL(b64_key, b64_key2);
}
