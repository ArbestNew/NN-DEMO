/***
 * mmap.h
 * Memory map template
 * Author: David Aledo
 * Date: 16/01/2019
 */

#include <inttypes.h>

//#define RAM_BASEADDR  0xB1000000 // Old stuff that I don't know what it is. I preserve this just in case is useful for somebody
//#define SLV_BASEADDR   0xC0000000

/* Processor's RAM */
// Old values. They can be deleted. They are commented for backward compability with old simulation versions:
#define  SYS_MEM_BASE   0x00000000
#define  SYS_MEM_SIZE   0x03FFFFFF //0x04000000
// Values for the ZedBoard:
//#define  SYS_MEM_BASE   0x00100000
//#define  SYS_MEM_SIZE   (0x1FFFFFFF-0x00100000)

/* Interrupt controller */
// Old values. They can be deleted. They are commented for backward compability with old simulation versions:
#define  SYS_IRQC_BASE   0x17000000
#define  SYS_IRQC_SIZE   0x0000FFFF
// Values for the ZedBoard (SCUGIC):
//#define  SYS_IRQC_BASE   0xF8F00100
//#define  SYS_IRQC_SIZE   0x000000FF //0x00000100

/* Console */
// Old values. They can be deleted. They are commented for backward compability with old simulation versions:
#define  SYS_CONS_BASE   0x40000000
#define  SYS_CONS_SIZE   0x00000FFF
// Values for the ZedBoard (UART1):
//#define  SYS_CONS_BASE   0xE0001000
//#define  SYS_CONS_SIZE   0x000000FF //0x00000100


// NOTE: BASE ADDR of SD card in ZedBoard: 0xE0100000

/* Define here the BASE address and SIZE of your peripherals: */

// Example of file. A file is connected directly to the memory map (i.e. to the TLM bus) by the Memory module 'rfile_mem'
#define  SYS_RFILE_RAM_BASE   0xB0000000
#define  SYS_RFILE_RAM_SIZE   RFILE_SIZE

// Another example of another file
#define  SYS_WFILE_RAM_BASE   0xB1000000
#define  SYS_WFILE_RAM_SIZE   WFILE_SIZE

// Example of text file with numbers
#define  SYS_RWNUM_RAM_BASE   0xB2000000
#define  SYS_RWNUM_RAM_SIZE   10

// ...

/* AXI Bus (to connect IPs) */
// Old values. They can be deleted. They are commented for backward compability with old simulation versions:
//#define  SYS_AXI_BASE   0xC0000000
//#define  SYS_AXI_SIZE   (0xFFFFFFFF - SYS_AXI_BASE) // Its size is all the remaining addresses from its BASE address
// Values for the ZedBoard:
//#define  SYS_AXI_BASE   0x40000000
//#define  SYS_AXI_SIZE   0x3FFFFFFF //0x40000000 //(0x7FFFFFFFU+1U-0x40000000U)
#define  SYS_AXI_BASE   0x60000000
#define  SYS_AXI_SIZE   0x1FFFFFFF

/* Macro to access an address of a peripheral */
#define SYS_MEM32(mem_addr)  (*((volatile uint32_t *)mem_addr)) 

