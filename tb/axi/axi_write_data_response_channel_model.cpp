#include "axi_write_data_response_channel_model.h"


void axi_write_data_response_channel_model::state_machine_update() {
    if (clk.posedge()) {
        if (resetn.read() == 0) {
            current_state.write((main_fsm_type)(reset ));
        } else {
            current_state.write((main_fsm_type)(next_state.read() ));


        }
    }
}
void axi_write_data_response_channel_model::state_machine_decisions() {
    bready.write((bool)(0 ));

    switch (current_state.read()) {
    case reset :
        next_state.write((main_fsm_type)(idle ));
        break;
    case idle :
        next_state.write((main_fsm_type)(idle ));
        bready.write((bool)(1 ));
        if (bvalid.read() == 1) {
            if (bresp.read() == (sc_uint<2>)"0b00") {
                next_state.write((main_fsm_type)(success ));
            } else {
                next_state.write((main_fsm_type)(error ));
            }
        } break;
    case success :
        next_state.write((main_fsm_type)(complete ));
        break;
    case error :
        next_state.write((main_fsm_type)(complete ));
        break;
    case complete :
        next_state.write((main_fsm_type)(complete ));
        if (bvalid.read() == 0) {
            next_state.write((main_fsm_type)(idle ));

        } break;
    default :
        next_state.write((main_fsm_type)(reset ));
        break;

    }
}

