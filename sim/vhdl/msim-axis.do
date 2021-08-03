###############################################################
#   ModelSim AXI Slave simulation script                      #
# It executes the RTL simulation with ModelSim.               #
###############################################################
vsim -64 -novopt  work.axi_master_testbench
#vsim -voptargs=+acc work.
