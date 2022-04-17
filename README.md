# OpenOCDRTT

OpenOCDRTT lets you send printf messages from your microcontroller to your PC without the need for an UART or SWO. printf messages are written to a buffer on the RAM which would get fetched by the debugger. RTT will be significantly faster since the CPU only needs to copy the message onto the RAM and doesn't need to wait for transmission to complete, unlike UART or SWO. This library has only been tested for STM32 Cortex M controllers with GCC.

### Getting started

- Modify `openocdrtt_config.h` as per your requirements. Note that the buffers should be atleast 2 bytes long
- At the very beginning of your `main()` function call `openOCDRTTInit();`
- The `_write` function will capture all the messages from your printf/puts/putc functions and write it to the buffer. In case your OS already has a `_write` function, you will need to remove it or mark it as weak with `__attribute__((weak))`
- Run openocd `openocd -f board/stm32l4discovery.cfg -c 'init;reset;sleep 200;rtt setup 0x20000000 20480 "OPENOCD RTT";rtt start;rtt server start 9090 0'`
  - `sleep 200`:  At startup OpenOCD will search for the identifier "OPENOCD RTT" within the target memory. A `sleep 200` is necessary for the MCU to startup and execute the `openOCDRTTInit()` function which will initialize the control block identifier ID.
  - `rtt setup 0x20000000 20480 "OPENOCD RTT"`: Instruct OpenOCD to search the RAM for the control block identifier `OPENOCD RTT`. Dependening upon where the control block is placed, you may need to increase the size from `20480`. The identifier ID is hardcoded to `"OPENOCD RTT"` and should not be changed
  - `rtt start`: OpenOCD will now begin to search for the control block
  - `rtt server start 9090 0`: OpenOCD will start a TCP server on port 9090 and forward all messages on channel 0 to the port.
- You can now connect to the server using `nc 127.1 9090`. If everything has gone well you should be seeing the output of printf here


#### Note:
On a STM32 MCU, if you are putting the MCU to sleep when idle. you will need to add the following code or else OpenOCD will fail to read the memory.

<code>
__HAL_RCC_DMA1_CLK_ENABLE();

HAL_DBGMCU_EnableDBGSleepMode();

HAL_DBGMCU_EnableDBGStopMode();
</code>

[RTT documentation from openocd](https://openocd.org/doc/html/General-Commands.html#Real-Time-Transfer-_0028RTT_0029)
