#ifndef AXI_WRITE_DATA_RESPONSE_CHANNEL_MODEL_H
#define AXI_WRITE_DATA_RESPONSE_CHANNEL_MODEL_H

#include <systemc.h>
SC_MODULE(axi_write_data_response_channel_model) {

    sc_in<bool >  clk;
    sc_in<bool >  resetn;
    sc_in<sc_uint<2> >  bresp;
    sc_in<bool >  bvalid;
    sc_out<bool > bready;

    enum main_fsm_type {reset  ,idle ,success ,error ,complete};
    sc_signal<main_fsm_type > current_state,next_state;
    void state_machine_update();
    void state_machine_decisions();
    axi_write_data_response_channel_model(sc_module_name _n) : sc_module(_n) {
        SC_METHOD(state_machine_update);
        sensitive << clk ;SC_METHOD(state_machine_decisions);
        sensitive << current_state  << bresp << bvalid;
    }

    ~axi_write_data_response_channel_model() {
    }
    SC_HAS_PROCESS(axi_write_data_response_channel_model);

};
#endif
