#ifndef AXI_ADDRESS_CONTROL_CHANNEL_MODEL_H
#define AXI_ADDRESS_CONTROL_CHANNEL_MODEL_H

#include <systemc.h>
#undef COMPLETE
#define RESET				0
#define IDLE				1
#define RUNNING				2
#define COMPLETE			3

typedef sc_uint<2> acc_fsm_type;

SC_MODULE(axi_address_control_channel_model) {

    sc_in<bool >  clk;
    sc_in<bool >  resetn;
    sc_in<bool >  go;
    sc_out<bool > done;
    sc_in<sc_uint<32> >  address;
    sc_out<sc_uint<32> > axaddr;
    sc_out<bool > axvalid;
    sc_in<bool >  axready;

    sc_signal<acc_fsm_type > current_state,next_state;
    sc_signal<bool > address_enable;
    void state_machine_update();
    void state_machine_decisions();
    void comb_assignments();
    axi_address_control_channel_model(sc_module_name _n) : sc_module(_n) {
        SC_METHOD(state_machine_update);
        sensitive << clk ;SC_METHOD(state_machine_decisions);
        sensitive << current_state  << go << axready;
        SC_METHOD(comb_assignments);
        sensitive << address_enable << address;
    }

    ~axi_address_control_channel_model() {
    }
    SC_HAS_PROCESS(axi_address_control_channel_model);

};
#endif
