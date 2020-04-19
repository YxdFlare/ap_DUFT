
/opt/xilinx/Xilinx_Vivado_vitis_2019.2/Vivado/2019.2/bin/xelab xil_defaultlib.apatb_top_standalone_top glbl -prj top_standalone.prj -L smartconnect_v1_0 -L axi_protocol_checker_v1_1_12 -L axi_protocol_checker_v1_1_13 -L axis_protocol_checker_v1_1_11 -L axis_protocol_checker_v1_1_12 -L xil_defaultlib -L unisims_ver -L xpm --initfile "/opt/xilinx/Xilinx_Vivado_vitis_2019.2/Vivado/2019.2/data/xsim/ip/xsim_ip.ini" --lib "ieee_proposed=./ieee_proposed" -s top_standalone -debug wave
/opt/xilinx/Xilinx_Vivado_vitis_2019.2/Vivado/2019.2/bin/xsim --noieeewarnings top_standalone -tclbatch top_standalone.tcl

