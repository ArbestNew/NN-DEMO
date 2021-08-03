################################################################
#                        Clean script                          #
# It cleans all the directories from the garbage files created #
# during simulations and synthesis.                            #
# It should be executed before re-executing any simulation or  #
# synthesis, so previous results do not interfere with the new #
# ones.                                                        #
################################################################

# Clean all directories from object (.o) and dependency (.d) files:
rm -f */*.o
rm -f */*.d
rm -f */*/*.o
rm -f */*/*.d

# Clean the 'tb' directory:
rm -f tb/sctop
rm -f tb/LOG
rm -f tb/tr.vcd
rm -f tb/tbsrc.vhd
rm -f tb/tbsrc.sv
rm -f tb/*.out

# Clean the 'sw' directory:
rm -f sw/app.elf

# Remove the automatically created 'VLSIProject' directory for the VivadoHLS project:
rm -rf ./VLSIProject/solution1/.autopilot/
rm -f vivado_hls.log


# Clean the 'sim/sysc' directory, but it has not been submitted yet:
#rm -f sim/sysc/sctop
#rm -f sim/sysc/*.vcd
#rm -f sim/sysc/*.dat

# Clean the 'sim/vhdl' directory:
rm -f sim/vhdl/tbsrc.vhd
rm -rf sim/vhdl/work
rm -f sim/vhdl/transcript
rm -f sim/vhdl/vsim.wlf

# Clean the 'fpga' directory (it has its own clean script), but it has not been submitted yet:
#cd fpga
#sh clean.sh
