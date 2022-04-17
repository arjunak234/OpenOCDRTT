#include "mbed.h"
#include "openocdrtt.h"

class OpenOCDRTTOutput : public FileHandle {
public:
    ssize_t read(void* buffer, size_t size);
    ssize_t write(const void* buffer, size_t size);
    off_t seek(off_t offset, int whence);
    int close();
};

ssize_t OpenOCDRTTOutput::read(void* buffer, size_t size)
{
    return -1;
}

ssize_t OpenOCDRTTOutput::write(const void* buffer, size_t size)
{
    return rtt_write(0, buffer, size);
}

off_t OpenOCDRTTOutput::seek(off_t offset, int whence)
{
    return -1;
}

int OpenOCDRTTOutput::close()
{
    return -1;
}

FileHandle* mbed::mbed_override_console(int)
{
    openOCDRTTInit();
    static OpenOCDRTTOutput rtt;
    return &rtt;
}

int main()
{
    __HAL_RCC_DMA1_CLK_ENABLE();
    HAL_DBGMCU_EnableDBGSleepMode();
    HAL_DBGMCU_EnableDBGStopMode();

    DigitalOut led(LED1);
    int ctr {};

    while (true) {
        ctr++;

        printf("%d\n", ctr);
        led = !led;
        ThisThread::sleep_for(1s);
    }
}
