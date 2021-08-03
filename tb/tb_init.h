/***********************************************************************
 * tb_init.h
 * SystemC module to generate the reset signal for the simulation.
 * It may also include a whatchdog to stop the simulation if it gets too
 * long (maybe because a critical event that causes the end of the
 * simulation was missed).
 * 
 * Version 1.0:
 *   Author: unknown
 *   Date: unknown
 * 
 * Version 2.0:
 *   Author: David Aledo
 *   Date: 27/03/2019
 */

#ifndef TB_INIT_H
#define TB_INIT_H
#include <systemc.h>

SC_MODULE(GenReset) {
public:
    sc_out<bool> reset;
    sc_in_clk clk;

    typedef GenReset SC_CURRENT_USER_MODULE;
    GenReset(sc_module_name name);
private:
    void initialReset();
};

#endif
