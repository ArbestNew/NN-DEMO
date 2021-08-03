#include "axi_read_data_channel_model.h"


void axi_read_data_channel_model::state_machine_update() {
    if (clk.posedge()) {
        if (resetn.read() == 0) {
            current_state.write((main_fsm_type)(RESET ));
        } else {
            current_state.write((main_fsm_type)(next_state.read() ));


        }
    }
}
void axi_read_data_channel_model::state_machine_decisions() {
    done.write((bool)(0 ));

    switch (current_state.read()) {
    case RESET :
        next_state.write((main_fsm_type)(IDLE ));
        break;
    case IDLE :
        next_state.write((main_fsm_type)(IDLE ));
    rready.write((bool)(0 ));
        if (rvalid.read() == 1) {
            if (rresp.read() == (sc_uint<2>)"0b00") {
                next_state.write((main_fsm_type)(TRANSACTION_OKAY ));
    		done.write((bool)(1 ));
            } else {
                next_state.write((main_fsm_type)(TRANSACTION_ERROR ));
            }
        }
		break;
    case TRANSACTION_OKAY :
		if (read_address_done.read() == 1) {
        	next_state.write((main_fsm_type)(COMPLETE ));
        	rready.write((bool)(1 ));
		}
		else
        	next_state.write((main_fsm_type)(IDLE ));
        break;
    case TRANSACTION_ERROR :
        next_state.write((main_fsm_type)(COMPLETE ));
        rready.write((bool)(1 ));
        break;
    case COMPLETE :
        if (rvalid.read() == 0) {
            next_state.write((main_fsm_type)(IDLE ));
        	rready.write((bool)(0 ));
        } 
		else
        next_state.write((main_fsm_type)(COMPLETE ));
break;
    default :
        next_state.write((main_fsm_type)(RESET ));
        break;

    }
}
void axi_read_data_channel_model::comb_assignments() {

    data.write(((rvalid.read() == 1)) ? rdata.read()  : "0x00000000" );

}

