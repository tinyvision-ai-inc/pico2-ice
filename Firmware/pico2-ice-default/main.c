/*
 * MIT License
 *
 * Copyright (c) 2023 tinyVision.ai
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// pico-sdk
#include <stdio.h>
#include "pico/stdio.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

// pico-ice-sdk
#include "boards.h"
#include "ice_usb.h"
#include "ice_fpga.h"
#include "ice_led.h"

#if PICO_ICE_BOARD_ID == PICO_ICE
#define UART_TX_PIN 0  //ICE_27
#define UART_RX_PIN 1  //ICE_25
#elif PICO_ICE_BOARD_ID == PICO2_ICE
#define UART_TX_PIN 20  //ICE_27
#define UART_RX_PIN 30  //ICE_25
#else
#error "pico-ice board model not found"
#endif

#define DOC_FORWARD_SPI \
"https://pico-ice.tinyvision.ai/group__ice__usb.html#autotoc_md2"

#define DOC_DEFAULT_FIRMWARE \
"https://github.com/tinyvision-ai-inc/pico-ice/tree/main/Firmware/pico-ice-default"

// for repl_ungetchar() to take back the last character from repl_getchar():
int repl_last_char;
bool repl_last_held;

static int repl_getchar(void)
{
    int c;

    if (repl_last_held) {
        repl_last_held = false;
        return repl_last_char;
    }

    c = getchar_timeout_us(0);
    if (c == PICO_ERROR_TIMEOUT)
        return c;

    if (c == '\r' || c == '\n') {
        printf("\r\n");
    } else {
        putchar(c);
    }
    return c;
}

static void repl_ungetchar(int c)
{
    assert(!repl_last_held);
    repl_last_char = c;
    repl_last_held = true;
}

static inline bool repl_parse_error(char *msg, char c)
{
    // reset whatever was being input
    repl_last_held = false;

    printf("\nerror: expected %s got '%c'\n", msg, c);
    return false;
}

static bool repl_parse_newline(void)
{
    int c;

    switch (c = repl_getchar()) {
    case '\r':
    case '\n':
    case EOF:
        return true;
    default:
        repl_ungetchar(c);
        return repl_parse_error("newline", c);
    }
}

static void repl_command_version(void)
{
    if (!repl_parse_newline()) {
        return;
    }
    printf("pico-ice-sdk %s\r\n", VERSION);
}

static void repl_prompt(void)
{
    printf("\x1b[1m%s>\x1b[m ", PICO_ICE_BOARD_NAME);
}

int main(void)
{
    // Configure USB as defined in tusb_config.h
    ice_usb_init();

    // Enable USB-CDC #0 (serial console)
    stdio_init_all();

    // Enable the physical UART
    uart_init(uart0, 115200);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Let the FPGA start
    ice_fpga_init(12);
    ice_fpga_start();

    // Prevent the LEDs from glowing slightly
    ice_led_init();

    // Print repl prompt
    repl_prompt();

    while (true) {
        tud_task();

        int chr = repl_getchar();
        if (chr == PICO_ERROR_TIMEOUT)
            continue;

        // not timeout, something received

        switch (chr) {
        case 'v':
            repl_command_version();
            break;
        default:
            printf("\r\n");
            printf("pico-ice default firmware\r\n", VERSION);
            printf("   %s\r\n", DOC_DEFAULT_FIRMWARE);
            printf("\r\n");
            printf("Serial port #0 - this shell, with commands:\r\n");
            printf("  v - print pico-ice-sdk version\r\n");
            printf("\r\n");
            printf("Serial port #1 - forwarding to UART\r\n");
            printf("  UART TX on RP%d = ICE27\r\n", UART_TX_PIN );
            printf("  UART RX on RP%d = ICE25\r\n", UART_RX_PIN );
            printf("\r\n");
            printf("Serial port #2 - forwarding to SPI:\r\n");
            printf("  %s\r\n", DOC_FORWARD_SPI);
            printf("\r\n");
            break;
        }

        // reprint prompt
        repl_prompt();
    }

    return 0;
}
