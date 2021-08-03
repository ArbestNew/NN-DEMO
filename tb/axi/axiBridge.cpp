/***********************************************************************
 * axiBridge.cpp
 * SystemC module for a bridge between the TLM bus and the AXI bus on
 * the SystemC simulation. It receives and generates the TLM
 * transactions, and interfaces with the
 * 'axi_lite_master_transaction_model' module (which is the master of
 * the AXI bus).
 * It also writes every transaction received from or to the AXI bus on
 * the VHDL testbench.
 * 
 * Version 1.0:
 *   Author: unknown
 *   Date: unknown
 * 
 * Version 2.0: added SYS_AXI_BASE to the addresses written on the VHDL
 * testbench.
 *   Author: David Aledo
 *   Date: 27/03/2019
 */

#include "axiBridge.h"

extern "C" {
void write_transaction(int a, int d);
void read_transaction(int a);
}


using namespace std;

AXIBridge::AXIBridge(sc_core::sc_module_name name, bool big_endian, sc_time &clock_cycle) :
        Module(name), big_endian(big_endian), pl(0) {
    rw_socket.register_b_transport(this, &AXIBridge::b_transport);

    SC_THREAD(axiBusHandling);
    reset_signal_is(resetn, false);

    SC_THREAD(updateirq);
    reset_signal_is(resetn, false);

    clk_cycle = clock_cycle;

}

void AXIBridge::axiBusHandling() {
    while (true) {
        wait(request);
        //std::cout << "\tbusHandling at " << sc_time_stamp() << " got request " << std::endl;
        if (pl->get_command() == tlm::TLM_WRITE_COMMAND) {
            // write data
            if (pl->get_data_length() == 1) {
                write_data.write((uint32_t) *(uint8_t*) pl->get_data_ptr());
#ifndef COSIM_SYSTEMC
write_transaction(pl->get_address()+SYS_AXI_BASE, (uint32_t) *(uint8_t*) pl->get_data_ptr());
#endif
            } else {
                write_data.write(*(uint32_t*) pl->get_data_ptr());
#ifndef COSIM_SYSTEMC
write_transaction(pl->get_address()+SYS_AXI_BASE, *(uint32_t*) pl->get_data_ptr());
#endif
            }
            address.write(pl->get_address()+SYS_AXI_BASE);
            rnw.write(false);
            go.write(true);
            wait(clk_cycle);
            go.write(false);
            while (done.read() == false) {
                wait(clk_cycle);
            }
        } else if (pl->get_command() == tlm::TLM_READ_COMMAND) {
            // read data
#ifndef COSIM_SYSTEMC
	read_transaction(pl->get_address()+SYS_AXI_BASE);
#endif
            address.write(pl->get_address()+SYS_AXI_BASE);
            rnw.write(true);
            go.write(true);
            wait(clk_cycle);
            go.write(false);
            while (done.read() == false) {
                wait(clk_cycle);
            }
            *(int*) pl->get_data_ptr() = read_data.read();
        }
        pl->set_command(tlm::TLM_IGNORE_COMMAND);
        //std::cout << "\tbusHandling at " << sc_time_stamp() << " notify request_done " << std::endl;
        request_done.notify();
    }
}

void AXIBridge::b_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay_time) {
    while (resetn.read() == 0)
        wait(clk_cycle);

    if (payload.get_byte_enable_ptr()) {
        payload.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
    } else {
        pl = &payload;
        request.notify();
        //std::cout << "b_transport at " << sc_time_stamp() << " request send" << std::endl;
        wait(request_done);
        //std::cout << "b_transport at " << sc_time_stamp() << " request done " <<std::endl;
        payload.set_response_status(tlm::TLM_OK_RESPONSE);
    }
}

void AXIBridge::updateirq() {
    while (true) {
        wait(clk_cycle);
        it_signal = interrupt_request.read();
        //if (it_signal != 0)
        //std::cout << "interrupt_request read at " <<  it_signal << std::endl;
    }
}
