/*
 * SPDX-FileCopyrightText: 2022 Arjun <arjunak234@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#define BOOST_TEST_MODULE test module name
#define BOOST_TEST_DYN_LINK
#include "openocdrtt.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_identifier)
{
    const char* const IDENTIFIER = "OPENOCD RTT";

    openOCDRTTInit();
    const auto* const ptr = getRTTBuffer();
    BOOST_CHECK_EQUAL(strncmp(ptr->controlBlockId, IDENTIFIER, strlen(IDENTIFIER)), 0);
}

static_assert(TO_HOST_BUFFER_SIZE == 16, "These tests require buffer size to be 16 to run");

BOOST_AUTO_TEST_CASE(test_writes)
{
    openOCDRTTInit();
    const auto* const ptr = getRTTBuffer();

    _write(0, "abcde", 5);
    BOOST_CHECK_EQUAL(ptr->toHostBuffer->writeIdx, 5);
    _write(0, "fghij", 5);
    BOOST_CHECK_EQUAL(ptr->toHostBuffer->writeIdx, 10);
    _write(0, "klmno", 5);
    BOOST_CHECK_EQUAL(ptr->toHostBuffer->writeIdx, 15);
    _write(0, "pqrst", 5);
    BOOST_CHECK_EQUAL(ptr->toHostBuffer->writeIdx, 4);

    const char* const EXPECTED_STR = "qrstefghijklmnop";
    BOOST_CHECK_EQUAL(strncmp(ptr->toHostBuffer->buffer, EXPECTED_STR, strlen(EXPECTED_STR)), 0);
}

BOOST_AUTO_TEST_CASE(test_buffer_fill_up)
{
    openOCDRTTInit();
    const auto* const ptr = getRTTBuffer();

    const char* const str = "abcdefghijklmno";
    _write(0, str, strlen(str));
    BOOST_CHECK_EQUAL(ptr->toHostBuffer->writeIdx, strlen(str));

    const char* const EXPECTED_STR = "abcdefghijklmno";
    BOOST_CHECK_EQUAL(strncmp(ptr->toHostBuffer->buffer, EXPECTED_STR, strlen(EXPECTED_STR)), 0);
}

BOOST_AUTO_TEST_CASE(test_buffer_overflow)
{
    openOCDRTTInit();
    const auto* const ptr = getRTTBuffer();

    const char* const str = "abcdefghijklmnopqrstuvwxyz";
    _write(0, str, strlen(str));
    BOOST_CHECK_EQUAL(ptr->toHostBuffer->writeIdx, TO_HOST_BUFFER_SIZE - 1);

    BOOST_CHECK_EQUAL(strncmp(ptr->toHostBuffer->buffer, str, TO_HOST_BUFFER_SIZE), 0);
}
