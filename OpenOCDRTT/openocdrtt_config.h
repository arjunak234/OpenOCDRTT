/*
 * SPDX-FileCopyrightText: 2022 Arjun <arjunak234@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef OPENOCDRTT_CONFIG_H
#define OPENOCDRTT_CONFIG_H

#ifdef __x86_64__
// Unit tests on Desktop require buffer size to be 16
#define TO_HOST_BUFFER_SIZE 16
#endif

// Note: Buffer size should be atleast 2 bytes long

#ifndef TO_HOST_BUFFER_SIZE
#define TO_HOST_BUFFER_SIZE 128
#endif

#ifndef TO_TARGET_BUFFER_SIZE
#define TO_TARGET_BUFFER_SIZE 2
#endif

#endif // OPENOCDRTT_CONFIG_H
