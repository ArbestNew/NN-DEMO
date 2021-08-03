#ifndef AXI_READ_DATA_CHANNEL_MODEL_H
#define AXI_READ_DATA_CHANNEL_MODEL_H

#include <systemc.h>
#undef COMPLETE
#define RESET				0
#define IDLE				1
#define TRANSACTION_OKAY	2
#define TRANSACTION_ERROR	3
#define COMPLETE			4

typedef sc_uint<3> main_fsm_type;

SC_MODULE(axi_read_data_channel_model) {

    sc_in<bool >  clk;
    sc_in<bool >  resetn;
    sc_in<bool >  read_address_done;
    sc_out<bool >  done;

    sc_out<sc_uint<32> > data;
    sc_in<sc_uint<32> >  rdata;
    sc_in<sc_uint<2> >  rresp;
    sc_in<bool >  rvalid;
    sc_out<bool > rready;


    sc_signal<main_fsm_type > current_state,next_state;
    void state_machine_update();
    void state_machine_decisions();
    void comb_assignments();
    axi_read_data_channel_model(sc_module_name _n) : sc_module(_n) {
        SC_METHOD(state_machine_update);
        sensitive << clk ;
	SC_METHOD(state_machine_decisions);
        sensitive << current_state  << rvalid << rresp << read_address_done;
        SC_METHOD(comb_assignments);
        sensitive << rvalid << rdata;
    }

    ~axi_read_data_channel_model() {
    }
    SC_HAS_PROCESS(axi_read_data_channel_model);

};
#endif
