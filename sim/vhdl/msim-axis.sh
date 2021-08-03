###############################################################
#   ModelSim AXI Slave script                                 #
# It synthesizes the VHDL testbench generated on the SystemC  #
# simulation and the AXI Master VHDL files for RTL simulation #
# with ModelSim.                                              #
# It will create the 'work' folder for the simulation files.  #
###############################################################
vlib work
vcom -64 -quiet  "axi_address_control_channel_model.vhd"
vcom -64 -quiet  "axi_lite_master_transaction_model.vhd"
vcom -64 -quiet  "axi_read_data_channel_model.vhd"
vcom -64 -quiet  "axi_write_data_channel_model.vhd"
vcom -64 -quiet  "axi_write_response_channel_model.vhd"
vcom -64 -quiet  "tbsrc.vhd"
