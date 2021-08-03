/***********************************************************************
 * top.cpp
 * SystemC simulation main function and top modules for a system with
 * an ISS (SimSoC), a TLM bus with an AXI bus bridge, IRQC and console.
 * Other peripherals can be added to the TLM or AXI buses, and files
 * can be mapped directly on the TLM bus for reading and writing.
 * 
 * Version 1.0:
 *   Author: unknown
 *   Date: unknown
 * 
 * Version 2.0:
 *   Author: David Aledo
 *   Date: 17/01/2019
 */

// Part of this code is originaly from:
//
// SimSoC Initial software, Copyright © INRIA 2007, 2008, 2009, 2010
// LGPL license version 3
//

// SimSoc libraries:
#include <libsimsoc/context.hpp>
#include <libsimsoc/processors/arm/arm_processor.hpp>
#include <libsimsoc/processors/arm_v6/arm_v6_processor.hpp>
#include <libsimsoc/components/memory.hpp>
#include <libsimsoc/components/debug_console.hpp>
#include <libsimsoc/components/irqc.hpp>
#include <libsimsoc/components/bus.hpp>
#include <libsimsoc/components/dce_null_modem.hpp>
#include <libsimsoc/components/stop_box.hpp>
#include "libsimsoc/interfaces/tlm.hpp"
#include "libsimsoc/interfaces/tlm_signal.hpp"

// Libraries of the system to be simulated:
#include "../sw/app.h"
#include "../sw/mmap.h"
#include "tb_init.h"
#include "axi_lite_master_transaction_model.h"
#include "AXIBridgeStructs.h"
#include "axiBridge.h"

#ifdef __RTL_SIMULATION__
#include "AXISlave_rtl_wrapper.h"
#define AXISlave AXISlave_rtl_wrapper
#elif MTI_SYSTEMC
#include "AXISlave_vhdl.h"
#elif COSIM_SYSTEMC
#include "../sw/app.h"
#include "../VLSIProject/solution1/syn/systemc/AXISlave_rtl_wrapper.h"
#define AXISlave AXISlave_rtl_wrapper
#else
#include "../synth/AXISlave.h"
#endif

extern "C" {
    void head(void);
    void tail(void);
}

// Add your libraries here:
/*** START OF EDITABLE SECTION ***/


/*** END OF EDITABLE SECTION ***/

// Namespaces used in the code:
using namespace std;
using namespace sc_core;
using namespace simsoc;

// Choose procesor to be emulated by the ISS:
ParameterBool arm_v5("ARM processor", "-v5", "Simulate ARMv5 (instead of ARMv6)");

/****
 * SC_MODULE SubSystem
 * Description: It instantiates and connects the ISS (SimSoC), the TLM bus,
 * an AXI bus bridge and the peripherals connected to the buses.
 * 
 */
class SubSystem: public Module {
public:

    // Memmory map BASE addresses and SIZES (from mmap.h) stored as static constants:
    // NOTE: I am not sure which is the benefit of it.
    static const uint32_t MEM_BASE = SYS_MEM_BASE;
    static const uint32_t MEM_SIZE = SYS_MEM_SIZE;

    static const uint32_t CONS_BASE = SYS_CONS_BASE;
    static const uint32_t CONS_SIZE = SYS_CONS_SIZE;

    static const uint32_t IRQC_BASE = SYS_IRQC_BASE;
    static const uint32_t IRQC_SIZE = SYS_IRQC_SIZE;

    static const uint32_t AXI_BASE = SYS_AXI_BASE;
    static const uint32_t AXI_SIZE = SYS_AXI_SIZE;

    // Instantiate the memory map BASE addresses and SIZES static constants of your peripherals here:
/*** START OF EDITABLE SECTION ***/

    // Example of file for reading:
    static const uint32_t RFILE_RAM_BASE = SYS_RFILE_RAM_BASE;
    static const uint32_t RFILE_RAM_SIZE = SYS_RFILE_RAM_SIZE*4; // Take care of the *4 

    // Example of a file for writing:
    static const uint32_t WFILE_RAM_BASE = SYS_WFILE_RAM_BASE;
    static const uint32_t WFILE_RAM_SIZE = SYS_WFILE_RAM_SIZE*4; // Take care of the *4

    // Example of a file for r/w:
    static const uint32_t RWNUM_RAM_BASE = SYS_RWNUM_RAM_BASE;
    static const uint32_t RWNUM_RAM_SIZE = SYS_RWNUM_RAM_SIZE*4; // Take care of the *4 


/*** END OF EDITABLE SECTION ***/

    /* Submodules: */
    Bus bus; // TLM bus
    Memory mem; // Processor's memory
    DebugConsole cons; // Console
    Irqc irqc;
    Processor *proc; // ISS (SimSoC)
    AXIBridge axiBridge;
    axi_lite_master_transaction_model axi_master;

    // Declare yor peripheral modules here:
/*** START OF EDITABLE SECTION ***/

    // Examples of files directly mapped on the TLM bus:
    Memory rfile_mem; // Memory to be associate to rfile.txt for reading
    Memory wfile_mem; // Memory to be associate to wfile.out for writing
    Memory rwnum_mem; // Memory to be associate to rwnum.txt and rwnum.out for reading and writing

    // Example of an IP connected on the AXI bus:
    // It is connected through the AXISlave interface.
    AXISlave axi_slave; // AXI slave interface for the example IP


/*** END OF EDITABLE SECTION ***/

    /* Ports: */
    sc_in<bool> clk;
    sc_in<bool> resetn;
    AXI_Signals axiSignals;
    sc_signal<bool> stb; // ???

    /* SC functions: */
    void start_of_simulation () {
        std::cout << " PRESTART CALLED" << endl;

        // Read input txt files and associate them to their Memory modules:
/*** START OF EDITABLE SECTION ***/

        rfile_mem.read_file( "rfile.txt", 0 , CHAR2BYTE); // Example rfile.txt for reading
        // use the appropiate "conv_format" tag for your file kind from: STR2INT, STR2UINT, STR2FLOAT, CHAR2BYTE

        rwnum_mem.read_file( "rwnum.txt", 0 , STR2UINT); // Example rwnum.txt for reading

/*** END OF EDITABLE SECTION ***/

        // Writes the header of the RTL (in VHDL) testbench:
#ifndef COSIM_SYSTEMC
        head();
#endif
    }

    void end_of_simulation () {
        std::cout << " PRESTOP CALLED" << endl;

        // Write output files with the contents stored on their associated Memory modules:
/*** START OF EDITABLE SECTION ***/

        // The function 'write_file' declaration is:
        // void write_file(const char *file_name, uint32_t address, uint32_t elem, uint32_t bits, conv_format type);
        // However, the argument bits is not used, so it may be given whatever integer vale. Be carfeul because in future versions this argument may dissapear or become effective.
        wfile_mem.write_file( "wfile.out", 0, 12, 0, BYTE2CHAR); // Example wfile.out for writing
        // use the appropiate "conv_format" tag for your file kind from: INT2STR, UINT2STR, FLOAT2STR, BYTE2CHAR

        rwnum_mem.write_file( "rwnum.out", 0, SYS_RWNUM_RAM_SIZE, 0, UINT2STR); // Example rwnum.out for writing

/*** END OF EDITABLE SECTION ***/

        // Writes the tail of the RTL (in VHDL) testbench:
#ifndef COSIM_SYSTEMC
        tail();
#endif
    }

    /* Constructor: */
    /* Constructor parameters:
     *   name (type sc_module_name): name of the constructed module
     *   sb (type StopBox*): address of ***NOTE: I don't know what it is***, used to construct/initialize the console
     *   clock_cycle (type sc_time&): reference of ***NOTE: it seems to be the AXI bus clock period***, used to construct/initialize the AXI bridge
     */
    SubSystem(sc_module_name name, StopBox *sb, sc_time &clock_cycle) :
            Module(name), clk("CLK"), resetn("RESETN"), stb("STB"),
            // module instantiation:
            bus("BUS"),
            mem("MEMORY", MEM_SIZE),
            cons("CONSOLE", sb),
            irqc("IRQC"),
            proc(NULL), // Initialization of the pointer to the ISS (SimSoC) module, it is not construced yet
            axiBridge("AXIBridge", main_context().is_big_endian(), clock_cycle),
            axi_master("axi_master"),
#ifdef MTI_SYSTEMC
            axi_slave("axi_slave","work.AXISlave"),
#else
            axi_slave("axi_slave"),
#endif
            // Instantiate your peripherals here:
/*** START OF EDITABLE SECTION ***/

            rwnum_mem("RWNUM_MEM", RWNUM_RAM_SIZE),
            rfile_mem("RFILE_MEM", RFILE_RAM_SIZE), // Example of file for reading
            wfile_mem("WFILE_MEM", WFILE_RAM_SIZE) // Example of file for writing
            // Remember to do not use "," after the last module instantiation


/*** END OF EDITABLE SECTION ***/
    {
        // Construct the ISS (SimSoC) for an ARM v5 or v6:
        if (arm_v5.get())
            proc = new ARM_Processor("ARMv5");
        else
            proc = new ARMv6_Processor("ARMv6");

        // Print the memory map (of the TLM bus):
        bus.print_mmap(std::cout);

        //rfile_mem.dump(0, 16, cout); // Inherited commented code, ignore it
        //wfile_mem.dump(0, 16, cout); // Inherited commented code, ignore it

        /* Initialize ports and buses and bind them: */
        proc->get_rw_port()(bus.target_sockets);

        axiBridge.it_signal(irqc.in_signals[0]); // Interruption from the AXI bus
        irqc.out_signal(proc->get_it_port()); // Connect IRQ controler to the processor (ISS SimSoC)

        bus.bind_target(mem.rw_socket, MEM_BASE, MEM_SIZE); // Connects processor's memory to the TLM bus

        bus.bind_target(cons.target_socket, CONS_BASE, CONS_SIZE); // Connect the console to the TLM bus
        bus.bind_target(irqc.rw_port, IRQC_BASE, IRQC_SIZE); // Connect IRQ controller to the TLM bus
        bus.bind_target(axiBridge.rw_socket, AXI_BASE, AXI_SIZE); // Connect the AXI bridge to the TLM bus

        // Console initializations:
        cons.set_big_endian(main_context().is_big_endian());
        cons.initiator_socket(proc->debug_port);

        //AXI Bridge ports:
        axiBridge.clk(clk);
        axiBridge.resetn(resetn);
        axiBridge.interrupt_request(axiSignals.interrupt_request);
        axiBridge.go(axiSignals.go);
        axiBridge.rnw(axiSignals.rnw);
        axiBridge.busy(axiSignals.busy);
        axiBridge.done(axiSignals.done);
        axiBridge.address(axiSignals.address);
        axiBridge.write_data(axiSignals.write_data);
        axiBridge.read_data(axiSignals.read_data);

        //AXI Master ports:
        axi_master.go(axiSignals.go);
        axi_master.rnw(axiSignals.rnw);
        axi_master.busy(axiSignals.busy);
        axi_master.done(axiSignals.done);
        axi_master.address(axiSignals.address);
        axi_master.write_data(axiSignals.write_data);
        axi_master.read_data(axiSignals.read_data);

        axi_master.m_axi_lite_aclk(clk);
        axi_master.m_axi_lite_aresetn(resetn);
        axi_master.m_axi_lite_arready(axiSignals.axi_lite_arready);
        axi_master.m_axi_lite_arvalid(axiSignals.axi_lite_arvalid);
        axi_master.m_axi_lite_araddr(axiSignals.axi_lite_araddr);
        axi_master.m_axi_lite_rready(axiSignals.axi_lite_rready);
        axi_master.m_axi_lite_rvalid(axiSignals.axi_lite_rvalid);
        axi_master.m_axi_lite_rdata(axiSignals.axi_lite_rdata);
        axi_master.m_axi_lite_rresp(axiSignals.axi_lite_rresp);
        axi_master.m_axi_lite_awready(axiSignals.axi_lite_awready);
        axi_master.m_axi_lite_awvalid(axiSignals.axi_lite_awvalid);
        axi_master.m_axi_lite_awaddr(axiSignals.axi_lite_awaddr);
        axi_master.m_axi_lite_wready(axiSignals.axi_lite_wready);
        axi_master.m_axi_lite_wvalid(axiSignals.axi_lite_wvalid);
        axi_master.m_axi_lite_wdata(axiSignals.axi_lite_wdata);
        axi_master.m_axi_lite_wstrb(axiSignals.axi_lite_wstrb);
        axi_master.m_axi_lite_bready(axiSignals.axi_lite_bready);
        axi_master.m_axi_lite_bvalid(axiSignals.axi_lite_bvalid);
        axi_master.m_axi_lite_bresp(axiSignals.axi_lite_bresp);

        // Initialize and connect/bind your peripherals here:
/*** START OF EDITABLE SECTION ***/

        // You may need to add more AXI slave modules:
        //AXI Slave ports of one AXI slave:
        axi_slave.axi_aclk(clk);
        axi_slave.axi_aresetn(resetn);
        axi_slave.interrupt_request(axiSignals.interrupt_request);
        axi_slave.s_axi_arready(axiSignals.axi_lite_arready);
        axi_slave.s_axi_arvalid(axiSignals.axi_lite_arvalid);
        axi_slave.s_axi_araddr(axiSignals.axi_lite_araddr);
        axi_slave.s_axi_arprot(axiSignals.axi_lite_arprot);
        axi_slave.s_axi_rready(axiSignals.axi_lite_rready);
        axi_slave.s_axi_rvalid(axiSignals.axi_lite_rvalid);
        axi_slave.s_axi_rdata(axiSignals.axi_lite_rdata);
        axi_slave.s_axi_rresp(axiSignals.axi_lite_rresp);
        axi_slave.s_axi_awaddr(axiSignals.axi_lite_awaddr);
        axi_slave.s_axi_awprot(axiSignals.axi_lite_awprot);
        axi_slave.s_axi_awvalid(axiSignals.axi_lite_awvalid);
        axi_slave.s_axi_awready(axiSignals.axi_lite_awready);
        axi_slave.s_axi_wdata(axiSignals.axi_lite_wdata);
        axi_slave.s_axi_wstrb(axiSignals.axi_lite_wstrb);
        axi_slave.s_axi_wvalid(axiSignals.axi_lite_wvalid);
        axi_slave.s_axi_wready(axiSignals.axi_lite_wready);
        axi_slave.s_axi_bresp(axiSignals.axi_lite_bresp);
        axi_slave.s_axi_bvalid(axiSignals.axi_lite_bvalid);
        axi_slave.s_axi_bready(axiSignals.axi_lite_bready);

        // Connect Memory modules associated to files to the TLM bus:
        bus.bind_target(rfile_mem.rw_socket, RFILE_RAM_BASE, RFILE_RAM_SIZE); // Connect the example file to read
        bus.bind_target(wfile_mem.rw_socket, WFILE_RAM_BASE, WFILE_RAM_SIZE); // Connect the example file to write
        bus.bind_target(rwnum_mem.rw_socket, RWNUM_RAM_BASE, RWNUM_RAM_SIZE);


/*** END OF EDITABLE SECTION ***/
    }
};

/****
 * SC_MODULE Top
 * Description: It is the top module of the simulation. It instantiates
 * and connects the SubSystem with clock and reset signal generators.
 * 
 */
class Top: public Module {
public:
    // TODO: Re-order declarations by parameters, ports and submodules and give them a descriptive comment:
    sc_time clock_cycle;
    StopBox sb;
    SubSystem soc0;
    sc_clock clk;
    GenReset resetGenerator;
    sc_signal<bool> resetn;

    // Constructor of the top module. It is structural only (i.e. it does not have processes):
    Top(sc_module_name name, sc_time &clock_cycle) : Module(name), clk("CLK", clock_cycle), sb("STOPBOX", 1), soc0("SOC0", &sb, clock_cycle), resetGenerator( "ResetGenerator") {
        resetGenerator.clk.bind(clk);
        resetGenerator.reset.bind(resetn);
        soc0.clk.bind(clk);
        soc0.resetn.bind(resetn);
    }
};


#ifdef MTI_SYSTEMC
//int sc_argc();
//const char* const* sc_argv();

SC_MODULE(arm_axi_soc)
{
    sc_time clock_cycle;
    int Iargc; //  = sc_argc();
    const char * const * Iargv ; // = sc_argv();

    // vsim -sc_arg "arg1" -sc_arg "arg2"

    SC_CTOR (arm_axi_soc) : clock_cycle(10, SC_NS)
    {
        Top top("TOP", clock_cycle);
        Iargc = sc_argc();
        Iargv = sc_argv();
        simsoc_init(Iargc, (char **)Iargv);
    }
} ;

SC_MODULE_EXPORT(arm_axi_soc);

#else

/***
 * SC main function
 * 
 */
int sc_main(int argc, char *argv[]) {

    /* Elaboration */

    // SystemC simulation configuration:
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
    sc_report_handler::set_actions( SC_ID_LOGIC_X_TO_BOOL_, SC_LOG);
    sc_report_handler::set_actions( SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_, SC_LOG);

    sc_time clock_cycle(10, SC_NS); // Period of the system clock

    // Start ISS (SimSoC):
    simsoc_init(argc, argv);

    // Instantiate the top module:
    Top top("TOP", clock_cycle);

    // Open a trace file to record the AXI bus signals:
    sc_trace_file* fp( sc_create_vcd_trace_file( "tr" ) );
    fp->set_time_unit(1,SC_NS);

    // Selects the signals to be traced:
#ifdef TRACE
    sc_trace(fp, top.clk, "clk" );
    sc_trace(fp, top.resetn, "resetn" );
    sc_trace(fp, top.soc0.axiSignals.interrupt_request, "interrupt_request" );
    sc_trace(fp, top.soc0.axiSignals.go, "go" );
    sc_trace(fp, top.soc0.axiSignals.rnw, "rnw" );
    sc_trace(fp, top.soc0.axiSignals.busy, "busy" );
    sc_trace(fp, top.soc0.axiSignals.done, "done" );
    sc_trace(fp, top.soc0.axiSignals.address, "address" );
    sc_trace(fp, top.soc0.axiSignals.write_data, "write_data" );
    sc_trace(fp, top.soc0.axiSignals.read_data, "read_data" );
    //sc_trace(fp, ((sc_uint<32>)top.soc0.axi_master.current_state.read()), "current_state" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_arready, "axi_lite_arready" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_arvalid, "axi_lite_arvalid" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_araddr, "axi_lite_araddr" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_rready, "axi_lite_rready" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_rvalid, "axi_lite_rvalid" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_rdata, "axi_lite_rdata" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_rresp, "axi_lite_rresp" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_awready, "axi_lite_awready" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_awvalid, "axi_lite_awvalid" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_awaddr, "axi_lite_awaddr" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_wready, "axi_lite_wready" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_wvalid, "axi_lite_wvalid" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_wdata, "axi_lite_wdata" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_wstrb, "axi_lite_wstrb" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_bready, "axi_lite_bready" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_bvalid, "axi_lite_bvalid" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_bresp, "axi_lite_bresp" );

    sc_trace(fp, top.soc0.axi_master.current_state, "current_state");
    sc_trace(fp, top.soc0.axi_master.next_state, "next_state");
    sc_trace(fp, top.soc0.axi_master.read_channel_data, "read_channel_data");
    sc_trace(fp, top.soc0.axi_master.write_channel_data, "write_channel_data");
    sc_trace(fp, top.soc0.axi_master.transaction_address, "transaction_address");
    sc_trace(fp, top.soc0.axi_master.start_read_transaction, "start_read_transaction");
    sc_trace(fp, top.soc0.axi_master.start_write_transaction, "start_write_transaction");
    sc_trace(fp, top.soc0.axi_master.read_address_finished, "read_address_finished");
    sc_trace(fp, top.soc0.axi_master.read_transaction_finished, "read_transaction_finished");
    sc_trace(fp, top.soc0.axi_master.send_write_data, "send_write_data");
    sc_trace(fp, top.soc0.axi_master.write_data_sent, "write_data_sent");


    sc_trace(fp, top.soc0.axiBridge.go, "Bridge_go");
    sc_trace(fp, top.soc0.axiBridge.rnw, "Bridge_rnw");
    sc_trace(fp, top.soc0.axiBridge.busy, "Bridge_busy");
    sc_trace(fp, top.soc0.axiBridge.done, "Bridge_done");
    sc_trace(fp, top.soc0.axiBridge.address, "Bridge_address");
    sc_trace(fp, top.soc0.axiBridge.write_data, "Bridge_write_data");
    sc_trace(fp, top.soc0.axiBridge.read_data, "Bridge_read_data");


    // User Ports:
/*** START OF EDITABLE SECTION ***/
#ifndef __RTL_SIMULATION__
#ifndef COSIM_SYSTEMC
    sc_trace(fp, top.soc0.axi_slave.myip_1.s_ip_waddr, "s_ip_waddr");
    sc_trace(fp, top.soc0.axi_slave.myip_1.s_ip_wdata, "s_ip_wdata");
    sc_trace(fp, top.soc0.axi_slave.myip_1.s_ip_wstrb, "s_ip_wstrb");
    sc_trace(fp, top.soc0.axi_slave.myip_1.s_ip_raddr, "s_ip_raddr");
    sc_trace(fp, top.soc0.axi_slave.myip_1.s_ip_rdata, "s_ip_rdata");
    sc_trace(fp, top.soc0.axi_slave.myip_1.s_ip_wvalid, "s_ip_wvalid");
    sc_trace(fp, top.soc0.axi_slave.myip_1.s_ip_rvalid, "s_ip_rvalid");
    sc_trace(fp, top.soc0.axi_slave.myip_1.s_ip_wready, "s_ip_wready");
    sc_trace(fp, top.soc0.axi_slave.myip_1.s_ip_rready, "s_ip_rready");
    
    sc_trace(fp, top.soc0.axi_slave.s_axi_arready, "s_axi_arready" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_arvalid, "s_axi_arvalid" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_araddr, "s_axi_araddr" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_rready, "s_axi_rready" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_rvalid, "s_axi_rvalid" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_rdata, "s_axi_rdata" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_rresp, "s_axi_rresp" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_awready, "s_axi_awready" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_awvalid, "s_axi_awvalid" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_awaddr, "s_axi_awaddr" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_wready, "s_axi_wready" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_wvalid, "s_axi_wvalid" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_wdata, "s_axi_wdata" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_wstrb, "s_axi_wstrb" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_bready, "s_axi_bready" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_bvalid, "s_axi_bvalid" );
    sc_trace(fp, top.soc0.axi_slave.s_axi_bresp, "s_axi_bresp" );
#endif
#endif
/*** END OF EDITABLE SECTION ***/
#endif //ifdef TRACE

    //sc_trace(fp, top.soc0.axiBridge.it_signal, "it_signal");
    //sc_trace(fp, top.soc0.irqc.in_signals[0], "irqc_in_signals");

    /* Start simulation */
    sc_start();

    /* Cleanup */
    
    // Close trace file:
    sc_close_vcd_trace_file( fp );

    return 0;
}

#endif
