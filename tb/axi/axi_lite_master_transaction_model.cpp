#include "axi_lite_master_transaction_model.h"
using namespace std;

void axi_lite_master_transaction_model::state_machine_update() {
    if (m_axi_lite_aclk.posedge()) {
        if (m_axi_lite_aresetn.read() == 0) {
            current_state.write((lmt_fsm_type)(RESET ));
        } else {
            current_state.write((lmt_fsm_type)(next_state.read() ));


        }
    }
}
void axi_lite_master_transaction_model::state_machine_decisions() {
    write_channel_data.write((sc_uint<32>)(write_data.read() ));
    transaction_address.write((sc_uint<32>)(address.read() ));
    start_read_transaction.write((bool)(0 ));
    start_write_transaction.write((bool)(0 ));
    send_write_data.write((bool)(0 ));
    busy.write((bool)(1 ));
    done.write((bool)(0 ));

    switch (current_state.read()) {
    case RESET :
        next_state.write((lmt_fsm_type)(IDLE ));
        break;
    case IDLE :
        next_state.write((lmt_fsm_type)(IDLE ));
        busy.write((bool)(0 ));
        if (go.read() == 1) {

//            switch (rnw.read()) {
//            case 1 :
//                next_state.write((lmt_fsm_type)(READ_TRANSACTION ));
//                break;
//            case 0 :
//                next_state.write((lmt_fsm_type)(WRITE_TRANSACTION ));
//                break;
//            default :
//                break;
//
//            }

            if (rnw.read()) 
                next_state.write((lmt_fsm_type)(READ_TRANSACTION ));
            else 
                next_state.write((lmt_fsm_type)(WRITE_TRANSACTION ));

        } break;
    case READ_TRANSACTION :
        next_state.write((lmt_fsm_type)(READ_TRANSACTION ));
        start_read_transaction.write((bool)(1 ));
        if (read_transaction_finished.read() == 1) {
            next_state.write((lmt_fsm_type)(COMPLETE ));

        } break;
    case WRITE_TRANSACTION :
        next_state.write((lmt_fsm_type)(WRITE_TRANSACTION ));
        start_write_transaction.write((bool)(1 ));
        send_write_data.write((bool)(1 ));
        if ((write_transaction_finished.read() == 1) && (write_data_sent.read() == 1)) {
            next_state.write((lmt_fsm_type)(COMPLETE ));

        } break;
    case COMPLETE :
        next_state.write((lmt_fsm_type)(COMPLETE ));
        done.write((bool)(1 ));
        if (go.read() == 0) {
            next_state.write((lmt_fsm_type)(IDLE ));

        } break;
    default :
        next_state.write((lmt_fsm_type)(RESET ));
        break;

    }
}
void axi_lite_master_transaction_model::comb_assignments() {

    read_data.write((sc_uint<32>)(read_channel_data.read() ));
    resetn.write((bool)(m_axi_lite_aresetn.read() ));

}

