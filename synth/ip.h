/***********************************************************************
 * IP template/wrapper that connects with the AXI4 lite slave interface
 * 
 * Description: A simplified version of the AXI4 lite protocol is used
 * to connect with the interface. Particularly, the handshake signals
 * for the write address channel and write response channel are reduced
 * to the write data channel ones. So, the IP only receives WVALID for
 * validating both AWADDR and WDATA, and it only needs to deassert
 * WREADY to indicate that it is not able to receive new data. And
 * signals for the read channels are reduced similarly to the read data
 * channel ones (it receives RREADY as a read request and asserts RVALID
 * to validate the data).
 * 
 * It also generates an interrupt request when the "computations" are
 * finished.
 * 
 * Version 1.0:
 *   Author: unknown
 *   Date: unknown
 * 
 * Version 2.0:
 *   Author: David Aledo
 *   Date: 18/01/2019
 * 
 * Version 3.0:
 *   Changed polarity (from input to output and viceversa) of the
 *   s_ip_rvalid and s_ip_rready to match AXI4 meaning/usage.
 *   Author: David Aledo
 *   Date: 18/03/2019
 */

#ifndef IP_H
#define IP_H

// SystemC library:
#include <systemc.h>

// Include your libraries here:
#include "../sw/app.h"


// Parameters:
#define AXI_ADDR_WIDTH 32 // width of the AXI address bus
#define AXI_DATA_WIDTH 32 // width of the AXI data bus
#define AXI_STRB_WIDTH (AXI_DATA_WIDTH/8)

/***
 * SC MODULE myip
 * Description: dummy IP that copy the input to the output.
 * It connects to the AXI little slave interface.
 * It may be used as a template to connect custom IPs to the AXI4-LITE
 * interface.
 */
SC_MODULE (myip)
{
    /* Ports: */

    // Clock and Reset:
    sc_in< bool > axi_aclk;
    sc_in< bool > axi_aresetn;

    // Simplified AXI4-LITE interface:
    sc_in< sc_uint<AXI_ADDR_WIDTH> > s_ip_waddr;
    sc_in< sc_uint<AXI_DATA_WIDTH> > s_ip_wdata;
    sc_in< sc_uint<AXI_STRB_WIDTH> > s_ip_wstrb;
    sc_in< sc_uint<AXI_ADDR_WIDTH> > s_ip_raddr;
    sc_out< sc_uint<AXI_DATA_WIDTH> > s_ip_rdata;
    sc_in< bool > s_ip_wvalid;
    sc_out< bool > s_ip_rvalid; // Note: changed from sc_in to sc_out to match AXI4 meaning/usage
    sc_out< bool > s_ip_wready;
    sc_in< bool > s_ip_rready; // Note: changed from sc_out to sc_in to match AXI4 meaning/usage
    sc_out< bool > interrupt_request;

    // Custom ports:
    // Add here custom ports:


    /* Submodules: */
    // Declare here submodules:


    /* Auxiliar (member) functions: */
    // Declare here auxiliar functions:


    /* Processes: */
    void write_cthread(); // Simplified write channel clock thread
    void read_cthread(); // Simplified read channel clock thread

    // Declare here other processes:


    /* Internal signals: */
    // Add here internal signals:


    /* Internal data (variables): */

    sc_signal<unsigned> register0;
    unsigned registers[16];

    // For large arrays, they need to be described as static arrays for HLS.
    // However, they may lead to stack overflow problems during simulation.
    // Hence, they may be described as dynamic arrays for simulation:
#ifdef __SYNTHESIS__
    char inbuff[RFILE_SIZE]; // input buffer
    char outbuff[WFILE_SIZE]; // output buffer
#endif

#ifndef __SYNTHESIS__
    char *inbuff; // input buffer
    char *outbuff; // output buffer
#endif

    // Add here internal variables:


    /* Constructor: */
    SC_CTOR(myip) //: Initialize/construct your submodules and internal signals and data here
    {
#ifndef __SYNTHESIS__
        inbuff = new char[RFILE_SIZE];
        outbuff = new char[WFILE_SIZE];
#endif

        // Register functions as a clock threads:
        SC_CTHREAD (write_cthread, axi_aclk.pos()); // Note: changed from 'neg' edges to 'pos' edges. It will syncronize it with AXISlave
            reset_signal_is(axi_aresetn, false);

        SC_CTHREAD (read_cthread, axi_aclk.pos()); // Note: changed from 'neg' edges to 'pos' edges. It will syncronize it with AXISlave
            reset_signal_is(axi_aresetn, false);

        // Register here your processes:


        // Connect/bind your signals and submodules here:


    }

    /* Destructor: */
    ~myip ()
    {
#ifndef __SYNTHESIS__
        delete [] inbuff;
        delete [] outbuff;
#endif
    }

};

#endif
