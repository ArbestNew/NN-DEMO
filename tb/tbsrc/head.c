/***********************************************************************
 * head.c
 * 
 * C function to create the VHDL testbench automatically from the
 * SystemC simulation. It opens the testbench file for writing and
 * writes its "head" (i.e. initial lines).
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

#include<stdio.h>
FILE *tbsrc;
FILE *tbsv;

void head()
{
    tbsrc = fopen("tbsrc.vhd", "w");
    tbsv = fopen("tbsrc.sv", "w");

    fprintf(tbsrc, "library ieee;\n");
    fprintf(tbsrc, "use ieee.std_logic_1164.all;\n");
    fprintf(tbsrc, "use ieee.std_logic_unsigned.all;\n");
    fprintf(tbsrc, "use ieee.numeric_std.all;\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "library STD;\n");
    fprintf(tbsrc, "use STD.textio.all;                     -- basic I/O\n");
    fprintf(tbsrc, "use IEEE.std_logic_textio.all;          -- I/O for logic types\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "entity axi_master_testbench is\n");
    fprintf(tbsrc, "end axi_master_testbench;\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "architecture behaviour of axi_master_testbench is\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "	-- simulation constants\n");
    fprintf(tbsrc, "	constant c_s_axi_data_width : integer := 32;\n");
    fprintf(tbsrc, "	constant c_s_axi_addr_width : integer := 32;\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "	constant c_s_axi_aclk_freq_hz : integer := 100000000;\n");
    fprintf(tbsrc, "	constant axi_aclk_period      : time    := 10 ns; -- 100mhz\n");
    fprintf(tbsrc, "	constant axi_addr_width       : integer := 32;\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "	constant simulation_interval : time := 80 ns;\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "	-- component declaration for the unit under test (uut)\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "	component axislave\n");
    fprintf(tbsrc, "		port(\n");
    fprintf(tbsrc, "			axi_aclk          : in  std_logic;\n");
    fprintf(tbsrc, "			axi_aresetn       : in  std_logic;\n");
    fprintf(tbsrc, "			s_axi_awaddr      : in  std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "			s_axi_awprot      : in  std_logic_vector(2 downto 0);\n");
    fprintf(tbsrc, "			s_axi_awvalid     : in  std_logic;\n");
    fprintf(tbsrc, "			s_axi_awready     : out std_logic;\n");
    fprintf(tbsrc, "			s_axi_wdata       : in  std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "			s_axi_wstrb       : in  std_logic_vector(3 downto 0);\n");
    fprintf(tbsrc, "			s_axi_wvalid      : in  std_logic;\n");
    fprintf(tbsrc, "			s_axi_wready      : out std_logic;\n");
    fprintf(tbsrc, "			s_axi_bresp       : out std_logic_vector(1 downto 0);\n");
    fprintf(tbsrc, "			s_axi_bvalid      : out std_logic;\n");
    fprintf(tbsrc, "			s_axi_bready      : in  std_logic;\n");
    fprintf(tbsrc, "			s_axi_araddr      : in  std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "			s_axi_arprot      : in  std_logic_vector(2 downto 0);\n");
    fprintf(tbsrc, "			s_axi_arvalid     : in  std_logic;\n");
    fprintf(tbsrc, "			s_axi_arready     : out std_logic;\n");
    fprintf(tbsrc, "			s_axi_rdata       : out std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "			s_axi_rresp       : out std_logic_vector(1 downto 0);\n");
    fprintf(tbsrc, "			s_axi_rvalid      : out std_logic;\n");
    fprintf(tbsrc, "			s_axi_rready      : in  std_logic;\n");
    fprintf(tbsrc, "			interrupt_request : out std_logic\n");
    fprintf(tbsrc, "		);\n");
    fprintf(tbsrc, "	end component;\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "	signal interrupt_request : std_logic;\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "	component axi_lite_master_transaction_model is\n");
    fprintf(tbsrc, "		port(\n");
    fprintf(tbsrc, "			-- user signals\n");
    fprintf(tbsrc, "			go                 : in  std_logic;\n");
    fprintf(tbsrc, "			busy               : out std_logic;\n");
    fprintf(tbsrc, "			done               : out std_logic;\n");
    fprintf(tbsrc, "			rnw                : in  std_logic;\n");
    fprintf(tbsrc, "			address            : in  std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "			write_data         : in  std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "			read_data          : out std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "			--  axi4 signals\n");
    fprintf(tbsrc, "			--  axi4 clock / reset\n");
    fprintf(tbsrc, "			m_axi_lite_aclk    : in  std_logic;\n");
    fprintf(tbsrc, "			m_axi_lite_aresetn : in  std_logic;\n");
    fprintf(tbsrc, "			--  axi4 read address channel\n");
    fprintf(tbsrc, "			m_axi_lite_arready : in  std_logic;\n");
    fprintf(tbsrc, "			m_axi_lite_arvalid : out std_logic;\n");
    fprintf(tbsrc, "			m_axi_lite_araddr  : out std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "			--  axi4 read data channel\n");
    fprintf(tbsrc, "			m_axi_lite_rready  : out std_logic;\n");
    fprintf(tbsrc, "			m_axi_lite_rvalid  : in  std_logic;\n");
    fprintf(tbsrc, "			m_axi_lite_rdata   : in  std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "			m_axi_lite_rresp   : in  std_logic_vector(1 downto 0);\n");
    fprintf(tbsrc, "			-- axi4 write address channel\n");
    fprintf(tbsrc, "			m_axi_lite_awready : in  std_logic;\n");
    fprintf(tbsrc, "			m_axi_lite_awvalid : out std_logic;\n");
    fprintf(tbsrc, "			m_axi_lite_awaddr  : out std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "			-- axi4 write data channel\n");
    fprintf(tbsrc, "			m_axi_lite_wready  : in  std_logic;\n");
    fprintf(tbsrc, "			m_axi_lite_wvalid  : out std_logic;\n");
    fprintf(tbsrc, "			m_axi_lite_wdata   : out std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "			m_axi_lite_wstrb   : out std_logic_vector(3 downto 0);\n");
    fprintf(tbsrc, "			-- axi4 write response channel\n");
    fprintf(tbsrc, "			m_axi_lite_bready  : out std_logic;\n");
    fprintf(tbsrc, "			m_axi_lite_bvalid  : in  std_logic;\n");
    fprintf(tbsrc, "			m_axi_lite_bresp   : in  std_logic_vector(1 downto 0)\n");
    fprintf(tbsrc, "		);\n");
    fprintf(tbsrc, "	end component;\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "	signal axi_aclk    : std_logic;\n");
    fprintf(tbsrc, "	signal axi_aresetn : std_logic;\n");
    fprintf(tbsrc, "	signal axi_awaddr  : std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "	signal axi_awvalid : std_logic;\n");
    fprintf(tbsrc, "	signal axi_wdata   : std_logic_vector(c_s_axi_data_width - 1 downto 0);\n");
    fprintf(tbsrc, "	signal axi_wstrb   : std_logic_vector((c_s_axi_data_width / 8) - 1 downto 0);\n");
    fprintf(tbsrc, "	signal axi_wvalid  : std_logic;\n");
    fprintf(tbsrc, "	signal axi_bready  : std_logic;\n");
    fprintf(tbsrc, "	signal axi_araddr  : std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "	signal axi_arvalid : std_logic;\n");
    fprintf(tbsrc, "	signal axi_rready  : std_logic;\n");
    fprintf(tbsrc, "	signal axi_arready : std_logic;\n");
    fprintf(tbsrc, "	signal axi_rdata   : std_logic_vector(c_s_axi_data_width - 1 downto 0);\n");
    fprintf(tbsrc, "	signal axi_rresp   : std_logic_vector(1 downto 0);\n");
    fprintf(tbsrc, "	signal axi_rvalid  : std_logic;\n");
    fprintf(tbsrc, "	signal axi_wready  : std_logic;\n");
    fprintf(tbsrc, "	signal axi_bresp   : std_logic_vector(1 downto 0);\n");
    fprintf(tbsrc, "	signal axi_bvalid  : std_logic;\n");
    fprintf(tbsrc, "	signal axi_awready : std_logic;\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "	signal axi_awprot : std_logic_vector(2 downto 0);\n");
    fprintf(tbsrc, "	signal axi_arprot : std_logic_vector(2 downto 0);\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "	signal go         : std_logic;\n");
    fprintf(tbsrc, "	signal interrupt  : std_logic;\n");
    fprintf(tbsrc, "	signal busy       : std_logic;\n");
    fprintf(tbsrc, "	signal done       : std_logic;\n");
    fprintf(tbsrc, "	signal rnw        : std_logic;\n");
    fprintf(tbsrc, "	signal address    : std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "	signal write_data : std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "	signal read_data  : std_logic_vector(31 downto 0);\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "	-- testbench control signals\n");
    fprintf(tbsrc, "	signal sim_end     : boolean := false;\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "begin\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "	axi_clk_gen : process\n");
    fprintf(tbsrc, "	begin\n");
    fprintf(tbsrc, "		while (not sim_end) loop\n");
    fprintf(tbsrc, "			axi_aclk <= '0';\n");
    fprintf(tbsrc, "			wait for axi_aclk_period / 2;\n");
    fprintf(tbsrc, "			axi_aclk <= '1';\n");
    fprintf(tbsrc, "			wait for axi_aclk_period / 2;\n");
    fprintf(tbsrc, "		end loop;\n");
    fprintf(tbsrc, "		wait;\n");
    fprintf(tbsrc, "	end process axi_clk_gen;\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "	axi_rst_gen : process\n");
    fprintf(tbsrc, "	begin\n");
    fprintf(tbsrc, "		axi_aresetn <= '0';\n");
    fprintf(tbsrc, "		wait for axi_aclk_period * 20;\n");
    fprintf(tbsrc, "		axi_aresetn <= '0';\n");
    fprintf(tbsrc, "		wait for axi_aclk_period * 5;\n");
    fprintf(tbsrc, "		axi_aresetn <= '1';\n");
    fprintf(tbsrc, "		wait;\n");
    fprintf(tbsrc, "	end process axi_rst_gen;\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "	-- -------------------------------------------\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "	stimulus : process\n");
    fprintf(tbsrc, "		variable my_line : line;        -- type 'line' comes from textio\n");
    fprintf(tbsrc, "	begin\n");
    fprintf(tbsrc, "		-- set an idle state\n");
    fprintf(tbsrc, "		address    <= x\"00000000\";\n");
    fprintf(tbsrc, "		write_data <= x\"00000000\";\n");
    fprintf(tbsrc, "		rnw        <= '0';\n");
    fprintf(tbsrc, "		go         <= '0';\n");
    fprintf(tbsrc, "\n");
    fprintf(tbsrc, "		--enable <= '0';\n");
    fprintf(tbsrc, "		wait for simulation_interval;\n");
    fprintf(tbsrc, "		write(my_line, string'(\"Simulation starts\")); -- formatting\n");
    fprintf(tbsrc, "		writeline(output, my_line);     -- write to \"output\"\n");
    fprintf(tbsrc, "\n");

    fprintf(tbsv,"`timescale 1ns / 1ps\n");
    fprintf(tbsv,"\n");
    fprintf(tbsv,"import axi_vip_pkg::*;\n");
    fprintf(tbsv,"import design_1_axi_vip_0_0_pkg::*;\n");
    fprintf(tbsv,"\n");
    fprintf(tbsv,"module tb_AXI_VIP_Master(\n");
    fprintf(tbsv,");\n");
    fprintf(tbsv,"\n");
    fprintf(tbsv,"	bit aclk = 0;\n");
    fprintf(tbsv,"	bit aresetn=0;\n");
    fprintf(tbsv,"	bit interrupt_request;\n");
    fprintf(tbsv,"	xil_axi_prot_t  prot = 0;\n");
    fprintf(tbsv,"	bit [31:0] 	data_rd;\n");
    fprintf(tbsv,"	xil_axi_resp_t 	resp;\n");
    fprintf(tbsv,"	always #5ns aclk = ~aclk;\n");
    fprintf(tbsv,"\n");
    fprintf(tbsv,"	design_1_wrapper DUT\n");
    fprintf(tbsv,"	(\n");
    fprintf(tbsv,"		.aclk (aclk),\n");
    fprintf(tbsv,"		.aresetn(aresetn),\n");
    fprintf(tbsv,"		.interrupt_request (interrupt_request)\n");
    fprintf(tbsv,"	);\n");
    fprintf(tbsv,"\n");
    fprintf(tbsv,"	// Declare agent\n");
    fprintf(tbsv,"	design_1_axi_vip_0_0_mst_t      master_agent;\n");
    fprintf(tbsv,"\n");
    fprintf(tbsv,"	initial begin\n");
    fprintf(tbsv,"		//Create an agent\n");
    fprintf(tbsv,"		master_agent = new(\"master vip agent\",DUT.design_1_i.axi_vip_0.inst.IF);\n");
    fprintf(tbsv,"\n");
    fprintf(tbsv,"		// set tag for agents for easy debug\n");
    fprintf(tbsv,"		master_agent.set_agent_tag(\"Master VIP\");\n");
    fprintf(tbsv,"\n");
    fprintf(tbsv,"		// set print out verbosity level.\n");
    fprintf(tbsv,"		// master_agent.set_verbosity(400);\n");
    fprintf(tbsv,"		master_agent.set_verbosity(0);\n");
    fprintf(tbsv,"\n");
    fprintf(tbsv,"		//Start the agent\n");
    fprintf(tbsv,"		master_agent.start_master();\n");
    fprintf(tbsv,"\n");
    fprintf(tbsv,"		#80ns\n");
    fprintf(tbsv,"		aresetn = 1;\n");
    fprintf(tbsv,"\n");
    fprintf(tbsv,"		#20ns\n");
    fprintf(tbsv,"\n");
}
