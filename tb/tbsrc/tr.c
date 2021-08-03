/***********************************************************************
 * tr.c
 * 
 * C functions to create the VHDL testbench automatically from the
 * SystemC simulation. They use the testbench opened by head.c and write
 * transactions (one function for write transactions and another one for
 * read transactions).
 * 
 * Version 1.0:
 *   Author: unknown
 *   Date: unknown
 * 
 * Version 2.0: added Verilog testbench generation for VIP
 *   Author: Rene van Leuken
 *   Date: unknown
 * 
 * Version 2.1: formatted
 *   Author: David Aledo
 *   Date: 27/03/2019
 */

#include <stdio.h>
#include "../../sw/mmap.h"

extern FILE *tbsrc;
extern FILE *tbsv;

void write_transaction(a, d)
int a, d;
{
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "		-- Generate a write transaction \n");
    fprintf(tbsrc, "		address    <= X\"%.8x\";\n", a);
    fprintf(tbsrc, "		write_data <= X\"%.8x\";\n", d);
    fprintf(tbsrc, "		rnw        <= '0';\n");
    fprintf(tbsrc, "		go         <= '1';\n");
    fprintf(tbsrc, "		wait for AXI_ACLK_period;\n");
    fprintf(tbsrc, "		wait until done = '1';\n");
    fprintf(tbsrc, "		go         <= '0';\n");
    fprintf(tbsrc, "		wait for AXI_ACLK_period;\n");
    fprintf(tbsrc, "		address    <= X\"00000000\";\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "		wait for AXI_ACLK_period;\n");
    fprintf(tbsrc, "		wait for simulation_interval;\n");
    fprintf(tbsrc, "\n");

    fprintf(tbsv, "\n");
    fprintf(tbsv, "\tmaster_agent.AXI4LITE_WRITE_BURST(32'h%.8x,prot,32'h%.8x,resp);\n", a, d);
    fprintf(tbsv, "\t#20ns\n");
    fprintf(tbsv, "\n");

    if (a == SYS_AXI_BASE && d == 0x80)
    {
        fprintf(tbsrc, "		write(my_line, string'(\"IP starts\")); -- formatting\n");
        fprintf(tbsrc, "		writeline(output, my_line);     -- write to \"output\"\n");
        fprintf(tbsrc, "\n");
        fprintf(tbsrc, "		wait until interrupt_request = '1';\n");
        fprintf(tbsrc, "		write(my_line, string'(\"IP finished\")); -- formatting\n");
        fprintf(tbsrc, "		writeline(output, my_line);     -- write to \"output\"\n");
        fprintf(tbsrc, "\n");

        fprintf(tbsv, "\n");
        fprintf(tbsv, "\t$display(\"IP starts\");\n");
        fprintf(tbsv, "\twait (interrupt_request == 1);\n");
        fprintf(tbsv, "\t$display(\"IP finished\");\n");
        fprintf(tbsv, "\n");
    }

}

void read_transaction(a)
int a;
{
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "		-- Generate a read transaction \n");
    fprintf(tbsrc, "		address    <= X\"%.8x\";\n", a);
    fprintf(tbsrc, "		write_data <= X\"00000000\";\n");
    fprintf(tbsrc, "		rnw        <= '1';\n");
    fprintf(tbsrc, "		go         <= '1';\n");
    fprintf(tbsrc, "		wait for AXI_ACLK_period;\n");
    fprintf(tbsrc, "		wait until done = '1';\n");
    fprintf(tbsrc, "		go         <= '0';\n");
    fprintf(tbsrc, "		wait for AXI_ACLK_period;\n");
    fprintf(tbsrc, "		address    <= X\"00000000\";\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "		wait for AXI_ACLK_period;\n");
    fprintf(tbsrc, "		wait for simulation_interval;\n");
    fprintf(tbsrc, "\n");

    fprintf(tbsv, "\n");
    fprintf(tbsv, "\tmaster_agent.AXI4LITE_READ_BURST(32'h%.8x,prot,data_rd,resp);\n", a);
    fprintf(tbsv, "\n");
}
