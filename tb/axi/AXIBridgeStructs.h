#ifndef AXIBRIDGESTRUCTS_H
#define AXIBRIDGESTRUCTS_H

#include <systemc>
using namespace sc_core;
using namespace sc_dt;

#include "AXIParameters.h"

struct AXI_Signals {
    sc_signal<bool > go;
    sc_signal<bool > rnw;
    sc_signal<bool > busy;
    sc_signal<bool > done;
    sc_signal<sc_uint<32> > address;
    sc_signal<sc_uint<32> > write_data;
    sc_signal<sc_uint<32> > read_data;

    sc_signal<bool > interrupt_request;

    sc_signal<bool > axi_lite_arready;
    sc_signal<bool > axi_lite_rvalid;
    sc_signal<sc_uint<AXIParameters::BITWIDTH> > axi_lite_rdata;
    sc_signal<sc_uint<2> > axi_lite_rresp;
    sc_signal<bool > axi_lite_awready;
    sc_signal<bool > axi_lite_wready;
    sc_signal<bool > axi_lite_bvalid;
    sc_signal<sc_uint<2> > axi_lite_bresp;
    sc_signal<bool > axi_lite_arvalid;
    sc_signal<sc_uint<AXIParameters::BITWIDTH> > axi_lite_araddr;
    sc_signal<sc_uint<3> > axi_lite_arprot;
    sc_signal<bool > axi_lite_rready;
    sc_signal<bool > axi_lite_awvalid;
    sc_signal<sc_uint<AXIParameters::BITWIDTH> > axi_lite_awaddr;
    sc_signal<sc_uint<3> > axi_lite_awprot;
    sc_signal<bool > axi_lite_wvalid;
    sc_signal<sc_uint<AXIParameters::BITWIDTH> > axi_lite_wdata;
    sc_signal<sc_uint<4> > axi_lite_wstrb;
    sc_signal<bool > axi_lite_bready;
};


#endif

