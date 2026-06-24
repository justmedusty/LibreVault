//
// Created by dustyn on 6/24/26.
//
#define BOOST_TEST_MODULE EncryptionTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(always_passes)
{
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(simple_equality)
{
    BOOST_CHECK_EQUAL(2 + 2, 4);
}

BOOST_AUTO_TEST_CASE(string_compare)
{
    std::string a = "hello";
    std::string b = "hello";
    BOOST_CHECK_EQUAL(a, b);
}