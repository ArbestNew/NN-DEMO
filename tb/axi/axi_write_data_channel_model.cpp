#include "axi_write_data_channel_model.h"


void axi_write_data_channel_model::state_machine_update() {
    if (clk.posedge()) {
        if (resetn.read() == 0) {
            current_state.write((wdc_fsm_type)(RESET ));
        } else {
            current_state.write((wdc_fsm_type)(next_state.read() ));


        }
    }
}
void axi_write_data_channel_model::state_machine_decisions() {
    wstrb.write((sc_uint<4>)("0b0000" ));
    wvalid.write((bool)(0 ));
    output_data.write((bool)(0 ));
    done.write((bool)(0 ));

    switch (current_state.read()) {
    case RESET :
        next_state.write((wdc_fsm_type)(IDLE ));
        break;
    case IDLE :
        next_state.write((wdc_fsm_type)(IDLE ));
        if (go.read() == 1) {
            next_state.write((wdc_fsm_type)(RUNNING ));

        } break;
    case RUNNING :
        output_data.write((bool)(1 ));
        wstrb.write((sc_uint<4>)("0b1111" ));
        wvalid.write((bool)(1 ));
        if (wready.read() == 1) {
            next_state.write((wdc_fsm_type)(COMPLETE ));
        } else {
            next_state.write((wdc_fsm_type)(RUNNING ));


        } break;
    case COMPLETE :
        done.write((bool)(1 ));
        if (go.read() == 0) {
            next_state.write((wdc_fsm_type)(IDLE ));
        } else {
            next_state.write((wdc_fsm_type)(COMPLETE ));


        } break;
    default :
        next_state.write((wdc_fsm_type)(RESET ));
        break;

    }
}
void axi_write_data_channel_model::comb_assignments() {

    wdata.write(((output_data.read() == 1)) ? data.read()  : "0x00000000" );

}

