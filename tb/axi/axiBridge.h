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

#ifndef AXIBRIDGE_H_
#define AXIBRIDGE_H_

// NOTE: Are really needed all these libraries???
#include <cmath>
#include <deque>
#include <tlm.h>
#include <tlm_utils/simple_target_socket.h>
#include <libsimsoc/interfaces/tlm.hpp>
#include <libsimsoc/interfaces/tlm_signal.hpp>
#include <libsimsoc/interfaces/RS232.hpp>
#include <libsimsoc/module.hpp>
#include "AXIBridgeStructs.h"
#include "axi_lite_master_transaction_model.h"
#include "../../sw/mmap.h"

using namespace sc_core;
using namespace sc_dt;
using namespace simsoc;


class AXIBridge: public simsoc::Module {
public:
    typedef AXIBridge SC_CURRENT_USER_MODULE;
    AXIBridge(sc_core::sc_module_name name, bool big_endian, sc_time &clock_cycle);

    void b_transport(tlm::tlm_generic_payload &payload,
            sc_core::sc_time &delay_time);

    sc_in<bool> clk;
    sc_in<bool> resetn;
    sc_in<bool> interrupt_request;

    sc_out<bool > go;
    sc_out<bool > rnw;
    sc_in<bool > busy;
    sc_in<bool > done;
    sc_out<sc_uint<32> > address;
    sc_out<sc_uint<32> > write_data;
    sc_in<sc_uint<32> > read_data;

    SignalInitiatorPort<bool> it_signal;

    static const int AXISIZE ; // = 0xFFFFFFFF - 0xc0000000;
    static const int SIZE ; //  = 0x0000FFFF;

    tlm_utils::simple_target_socket<AXIBridge> rw_socket;
protected:
    void axiBusHandling();
    void updateirq();

    sc_event request;
    sc_event request_done;
    bool big_endian;
    sc_time clk_cycle;
    tlm::tlm_generic_payload *pl;

};

#endif
