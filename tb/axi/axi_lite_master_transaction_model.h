#ifndef AXI_LITE_MASTER_TRANSACTION_MODEL_H
#define AXI_LITE_MASTER_TRANSACTION_MODEL_H

#include <systemc.h>
#include "axi_address_control_channel_model.h"
#include "axi_read_data_channel_model.h"
#include "axi_write_data_channel_model.h"
#include "axi_write_data_response_channel_model.h"
#undef COMPLETE
#define RESET				0
#define IDLE				1
#define READ_TRANSACTION	2
#define WRITE_TRANSACTION	3
#define COMPLETE			4


typedef sc_uint<3> lmt_fsm_type;

SC_MODULE(axi_lite_master_transaction_model) {

    sc_in<bool >  go;
    sc_in<bool >  rnw;
    sc_out<bool > busy;
    sc_out<bool > done;
    sc_in<sc_uint<32> >  address;
    sc_in<sc_uint<32> >  write_data;
    sc_out<sc_uint<32> > read_data;
    sc_in<bool >  m_axi_lite_aclk;
    sc_in<bool >  m_axi_lite_aresetn;
    sc_in<bool >  m_axi_lite_arready;
    sc_out<bool > m_axi_lite_arvalid;
    sc_out<sc_uint<32> > m_axi_lite_araddr;
    sc_out<bool > m_axi_lite_rready;
    sc_in<bool >  m_axi_lite_rvalid;
    sc_in<sc_uint<32> >  m_axi_lite_rdata;
    sc_in<sc_uint<2> >  m_axi_lite_rresp;
    sc_in<bool >  m_axi_lite_awready;
    sc_out<bool > m_axi_lite_awvalid;
    sc_out<sc_uint<32> > m_axi_lite_awaddr;
    sc_in<bool >  m_axi_lite_wready;
    sc_out<bool > m_axi_lite_wvalid;
    sc_out<sc_uint<32> > m_axi_lite_wdata;
    sc_out<sc_uint<4> > m_axi_lite_wstrb;
    sc_out<bool > m_axi_lite_bready;
    sc_in<bool >  m_axi_lite_bvalid;
    sc_in<sc_uint<2> >  m_axi_lite_bresp;

    sc_signal<lmt_fsm_type > current_state,next_state;
    sc_signal<sc_uint<32> > read_channel_data;
    sc_signal<sc_uint<32> > write_channel_data;
    sc_signal<sc_uint<32> > transaction_address;
    sc_signal<bool > start_read_transaction;
    sc_signal<bool > start_write_transaction;

    sc_signal<bool > read_transaction_finished;
    sc_signal<bool > read_address_finished;

    sc_signal<bool > write_transaction_finished;
    sc_signal<bool > send_write_data;
    sc_signal<bool > write_data_sent;
    sc_signal<bool > resetn;
    void state_machine_update();
    void state_machine_decisions();
    void comb_assignments();
    axi_address_control_channel_model read_address_channel_model;
    axi_address_control_channel_model write_address_channel_model;
    axi_read_data_channel_model read_data_channel_model;
    axi_write_data_channel_model write_data_channel_model;
    axi_write_data_response_channel_model write_data_response_channel_model;

    axi_lite_master_transaction_model(sc_module_name _n) : sc_module(_n),read_address_channel_model("read_address_channel_model"),write_address_channel_model("write_address_channel_model"),read_data_channel_model("read_data_channel_model"),write_data_channel_model("write_data_channel_model"),write_data_response_channel_model("write_data_response_channel_model")
    {
        read_address_channel_model.clk(m_axi_lite_aclk);
        read_address_channel_model.resetn(resetn);
        read_address_channel_model.go(start_read_transaction);

        read_address_channel_model.done(read_address_finished); //////////////////

        read_address_channel_model.address(transaction_address);
        read_address_channel_model.axaddr(m_axi_lite_araddr);
        read_address_channel_model.axvalid(m_axi_lite_arvalid);

        read_address_channel_model.axready(m_axi_lite_arready);

        write_address_channel_model.clk(m_axi_lite_aclk);
        write_address_channel_model.resetn(resetn);
        write_address_channel_model.go(start_write_transaction);
        write_address_channel_model.done(write_transaction_finished);
        write_address_channel_model.address(transaction_address);
        write_address_channel_model.axaddr(m_axi_lite_awaddr);
        write_address_channel_model.axvalid(m_axi_lite_awvalid);
        write_address_channel_model.axready(m_axi_lite_awready);

        read_data_channel_model.clk(m_axi_lite_aclk);
        read_data_channel_model.resetn(resetn);
        read_data_channel_model.data(read_channel_data);
        read_data_channel_model.rdata(m_axi_lite_rdata);
        read_data_channel_model.rresp(m_axi_lite_rresp);
        read_data_channel_model.rvalid(m_axi_lite_rvalid);
        read_data_channel_model.rready(m_axi_lite_rready);


        read_data_channel_model.done(read_transaction_finished);
        read_data_channel_model.read_address_done(read_address_finished);


        write_data_channel_model.clk(m_axi_lite_aclk);
        write_data_channel_model.resetn(resetn);
        write_data_channel_model.go(send_write_data);
        write_data_channel_model.done(write_data_sent);
        write_data_channel_model.data(write_channel_data);
        write_data_channel_model.wdata(m_axi_lite_wdata);
        write_data_channel_model.wstrb(m_axi_lite_wstrb);
        write_data_channel_model.wvalid(m_axi_lite_wvalid);
        write_data_channel_model.wready(m_axi_lite_wready);
        write_data_response_channel_model.clk(m_axi_lite_aclk);
        write_data_response_channel_model.resetn(resetn);
        write_data_response_channel_model.bresp(m_axi_lite_bresp);
        write_data_response_channel_model.bvalid(m_axi_lite_bvalid);
        write_data_response_channel_model.bready(m_axi_lite_bready);

        SC_METHOD(state_machine_update);
        sensitive << m_axi_lite_aclk ;
		
		SC_METHOD(state_machine_decisions);
        sensitive << current_state  << read_address_finished << read_transaction_finished << write_transaction_finished << go << rnw << address << write_data << read_channel_data << write_data_sent;
        SC_METHOD(comb_assignments);
        sensitive << read_channel_data << m_axi_lite_aresetn;
    }

    ~axi_lite_master_transaction_model() {
    }
    SC_HAS_PROCESS(axi_lite_master_transaction_model);

};
#endif
