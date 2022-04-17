/*
 * SPDX-FileCopyrightText: 2022 Arjun <arjunak234@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "openocdrtt.h"
#ifdef __x86_64__
void __DSB();
void __DSB() { }
#else
#include "cmsis_compiler.h"
#endif
#include "openocdrtt_config.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#if TO_TARGET_BUFFER_SIZE < 2 || TO_HOST_BUFFER_SIZE < 2
#error Buffer size must be atleast 2 bytes long
#endif

static char toHostBufferMemory[TO_HOST_BUFFER_SIZE];
static char toTargetMemBuffer[TO_TARGET_BUFFER_SIZE];

static struct OpenOCDRTT oRTT = { .toHostBufferCount = 1, .toTargetBufferCount = 1 };

static int inited;

void openOCDRTTInit()
{
    // OpenOCD finds the control block by search for "OPENOCD RTT" in memory
    // In order to prevent it from matching with this variable, we increment
    // all characters by 1.
    char const* const CONTROL_BLOCK_ID = "PQFOPDE!SUU";
    const size_t controlBlockIdLen = 11;

    for (size_t i = 0; i < controlBlockIdLen; i++) {
        oRTT.controlBlockId[i] = CONTROL_BLOCK_ID[i] - 1;
    }

    oRTT.toHostBuffer[0].buffer = toHostBufferMemory;
    oRTT.toHostBuffer[0].bufferSize = TO_HOST_BUFFER_SIZE;
    oRTT.toHostBuffer[0].name = "ToHostBuffer";
    oRTT.toHostBuffer[0].writeIdx = 0;

    oRTT.toTargetBuffer[0].buffer = toTargetMemBuffer;
    oRTT.toTargetBuffer[0].bufferSize = TO_TARGET_BUFFER_SIZE;
    oRTT.toTargetBuffer[0].name = "Unsupported";

    inited = 1;

    __DSB();
}

int _write(int fd, const void* const buffer, const int bufferSize)
{
    (void)fd;
    if (!bufferSize) {
        return 0;
    }

    const size_t uBufferSize = (size_t)bufferSize;
    const char* const ptr = (const char*)buffer;
    size_t ptrIdx = 0;

    assert(inited);

    const size_t startIdx = oRTT.toHostBuffer->writeIdx;
    size_t wIdx = startIdx;

    // In the first pass try to write as much as possible till the end of the buffer.
    size_t available = (TO_HOST_BUFFER_SIZE - startIdx - (startIdx == 0));
    size_t bytesToWrite = available < uBufferSize ? available : uBufferSize;

    memcpy(oRTT.toHostBuffer->buffer + wIdx, ptr, bytesToWrite);

    wIdx += bytesToWrite;
    ptrIdx += bytesToWrite;

    assert(wIdx <= TO_HOST_BUFFER_SIZE);

    if (wIdx == TO_HOST_BUFFER_SIZE) {
        wIdx = 0;
    }

    if (bytesToWrite == uBufferSize || startIdx == 0) {
        __DSB();
        oRTT.toHostBuffer->writeIdx = wIdx;
        return bufferSize;
    }

    assert(ptrIdx < uBufferSize);

    // in second pass, wrap around and write till (startIdx - 1)
    const size_t leftToWrite = (uBufferSize - bytesToWrite);
    bytesToWrite = leftToWrite < (startIdx - 1) ? leftToWrite : (startIdx - 1);

    memcpy(oRTT.toHostBuffer->buffer + wIdx, ptr + ptrIdx, bytesToWrite);

    wIdx += bytesToWrite;

    assert(wIdx < TO_HOST_BUFFER_SIZE);
    __DSB();
    oRTT.toHostBuffer->writeIdx = wIdx;

    return bufferSize;
}

#ifdef __x86_64__
struct OpenOCDRTT* getRTTBuffer()
{
    return &oRTT;
}
#endif
