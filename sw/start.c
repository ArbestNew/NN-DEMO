/*
 * SimSoC Initial software, Copyright © INRIA 2007, 2008, 2009, 2010, 2011, 2012, 2013
 * LGPL license version 3
 */

#include "dbg_console.h"

#define halt()            DBGC_HALT = 1

extern int main();

void check_status() {
    int status = main();
    if (status!=0) {
        print_str("Embedded program returned with status: ");
        print_int(status);
        print_char('\n');
    }
    halt();
}
