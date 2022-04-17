/*
 * SPDX-FileCopyrightText: 2022 Arjun <arjunak234@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "openocdrtt.h"
#include <cstring>
#include <iostream>
#include <limits>
#include <syslog.h>
int main()
{
    std::string input;
    std::cin >> input;

    openOCDRTTInit();

    _write(0, input.c_str(), input.length());
    return 0;
}
