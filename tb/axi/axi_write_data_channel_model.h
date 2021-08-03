#ifndef AXI_WRITE_DATA_CHANNEL_MODEL_H
#define AXI_WRITE_DATA_CHANNEL_MODEL_H

#include <systemc.h>
#undef COMPLETE
#define RESET				0
#define IDLE				1
#define RUNNING				2
#define COMPLETE			3


typedef sc_uint<2> wdc_fsm_type;

SC_MODULE(axi_write_data_channel_model) {

    sc_in<bool >  clk;
    sc_in<bool >  resetn;
    sc_in<sc_uint<32> >  data;
    sc_in<bool >  go;
    sc_out<bool > done;
    sc_out<sc_uint<32> > wdata;
    sc_out<sc_uint<4> > wstrb;
    sc_out<bool > wvalid;
    sc_in<bool >  wready;

    sc_signal<wdc_fsm_type > current_state,next_state;
    sc_signal<bool > output_data;
    void state_machine_update();
    void state_machine_decisions();
    void comb_assignments();
    axi_write_data_channel_model(sc_module_name _n) : sc_module(_n) {
        SC_METHOD(state_machine_update);
        sensitive << clk ;SC_METHOD(state_machine_decisions);
        sensitive << current_state  << wready << go;
        SC_METHOD(comb_assignments);
        sensitive << output_data << data;
    }

    ~axi_write_data_channel_model() {
    }
    SC_HAS_PROCESS(axi_write_data_channel_model);

};
#endif
