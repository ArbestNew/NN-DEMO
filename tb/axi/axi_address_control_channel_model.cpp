#include "axi_address_control_channel_model.h"


void axi_address_control_channel_model::state_machine_update() {
    if (clk.posedge()) {
        if (resetn.read() == 0) {
            current_state.write((acc_fsm_type)(RESET ));
        } else {
            current_state.write((acc_fsm_type)(next_state.read() ));


        }
    }
}
void axi_address_control_channel_model::state_machine_decisions() {
    done.write((bool)(0 ));
    address_enable.write((bool)(0 ));
    axvalid.write((bool)(0 ));

    switch (current_state.read()) {
    case RESET :
        next_state.write((acc_fsm_type)(IDLE ));
        break;
    case IDLE :
        next_state.write((acc_fsm_type)(IDLE ));
        	done.write((bool)(0 ));
        if (go.read() == 1) {
            next_state.write((acc_fsm_type)(RUNNING ));

        }
	break;
    case RUNNING :
        next_state.write((acc_fsm_type)(RUNNING ));
        address_enable.write((bool)(1 ));
        axvalid.write((bool)(1 ));
        if (axready.read() == 1) {
            next_state.write((acc_fsm_type)(COMPLETE ));

        }
	break;
    case COMPLETE :
        if (go.read() == 0) {
        	done.write((bool)(0 ));
            next_state.write((acc_fsm_type)(IDLE ));

        }
		else {
        	next_state.write((acc_fsm_type)(COMPLETE ));
        	done.write((bool)(1 ));
		}
		break;
    default :
        next_state.write((acc_fsm_type)(RESET ));
        break;

    }
}
void axi_address_control_channel_model::comb_assignments() {

    axaddr.write(((address_enable.read() == 1)) ? address.read()  : sc_uint<32>(0));

}

