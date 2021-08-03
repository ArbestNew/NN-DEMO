/***********************************************************************
 * tb_init.cpp
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

#include "tb_init.h"

void GenReset::initialReset() {
    // Start simulation with an active LOW reset:
    reset.write(false);
    std::cout << reset << " " << sc_time_stamp() << std::endl;
    // Keep reset active for 2 clock cycles:
    // NOTE: for a "high-level" simulation one wait() should be enough. They represent synchronization points rather than actual clock cycles.
    for (int i = 0; i < 2; ++i) {
        wait();
    }
    // De-activate the reset signal:
    reset.write(true);
    std::cout << reset << " " << sc_time_stamp() << std::endl;

#ifdef SCSIM_WHATCHDOG
    // Wait SCSIM_WHATCHDOG number of clock cycles (or better said, synchronization points) and force
    wait(SCSIM_WHATCHDOG);
    sc_stop();
#endif

    // This clock thread is not needed any more on the simulation, so it finish here:
}

GenReset::GenReset(sc_module_name name) :
reset("Reset") {
    SC_CTHREAD(initialReset, clk.pos());
}
