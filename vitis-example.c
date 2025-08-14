/******************************************************************************
 *
 * Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * Use of the Software is limited solely to applications:
 * (a) running on a Xilinx device, or
 * (b) that interact with a Xilinx device through a bus or interconnect.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of the Xilinx shall not be used
 * in advertising or otherwise to promote the sale, use or other dealings in
 * this Software without prior written authorization from Xilinx.
 *
 ******************************************************************************/

#include "platform.h"
#include "xil_io.h"
#include "xil_printf.h"
#include <stdio.h>

// Base address for AXI-lite control
#define SLV_REG0 0xA0000000
#define SLV_REG1 (SLV_REG0 + 4)
#define SLV_REG2 (SLV_REG0 + 8)
#define SLV_REG3 (SLV_REG0 + 12)

// Base address for accessing device BRAM
#define BRAM_BASE 0xB0000000

typedef unsigned __int128 uint128_t;

// Write to BRAM
void bram_write(uint128_t value, unsigned int bram_addr) {
    volatile uint128_t *bram_ptr = (volatile uint128_t *)BRAM_BASE;

    *((int *)SLV_REG3) = 0b1;   // Enable BRAM access
    bram_ptr[bram_addr] = value;
    *((int *)SLV_REG3) = 0b0;   // Disable BRAM access
}

// Read from BRAM
uint128_t bram_read(unsigned int bram_addr) {
    volatile uint128_t *bram_ptr = (volatile uint128_t *)BRAM_BASE;

    *((int *)SLV_REG3) = 0b1;   // Enable BRAM access
    uint128_t value = bram_ptr[bram_addr];
    *((int *)SLV_REG3) = 0b0;   // Disable BRAM access

    return value;
}

// Helper function to create an uing128_t from two uint64_t values
uint128_t createUint128_t(uint64_t high, uint64_t low) { return ((uint128_t)high << 64) | low; }


int main() {
    init_platform();

    print("Starting\n");
    
    // Writing a value into a register
    *((int *)SLV_REG0) = 42;

    // Reading a value from a register
    int value1 = *((int *)SLV_REG0);

    // Write into BRAM
    uint128_t value2 = createUint128_t(0x0000000000000001, 0x0000000000000002);
    bram_write(value2, 0);

    // Read from BRAM
    uint128_t value3 = bram_read(0);

    print("Done\n");

    cleanup_platform();
    return 0;
}
