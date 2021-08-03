/***********************************************************************
 * Synthesizable AXI4-LITE slave interface for a slow IP
 * 
 * Description: It uses the PRAGMA PROTOCOL to ensure the timing (clock
 * cycles) of the AXI4-LITE protocol on the Vivado HLS tool. For
 * synthesis with other HLS tools consult their manuals.
 * 
 * It registers the data an address to and from the AXI bus, and
 * generates a simplified interface. Particularly, the handshake signals
 * for the write address channel and write response channel are reduced
 * to the write data channel ones. So, the IP only receives WVALID for
 * validating both AWADDR and WDATA, and it only needs to deassert
 * WREADY to indicate that it is not able to receive new data. And
 * signals for the read channels are reduced similarly to the read data
 * channel ones (it receives RREADY as a read request and asserts RVALID
 * to validate the data).
 * 
 * Slow IP means that it neads more than one clock cycle to process the
 * data (write and/or read it). Default value of READY signals are LOW
 * and a throughput of 1 cannot be achieved.
 * 
 * Version 1.0:
 *   Author: David Aledo
 *   Date: 18/03/2019
 * 
 * Version 2.0: for slow IP and added validation of AWVALID & WVALID (previosly only WVALID)
 *   Author: David Aledo
 *   Date: 05/04/2019
 */

#include "AXISlave.h"

#undef ADEBUG // I do not know what it is. TODO: delete it if it is an obsolete stuff

/* Write address channel clock thread:
 * It drives the slave write address data channel
 * and registers the received address for the IP.
 */
void AXISlave::aw_cthread(void)
{
    // Reset initializations:
	s_ip_awaddr_reg.write(0);
	s_axi_awready.write(false);
	s_axi_awready_s.write(false);
    s_ip_awvalid_aux.write(false);

	wait();

	while(true)
    {
        // The following block shall execute wait() in exacly 1 clock cycle:
        // To ensure that in Vivado HLS the pragma PROTOCOL should be added. For other HLS tools, consult their manuals.
        AXI_PROTOCOL_REG_AWADDR:
        {
#pragma HLS protocol fixed
            // AXI4 standard recommends a default state of HIGH for AWREADY to allow one cycle transfers.
            // If the default state is HIGH, the slave must be able to accept any valid address.
            s_axi_awready.write(true);
            s_axi_awready_s.write(true);

            wait(); // To ensure that AWREADY is asserted at least one clock cycle. After this clock cycle, if AWVALID is HIGH, AWADDR can be registered, but not earlier.
            // Note: on previous clock cycle AWREADY was LOW

            // Wait for a valid address from the master:
            while ( !s_axi_awvalid.read() ) wait();

            // Although it is not required by the AXI4 standard (i.e. a next valid addres may be buffered in a FIFO), for simplicity,
            // lets wait until the transaction is finished before accepting a new valid address:
            s_axi_awready.write(false);
            s_axi_awready_s.write(false);
            // Note: this code leads to a slow slave (it needs at least two clock cycles to accept an address)

            // Register the address:
#if SYS_AXI_BASE >= SYS_AXI_SIZE
            // Bitwise operation to avoid a subtraction. However it will not work if the SYS_AXI_SIZE > SYS_AXI_BASE
            //s_ip_awaddr_reg.write(s_axi_awaddr.read() & ~SYS_AXI_BASE); // Alternative
            //s_ip_awaddr_reg.write(s_axi_awaddr.read() & 0x00FFFFFF); // WARNING: this alternative is a trick to ignore the BASE_ADDRESS
            s_ip_awaddr_reg.write(s_axi_awaddr.read() ^ SYS_AXI_BASE);
#else
            s_ip_awaddr_reg.write(s_axi_awaddr.read() - SYS_AXI_BASE);
#endif

            // Register AWPROT:
            // This optional input will be ignored for most of the IPs.
            // However, if needed shoud be registered here before passing it to the IP.
            /* REGISTER AWPROT HERE IF NEEDED */

            wait();
        }

        // Wait until the IP has deasserted WREADY.
        // NOTE: This is necesary only for HLS IPs without LATENCY or PROTOCOL pragmas, in which cannot be guaranteed that WREADY will be asserted a single clock cycle for a single transaction.
        // It may be removed/commented if it can be guaranteed that if WREADY is asserted no more than 2 clock cycles per transaction.
        while ( s_ip_wready.read() ) wait();

        // Generate AWVALID signal for the IP:
        s_ip_awvalid_aux.write(true);

        // Wait until the IP can accept new data. If the IP can always accept new data, this is skipped:
        while ( !s_ip_wready.read() ) wait();
        s_ip_awvalid_aux.write(false); // Deassert IP's WVALID once it has read the data.

        //// Wait the transaction to be finished:
        //// It can be waited for BVALID or BREADY.
        //while ( !s_axi_bvalid_s.read() ) wait();
	}
}

/* Write data channel clock thread
 * It drives the slave write data channel
 * and registers the received data for the IP.
 */
void AXISlave::w_cthread(void)
{
    // Reset initializations:
	s_axi_wready.write(false);
	s_axi_wready_s.write(false);
    s_ip_wvalid_aux.write(false);
    s_ip_wdata_reg.write(0);
	s_ip_wstrb.write(AXI_STRB_ALL_0);

	wait();

	while(true)
    {
        // The following block shall execute wait() in exacly 1 clock cycle:
        // To ensure that in Vivado HLS the pragma PROTOCOL should be added. For other HLS tools, consult their manuals.
        AXI_PROTOCOL_REG_WDATA:
        {
#pragma HLS protocol fixed
            // The default value of WREADY can be HIGH if the slave can always accept write data in a single cycle.
            s_axi_wready.write(true);
            s_axi_wready_s.write(true);

            wait(); // To ensure that WREADY is asserted at least one clock cycle. After this clock cycle, if WVALID is HIGH, WADDR can be registered, but not earlier.
            // Note: on previous clock cycle WREADY was LOW

            // Wait for valid data from the master (i.e. wait for WVALID). Note: it may also wait for a valid address too. To achieve that, a valid_awaddr flag should be added.
            while ( !s_axi_wvalid.read() ) wait();

            // Although it is not required by the AXI4 standard (i.e. a next valid data may be buffered in a FIFO), for simplicity,
            // lets wait until the transaction is finished before accepting a new valid data:
            s_axi_wready.write(false);
            s_axi_wready_s.write(false);
            // Note: this code leads to a slow slave (it needs at least two clock cycles to accept data)

            // Register the data:
            s_ip_wdata_reg.write( s_axi_wdata.read() );

            // Register WSTRB (if needed):
            s_ip_wstrb.write( s_axi_wstrb.read() );

            wait();
        }

        // Wait until the IP has deasserted WREADY.
        // NOTE: This is necesary only for HLS IPs without LATENCY or PROTOCOL pragmas, in which cannot be guaranteed that WREADY will be asserted a single clock cycle for a single transaction.
        // It may be removed/commented if it can be guaranteed that if WREADY is asserted no more than 2 clock cycles per transaction.
        while ( s_ip_wready.read() ) wait();

        // Assert data validation for the IP:
        s_ip_wvalid_aux.write(true);

        // Wait until the IP can accept new data. IF the IP can always accept new data, this is skipped:
        while ( !s_ip_wready.read() ) wait();
        s_ip_wvalid_aux.write(false); // Deassert IP's WVALID once it has read the data.

        //// Wait the transaction to be finished:
        //// To wait for the ransaction end, it can be waited for BVALID or BREADY.
        //while ( !s_axi_bvalid_s.read() ) wait(); // There will not be a new transaction until WREADY is HIGH again
	}
}

/* Simplified write channel WVALID generation in a combinational method
 * It generates the IP's WVALID only when both AWVALID and WVALID are
 * asserted. This is an AND gate.
 */
void AXISlave::write_method(void)
{
    s_ip_wvalid.write( s_ip_awvalid_aux.read() & s_ip_wvalid_aux.read() );
}

/* Write response channel clock thread
 * It drives the slave write response channel.
 */
void AXISlave::b_cthread(void)
{
    // Reset initializations:
	s_axi_bvalid.write(false);
	//s_axi_bvalid_s.write(false);
	s_axi_bresp.write(AXI_OKAY);
    bool awvalid_flag = false;
    bool awready_flag = false;
    bool wvalid_flag = false;
    bool wready_flag = false;

	wait();

	while(true)
    {
        // The following block shall be executed in exacly 1 clock cycle:
        // To ensure that in Vivado HLS the pragma PROTOCOL should be added. For other HLS tools, consult their manuals.
        AXI_PROTOCOL_GEN_BRESP:
        {
#pragma HLS protocol fixed

            // Wait for the transaction to be finished, i.e. for WVALID, WREADY, AWVALID and AWREADY:
            awvalid_flag = s_axi_awvalid.read();
            awready_flag = s_axi_awready_s.read();
            wvalid_flag = s_axi_wvalid.read();
            wready_flag = s_axi_wready_s.read();
            while ( !(awvalid_flag && awready_flag && wvalid_flag  && wready_flag) )
            {
                wait();
                // Latch the flags until all are asserted:
                if ( s_axi_awvalid.read() )
                    awvalid_flag = true;
                if ( s_axi_awready_s.read() )
                    awready_flag = true;
                if ( s_axi_wvalid.read() )
                    wvalid_flag = true;
                if ( s_axi_wready_s.read() )
                    wready_flag = true;
            }

            // Assert BVALID:
            s_axi_bvalid.write(true);
            //s_axi_bvalid_s.write(true);

            // Write BRESP if needed:
            // This optional output is not used by most of the IPs. It can be replaced by a constant OKAY (0b00).
            // However, if needed it shoud be written here. If it is deshired to be generated by the IP, it should be waited for the IP answer, which may make the slave slower.
            /* WRITE BRESP HERE IF NEEDED */

            wait();

            // BVALID must remain asserted until the rising clock edge after the master asserts BREADY:
            while ( !s_axi_bready.read() ) wait();
            s_axi_bvalid.write(false);
        }
	}
}

/* Read address channel clock thread:
 * It drives the slave read address data channel
 * and registers the received address for the IP.
 * It also sends the IP a read request.
 */
void AXISlave::ar_cthread(void)
{
    // Reset initializations:
	s_ip_araddr_reg.write(0);
    s_ip_rready.write(false); // Read request for the IP
	s_axi_arready.write(false);
	s_axi_arready_s.write(false);

	wait();

	while(true)
    {
        // The following block shall execute wait() in exacly 1 clock cycle:
        // To ensure that in Vivado HLS the pragma PROTOCOL should be added. For other HLS tools, consult their manuals.
        AXI_PROTOCOL_REG_ARADDR:
        {
#pragma HLS protocol fixed
            // AXI4 standard recommends a default state of HIGH for AWREADY to allow one cycle transfers.
            // If the default state is HIGH, the slave must be able to accept any valid address.
            s_axi_arready.write(true);
            s_axi_arready_s.write(true);

            wait(); // To ensure that ARREADY is asserted at least one clock cycle. After this clock cycle, if ARVALID is HIGH, ARADDR can be registered, but not earlier.
            // Note: on previous clock cycle ARREADY was LOW

            // Wait for a valid address from the master:
            while ( !s_axi_arvalid.read() ) wait();

            // Although it is not required by the AXI4 standard (i.e. a next valid addres may be buffered in a FIFO), for simplicity,
            // lets wait until the transaction is finished before accepting a new valid address:
            s_axi_arready.write(false);
            s_axi_arready_s.write(false);
            // Note: this code leads to a slow slave (it needs at least two clock cycles to accept an address)

            // Register the address:
#if SYS_AXI_BASE >= SYS_AXI_SIZE
            // Bitwise operation to avoid a subtraction. However it will not work if the SYS_AXI_SIZE > SYS_AXI_BASE
            s_ip_araddr_reg.write(s_axi_araddr.read() ^ SYS_AXI_BASE);
#else
            s_ip_araddr_reg.write(s_axi_araddr.read() - SYS_AXI_BASE);
#endif

            // Register ARPROT:
            // This optional input will be ignored for most of the IPs.
            // However, if needed shoud be registered here before passing it to the IP.
            /* REGISTER ARPROT HERE IF NEEDED */

            wait();
        }

        // Wait until the IP has deasserted RVALID.
        // NOTE: This is necesary only for HLS IPs without LATENCY or PROTOCOL pragmas, in which cannot be guaranteed that RVALID will be asserted a single clock cycle for a single transaction.
        // It may be removed/commented if it can be guaranteed that if RVALID is asserted no more than 2 clock cycles per transaction.
        while ( s_ip_rvalid.read() ) wait();

        // Send the IP a read request:
        s_ip_rready.write(true);

        // Wait until the IP can produce new data. IF the IP can always produce new data, this is skipped:
        while ( !s_ip_rvalid.read() ) wait();
        s_ip_rready.write(false); // Deassert IP's read request once it has write the data.

        //// Wait the transaction to be finished:
        //// It can be waited for RVALID or RREADY.
        //while ( !s_axi_rvalid_s.read() ) wait();
	}
}

/* Read data channel clock thread
 * It drives the slave read data channel
 * and registers the received data for the IP.
 */
void AXISlave::r_cthread(void)
{
    // Reset initializations
    s_axi_rresp.write(AXI_OKAY);
	s_axi_rvalid.write(false);
	s_axi_rdata.write(0);

	wait();

	while(true)
    {
        // Wait for the IP generate valid data:
        while ( !s_ip_rvalid.read() ) wait();

        // Register IP's data:
        // This is done outside the PROTOCOL block because it may need more than one clock cycle. It needs the fixed option of the PROTOCOL pragma
        s_axi_rdata.write( s_ip_rdata_reg.read() );

        // The following block shall execute wait() in exacly 1 clock cycle:
        // To ensure that in Vivado HLS the pragma PROTOCOL should be added. For other HLS tools, consult their manuals.
        AXI_PROTOCOL_GEN_RDATA:
        {
#pragma HLS protocol fixed

            // Assert RVALID:
            s_axi_rvalid.write(true);

            // Write RRESP if needed:
            // This optional output is not used by most of the IPs. It can be replaced by a constant OKAY (0b00).
            // However, if needed it shoud be written here. If it is deshired to be generated by the IP, it should be waited for the IP answer.
            /* WRITE RRESP HERE IF NEEDED */

            wait();

            // RVALID must remain asserted until the rising clock edge after the master asserts RREADY:
            while ( !s_axi_rready.read() ) wait();
            s_axi_rvalid.write(false);
        }

        // Wait until the IP has deasserted RVALID.
        // NOTE: This is necesary only for HLS IPs without LATENCY or PROTOCOL pragmas, in which cannot be guaranteed that RVALID will be asserted a single clock cycle for a single transaction.
        // It may be removed/commented if it can be guaranteed that if RVALID is asserted no more than 1 clock cycles per transaction.
        while ( s_ip_rvalid.read() ) wait();
	}
}

