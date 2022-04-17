/*
 * SPDX-FileCopyrightText: 2022 Arjun <arjunak234@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef OPENOCDRTT_H
#define OPENOCDRTT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

#include "openocdrtt_config.h"

struct OpenOCDRttBuffer {
    const char* name;
    char* buffer;
    uint32_t bufferSize;
    size_t writeIdx;
    volatile size_t readIdx;
    uint32_t flags;
};

#define OPENOCDRTT_BUFFER_COUNT 1

struct OpenOCDRTT {
    char controlBlockId[16];
    const uint32_t toHostBufferCount;
    const uint32_t toTargetBufferCount;
    struct OpenOCDRttBuffer toHostBuffer[OPENOCDRTT_BUFFER_COUNT];
    struct OpenOCDRttBuffer toTargetBuffer[OPENOCDRTT_BUFFER_COUNT];
} __attribute__((packed));

void openOCDRTTInit();

#ifdef __x86_64__
int _write(int fd, const void* const buffer, const int bufferSize);
// This is used by unit tests
struct OpenOCDRTT* getRTTBuffer();
#endif

#ifdef __MBED__
int rtt_write(int fd, const void* const buffer, const int bufferSize);
#endif

#ifdef __cplusplus
}
#endif

#endif // OPENOCDRTT_H
