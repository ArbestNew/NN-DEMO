/***********************************************************************
 * Synthesizable AXI4-LITE slave interface
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
 * It also includes an interrupt request from (generated on) the IP.
 * 
 * Version 1.0:
 *   Author: David Aledo
 *   Date: 18/03/2019
 */

#ifndef AXI_SLAVE_H
#define AXI_SLAVE_H

// SystemC library
#include <systemc.h>

// Include your IP header here:
#include "ip.h"

// AXI slave parameters:
#include "../sw/mmap.h"

// Already defined on ip.h:
//#define AXI_ADDR_WIDTH 32 // width of the AXI address bus
//#define AXI_DATA_WIDTH 32
//#define AXI_STRB_WIDTH 4

#define AXI_STRB_ALL_1 0xF
#define AXI_STRB_ALL_0 0x0

#define  AXI_OKAY 0 // 0b00
#define  AXI_DECERR 3 // 0b11

/***
 * SC MODULE AXISlave
 * Description: AXI lite slave interface
 */
SC_MODULE (AXISlave)
{
    /* Ports: */

    // Clock and Reset
    sc_in< bool > axi_aclk;
    sc_in< bool > axi_aresetn;

    // AXI Write Address Channel
    sc_in< sc_uint<AXI_ADDR_WIDTH > > s_axi_awaddr;
    sc_in< sc_uint<3> > s_axi_awprot; // Optional input, may be ignored or passed to the IP
    sc_in< bool > s_axi_awvalid;
    sc_out< bool > s_axi_awready;

    // AXI Write Data Channel
    sc_in< sc_uint<AXI_DATA_WIDTH> > s_axi_wdata;
    sc_in< sc_uint<AXI_STRB_WIDTH> > s_axi_wstrb;
    sc_in< bool > s_axi_wvalid;
    sc_out< bool > s_axi_wready;

    // AXI Read Address Channel
    sc_in< sc_uint<AXI_ADDR_WIDTH> > s_axi_araddr;
    sc_in< sc_uint<3> > s_axi_arprot; // Optional input, may be ignored or passed to the IP
    sc_in< bool > s_axi_arvalid;
    sc_out< bool > s_axi_arready;

    // AXI Read Data Channel
    sc_out< sc_uint<AXI_DATA_WIDTH> > s_axi_rdata;
    sc_out< sc_uint<2> > s_axi_rresp; // Optional output, it may be driven by 0b00 (OKAY) or by the IP
    sc_out< bool > s_axi_rvalid;
    sc_in< bool > s_axi_rready;

    // AXI Write Response Channel
    sc_out< sc_uint<2> > s_axi_bresp; // Optional output, it may be driven by 0b00 (OKAY) or by the IP
    sc_out< bool > s_axi_bvalid;
    sc_in< bool > s_axi_bready;

	sc_out< bool > interrupt_request;

    /* Submodules: */

    // IP interfaced:
    myip myip_1;

    /* Internal signals: */

    // Signals to and from MYIP:
    // They implement a simplified version of the AXI4-LITE interface.
    sc_signal< bool > s_ip_rvalid; // out from MYIP
    sc_signal< bool > s_ip_wvalid; // in to MYIP
    sc_signal< bool > s_ip_wready; // out from MYIP
    sc_signal< bool > s_ip_rready; // in to MYIP
    sc_signal< sc_uint<AXI_ADDR_WIDTH> > s_ip_araddr_reg; // in to MYIP
    sc_signal< sc_uint<AXI_ADDR_WIDTH> > s_ip_awaddr_reg; // in to MYIP
    sc_signal< sc_uint<AXI_DATA_WIDTH> > s_ip_wdata_reg; // in to MYIP
    sc_signal< sc_uint<AXI_STRB_WIDTH> > s_ip_wstrb; // in to MYIP
    sc_signal< sc_uint<AXI_DATA_WIDTH> > s_ip_rdata_reg; // out from MYIP

    // Internal signals that mirror the output ports (bacause output ports connot be read, but they are needed in other cthreads)
	sc_signal< bool > s_axi_awready_s;
	sc_signal< bool > s_axi_wready_s;
	//sc_signal< bool > s_axi_bvalid_s; // Not needed in this version
	sc_signal< bool > s_axi_arready_s;
	sc_signal< bool > s_axi_rvalid_s;

    // Auxiliar internal signals:
    sc_signal< bool > s_ip_awvalid_aux;
    sc_signal< bool > s_ip_wvalid_aux;

    /* Auxiliar (member) functions: */


	/* Processes: */
    void aw_cthread(void); // Write address channel clock thread
    void w_cthread(void);  // Write data channel clock thread
    void b_cthread(void);  // Write response channel clock thread
    void ar_cthread(void); // Read address channel clock thread
    void r_cthread(void);  // Read data channel clock thread
    void write_method(void); // Simplified write channel WVALID generation in a combinational method


    /* Constructor: */
    SC_CTOR(AXISlave) : myip_1("myip_1")
    {
        SC_CTHREAD(aw_cthread, axi_aclk.pos());
            reset_signal_is(axi_aresetn, false);

        SC_CTHREAD(w_cthread, axi_aclk.pos());
            reset_signal_is(axi_aresetn, false);

        SC_CTHREAD(b_cthread, axi_aclk.pos());
            reset_signal_is(axi_aresetn, false);

        SC_CTHREAD(ar_cthread, axi_aclk.pos());
            reset_signal_is(axi_aresetn, false);

        SC_CTHREAD(r_cthread, axi_aclk.pos());
            reset_signal_is(axi_aresetn, false);

        SC_METHOD(write_method);
            sensitive << s_ip_awvalid_aux << s_ip_wvalid_aux;

        // Connect te IP:
        myip_1.axi_aclk(axi_aclk);
        myip_1.axi_aresetn(axi_aresetn);

        myip_1.s_ip_wvalid(s_ip_wvalid);
        myip_1.s_ip_rvalid(s_ip_rvalid);
        myip_1.s_ip_wready(s_ip_wready);
        myip_1.s_ip_rready(s_ip_rready);

        myip_1.s_ip_waddr(s_ip_awaddr_reg);
        myip_1.s_ip_wdata(s_ip_wdata_reg);
        myip_1.s_ip_wstrb(s_ip_wstrb);
        myip_1.s_ip_raddr(s_ip_araddr_reg);
        myip_1.s_ip_rdata(s_ip_rdata_reg);
        myip_1.interrupt_request(interrupt_request);
    }

    ~AXISlave ()
    {
    }
};

#endif
