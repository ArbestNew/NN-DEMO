open_project VLSIProject
source "config.tcl"

#csim_design -clean
csynth_design
export_design 
close_project
open_project VLSIProject
source "config.tcl"
catch { cosim_design -setup -O -ldflags {-m64 -L/opt/eds/systemc/simsoc/lib-linux64-gcc-7.3.1 -lsimsoc} -argv {../../sw/app.elf} -trace_level all -rtl vhdl -tool modelsim  -compiled_library_dir /opt/eds/modeltech/10.6/xilinx }
exit
