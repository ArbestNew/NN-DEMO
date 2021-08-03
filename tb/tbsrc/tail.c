/***********************************************************************
 * tail.c
 * 
 * C function to create the VHDL testbench automatically from the
 * SystemC simulation. It uses the testbench file opened by head.c,
 * writes its "tail" (i.e. ending lines) and closes the file.
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

extern FILE *tbsrc;
extern FILE *tbsv;

void tail()
{
    fprintf (tbsrc, "\n");
    fprintf (tbsrc, "		write(my_line, string'(\"Simulation ends\")); -- formatting\n");
    fprintf (tbsrc, "		writeline(output, my_line);     -- write to \"output\"\n");
    fprintf (tbsrc, "\n");
    fprintf (tbsrc, "		-- end of stimuli.  give some time to finish up.\n");
    fprintf (tbsrc, "		wait for simulation_interval * 5;\n");
    fprintf (tbsrc, "\n");
    fprintf (tbsrc, "		wait;\n");
    fprintf (tbsrc, "	end process stimulus;\n");
    fprintf (tbsrc, "\n");
    fprintf (tbsrc, "	iaxislave : axislave\n");
    fprintf (tbsrc, "		port map(\n");
    fprintf (tbsrc, "			axi_aclk          => axi_aclk,\n");
    fprintf (tbsrc, "			axi_aresetn       => axi_aresetn,\n");
    fprintf (tbsrc, "			s_axi_awaddr      => axi_awaddr,\n");
    fprintf (tbsrc, "			s_axi_awprot      => axi_awprot,\n");
    fprintf (tbsrc, "			s_axi_awvalid     => axi_awvalid,\n");
    fprintf (tbsrc, "			s_axi_awready     => axi_awready,\n");
    fprintf (tbsrc, "			s_axi_wdata       => axi_wdata,\n");
    fprintf (tbsrc, "			s_axi_wstrb       => axi_wstrb,\n");
    fprintf (tbsrc, "			s_axi_wvalid      => axi_wvalid,\n");
    fprintf (tbsrc, "			s_axi_wready      => axi_wready,\n");
    fprintf (tbsrc, "			s_axi_bresp       => axi_bresp,\n");
    fprintf (tbsrc, "			s_axi_bvalid      => axi_bvalid,\n");
    fprintf (tbsrc, "			s_axi_bready      => axi_bready,\n");
    fprintf (tbsrc, "			s_axi_araddr      => axi_araddr,\n");
    fprintf (tbsrc, "			s_axi_arprot      => axi_arprot,\n");
    fprintf (tbsrc, "			s_axi_arvalid     => axi_arvalid,\n");
    fprintf (tbsrc, "			s_axi_arready     => axi_arready,\n");
    fprintf (tbsrc, "			s_axi_rdata       => axi_rdata,\n");
    fprintf (tbsrc, "			s_axi_rresp       => axi_rresp,\n");
    fprintf (tbsrc, "			s_axi_rvalid      => axi_rvalid,\n");
    fprintf (tbsrc, "			s_axi_rready      => axi_rready,\n");
    fprintf (tbsrc, "			interrupt_request => interrupt_request);\n");
    fprintf (tbsrc, "\n");
    fprintf (tbsrc, "	axi_master_model : axi_lite_master_transaction_model\n");
    fprintf (tbsrc, "			--	generic map\n");
    fprintf (tbsrc, "			--		(\n");
    fprintf (tbsrc, "			-- 		)\n");
    fprintf (tbsrc, "		port map(\n");
    fprintf (tbsrc, "			go                 => go,\n");
    fprintf (tbsrc, "			busy               => busy,\n");
    fprintf (tbsrc, "			done               => done,\n");
    fprintf (tbsrc, "			rnw                => rnw,\n");
    fprintf (tbsrc, "			address            => address,\n");
    fprintf (tbsrc, "			write_data         => write_data,\n");
    fprintf (tbsrc, "			read_data          => read_data,\n");
    fprintf (tbsrc, "			m_axi_lite_aclk    => axi_aclk,\n");
    fprintf (tbsrc, "			m_axi_lite_aresetn => axi_aresetn,\n");
    fprintf (tbsrc, "			m_axi_lite_arready => axi_arready,\n");
    fprintf (tbsrc, "			m_axi_lite_arvalid => axi_arvalid,\n");
    fprintf (tbsrc, "			m_axi_lite_araddr  => axi_araddr,\n");
    fprintf (tbsrc, "			m_axi_lite_rready  => axi_rready,\n");
    fprintf (tbsrc, "			m_axi_lite_rvalid  => axi_rvalid,\n");
    fprintf (tbsrc, "			m_axi_lite_rdata   => axi_rdata,\n");
    fprintf (tbsrc, "			m_axi_lite_rresp   => axi_rresp,\n");
    fprintf (tbsrc, "			m_axi_lite_awready => axi_awready,\n");
    fprintf (tbsrc, "			m_axi_lite_awvalid => axi_awvalid,\n");
    fprintf (tbsrc, "			m_axi_lite_awaddr  => axi_awaddr,\n");
    fprintf (tbsrc, "			m_axi_lite_wready  => axi_wready,\n");
    fprintf (tbsrc, "			m_axi_lite_wvalid  => axi_wvalid,\n");
    fprintf (tbsrc, "			m_axi_lite_wdata   => axi_wdata,\n");
    fprintf (tbsrc, "			m_axi_lite_wstrb   => axi_wstrb,\n");
    fprintf (tbsrc, "			m_axi_lite_bready  => axi_bready,\n");
    fprintf (tbsrc, "			m_axi_lite_bvalid  => axi_bvalid,\n");
    fprintf (tbsrc, "			m_axi_lite_bresp   => axi_bresp\n");
    fprintf (tbsrc, "		);\n");
    fprintf (tbsrc, "\n");
    fprintf (tbsrc, "end;\n");

    fprintf(tbsv, "\t$display(\"Simulation ends\");\n");
    fprintf(tbsv, "\tend\n");
    fprintf(tbsv, "endmodule\n");

    fclose(tbsrc);
    fclose(tbsv);
}
