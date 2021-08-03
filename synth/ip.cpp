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
 * Reads and writes from/to the IP registers and memories accesible by
 * AXI addresses are performed in this SC_MODULE.
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
 
#include "../synth/ip.h"

#undef ADEBUG // I do not know what it is. TODO: delete it, if it's an obsolete stuff

// Only uncomment for debugging:
//#include <iostream>
//using namespace std;

/***
 * Write channel clock thread:
 * 
 * It implements a simplified version of the AXI4-LITE write address,
 * data and response channels. When there is a valid address and data,
 * the AXI4-LITE interface register them and asserts the WVALID signal.
 * This module shall write WDATA on the corresponding place depending
 * on AWADDR. If it is not capable of accepting new data, it should
 * drive to LOW the WREADY signal.
 * 
 * It also runs the "computations" when a certain patern is written in
 * a certain register(s), and signals an interrupt request when finish.
 * 
 * It is intended to be use as an example and templete for writing data
 * comming from the AXI bus into the correct memory or register.
 */
void myip::write_cthread()
{
    /* Reset initializations: */
    // IT SHOULD BE ABLE TO BE EXECUTED IN ONE CLOCK CYCLE !!

    // Initialization of output ports:
    s_ip_wready.write(false);
    interrupt_request.write(false);

    // Internal variables:
    //int int_data = 0; // For an unimplemented example
    char char_data = 0;
    unsigned axi_waddr = 0;
    unsigned waddr = 0;
    sc_uint<AXI_DATA_WIDTH> axi_data = 0;

    // WARNING: variable registers is not initialized. If it is intended to be synthesized as registers, it is better to initialize it. Arrays intended to be synthesized as memory blocks (i.e. BRAM or LutRAM on Xilinx FPGAs) is better to do NOT initialize them.

    wait(); // END OF RESET INITIALIZATIONS

    /* Infinite loop */
    // It assures the hardware perdure on the time (because hardware does not volatilize)
    // THERE SHOULD BE AT LEAST ONE wait(); ON EVERY EXECUTION PATH !!
    // (To avoid the process to take infinite control over the simulation)
    while (true)
    {
        // Default value of WREADY can be HIGH only if it is always capable of write new data:
        //s_ip_wready.write(true);
        
        // It is a slow IP, so the default state should be LOW. Another option is to use the pragma protocol to force a fast reply.
        s_ip_wready.write(false);

        // Wait for WVALID:
        while ( !s_ip_wvalid.read() ) wait();

        // While dealing with the data this IP is not able to receive new data, so WREADY is deasserted (for fast IPs):
        //s_ip_wready.write(false); // already default value

        // Save AWADDRE in a local variable:
        axi_waddr = s_ip_waddr.read();

        // Save in a local variable the correct data, enabled by WSTRB:
#if AXI_DATA_WIDTH == 64
        if ( s_ip_wstrb.read()[0] )  axi_data.range(7,0) = s_ip_wdata.read().range(7,0);
        if ( s_ip_wstrb.read()[1] )  axi_data.range(15,8) = s_ip_wdata.read().range(15,8);
        if ( s_ip_wstrb.read()[2] )  axi_data.range(23,16) = s_ip_wdata.read().range(23,16);
        if ( s_ip_wstrb.read()[3] )  axi_data.range(31,24) = s_ip_wdata.read().range(31,24);
        if ( s_ip_wstrb.read()[4] )  axi_data.range(39,32) = s_ip_wdata.read().range(39,32);
        if ( s_ip_wstrb.read()[5] )  axi_data.range(47,40) = s_ip_wdata.read().range(47,40);
        if ( s_ip_wstrb.read()[6] )  axi_data.range(55,48) = s_ip_wdata.read().range(55,48);
        if ( s_ip_wstrb.read()[7] )  axi_data.range(63,56) = s_ip_wdata.read().range(63,55);
#else // AXI_DATA_WIDTH must be 32
        if ( s_ip_wstrb.read()[0] )  axi_data.range(7,0) = s_ip_wdata.read().range(7,0);
        if ( s_ip_wstrb.read()[1] )  axi_data.range(15,8) = s_ip_wdata.read().range(15,8);
        if ( s_ip_wstrb.read()[2] )  axi_data.range(23,16) = s_ip_wdata.read().range(23,16);
        if ( s_ip_wstrb.read()[3] )  axi_data.range(31,24) = s_ip_wdata.read().range(31,24);
#endif

        //cout << "IP: Received WADDR: " << axi_waddr << ", WDATA: " << axi_data << endl; // Only for debug

        // Depending on the address do:
        switch (axi_waddr)
        {
            // Registers:
            case 0 ... 16*4:
            {
                waddr = axi_waddr >> 2; // Addapt the 8 bit word address to 32 bit word address
                registers[waddr] = axi_data; // + 21; // +21 is a debbuging/checking trick, you can safely remove it

                // Writing on the register 0 triggers the "computations":
                if ( (axi_data & 0x80)  == 0x80 )
                {
                    // Run "computations":
                    // You can call your computation functions here:

                    // Dummy example that simply copy inputs into outputs:
                    for(int i=0; i<IPIN_SIZE; i++)
                    {
                        outbuff[i] = inbuff[i];
                    }
                    wait(); // Update computed values and forces interrupt_request assertion to become after

                    // Generate an interrupt request:
                    interrupt_request.write(true);
                }
                else
                {
                    // Disable the interrupt request when whatever other value is written in register 0
                    interrupt_request.write(false);
                }

                break;
            }
            // Input port:
            case  IPIN_OFFSET ... IPOUT_OFFSET-1 :
            {
                // Addapt the address:
#if IPIN_OFFSET >= IPOUT_SIZE*4
                // Bitwise operation to avoid a subtraction. However it will not work if the SYS_AXI_SIZE > SYS_AXI_BASE
                waddr = ( axi_waddr ^ IPIN_OFFSET ) >> 2;
                //waddr = ( axi_waddr & 0x00FFFFFF ) >> 2;
#else
                waddr = ( axi_waddr - IPIN_OFFSET ) >> 2;
#endif
                //waddr = (axi_waddr - IPIN_OFFSET) >> 2;
                char_data = axi_data.range(7,0); // Give proper format to the data
                //wait(); // It may be necessary to break a critical path, but it will also complicates the FSM which may cause side problems
                inbuff[waddr] = char_data; // Store read data on the input buffer
                break;
            }
            // Another example for reading data. Here integer data are read with a mechanism to prevent read the same data twice:
            //case DIGIT_OFFSET ... (DIGIT_OFFSET+DIGIT_SIZE*4)-1 :
            //{
                //waddr = (axi_waddr - DIGIT_OFFSET) >> 2;
                //if (waddr != prev_waddr) {
                    //int_data = axi_data.to_int(); // Give proper format to the data
                    //input_digit[waddr] = int_data;
                //}
                //prev_waddr = waddr;
                //break;
            //}
            default:
                break;
        } // End switch (axi_waddr)

        // After dealing with the data assert WREADY
        s_ip_wready.write(true);

        // Update written values:
        wait();

        // Wait until WVALID is LOW to ensure a correct handshake protocol
        while ( s_ip_wvalid.read() ) wait();

    } // End infinite loop
}

/***
 * Read channel clock thread:
 * 
 * It implements a simplified version of the AXI4-LITE read address and
 * data channels. When there is a valid address, the AXI4-LITE interface
 * register it and asserts the RREADY signal to request data.
 * This module shall generate RDATA on depending on AWADDR, and validate
 * it asserting RVALID.
 * 
 * It is intended to be use as an example and templete for sending data
 * to the AXI bus from the correct memory or register.
 */
void myip::read_cthread()
{
    /* Reset initializations: */
    // IT SHOULD BE ABLE TO BE EXECUTED IN ONE CLOCK CYCLE !!

    // Initialization of output ports:
    s_ip_rvalid.write(false);
    s_ip_rdata.write(0);

    // Internal variables:
    unsigned axi_raddr = 0;
    unsigned raddr = 0;
    sc_uint<AXI_DATA_WIDTH> axi_dataout = 0;

    wait(); // END OF RESET INITIALIZATIONS

    /* Infinit loop */
    // It assures the hardware perdure on the time (because hardware does not volatilize)
    // THERE SHOULD BE AT LEAST ONE wait(); ON EVERY EXECUTION PATH !!
    // (To avoid the process to take infinite control over the simulation)
    while (true)
    {
        // Default RVALID value should be LOW:
        s_ip_rvalid.write(false);

        // Wait for a data request:
        while ( !s_ip_rready.read() ) wait();

        // Save ARADDR in a local variable:
        axi_raddr = s_ip_raddr.read();

        // Depending on the address do:
        switch (axi_raddr)
        {
            // Registers:
            case 0 ... 16*4:
            {
                raddr =  axi_raddr >> 2; // Addapt the 8 bit word address to 32 bit word address
                axi_dataout = registers[raddr]; // Put data of the register in a local variable. You may need to format properly the data.
                break;
            }
            // Output port:
            case  IPOUT_OFFSET ... (IPOUT_OFFSET+IPOUT_SIZE*4)-1 :
            {
                // Addapt the address:
#if IPIN_OFFSET >= IPOUT_SIZE*4
                // Bitwise operation to avoid a subtraction. However it will not work if the SYS_AXI_SIZE > SYS_AXI_BASE
                raddr = ( axi_raddr ^ IPOUT_OFFSET ) >> 2;
                //raddr = ( axi_raddr & 0x00FFFFFF ) >> 2;
#else
                raddr = ( axi_raddr - IPOUT_OFFSET ) >> 2;
#endif
                raddr = (axi_raddr - IPOUT_OFFSET) >> 2; // Addapt the address
                //wait(); // It may be necessary to break a critical path, but it will also complicates the FSM which may cause side problems
                axi_dataout = outbuff[raddr]; // Put data of the outbuff in a local variable. You may need to format properly the data.
                break;
            }
            default:
                break;
        }
        // Send to the AXI4-LITE slave interface the data on the local variable:
        s_ip_rdata.write(axi_dataout);

        // Validate the data:
        s_ip_rvalid.write(true);

        // Update written values:
        wait();

        // Note: It may be necessary to wait until RREADY is LOW to ensure a correct handshake protocol
        while ( s_ip_rready.read() ) wait();

    }
}

