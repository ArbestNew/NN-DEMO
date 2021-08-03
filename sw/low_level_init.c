/*****************************************************************************
* Product: Low-level initialization for AT91SAM7S-EK
* Date of the Last Update:  May 16, 2007
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2007 Quantum Leaps, LLC. All rights reserved.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/

#include <stdint.h>

void low_level_init(void (*reset_addr)(), void (*return_addr)()) {
    extern uint8_t __ram_start;

    static uint32_t const LDR_PC_PC = 0x00010000U;
    static uint32_t const MAGIC = 0xDEADBEEFU;

    *(uint32_t volatile *)(&__ram_start + 0x00) = LDR_PC_PC | 0x18;
    *(uint32_t volatile *)(&__ram_start + 0x04) = LDR_PC_PC | 0x18;
    *(uint32_t volatile *)(&__ram_start + 0x08) = LDR_PC_PC | 0x18;
    *(uint32_t volatile *)(&__ram_start + 0x0C) = LDR_PC_PC | 0x18;
    *(uint32_t volatile *)(&__ram_start + 0x10) = LDR_PC_PC | 0x18;
    *(uint32_t volatile *)(&__ram_start + 0x14) = MAGIC;
    *(uint32_t volatile *)(&__ram_start + 0x18) = LDR_PC_PC | 0x18;
    *(uint32_t volatile *)(&__ram_start + 0x1C) = LDR_PC_PC | 0x18;


    *(uint32_t volatile *)(&__ram_start + 0x20) = (uint32_t)reset_addr;
    *(uint32_t volatile *)(&__ram_start + 0x24) = 0x04U;
    *(uint32_t volatile *)(&__ram_start + 0x28) = 0x08U;
    *(uint32_t volatile *)(&__ram_start + 0x2C) = 0x0CU;
    *(uint32_t volatile *)(&__ram_start + 0x30) = 0x10U;
    *(uint32_t volatile *)(&__ram_start + 0x34) = 0x14U;
    *(uint32_t volatile *)(&__ram_start + 0x38) = 0x18U;
    *(uint32_t volatile *)(&__ram_start + 0x3C) = 0x1CU;
}
