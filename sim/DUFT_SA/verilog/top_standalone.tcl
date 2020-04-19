
log_wave [get_objects -filter {type == in_port || type == out_port || type == inout_port || type == port} /apatb_top_standalone_top/AESL_inst_top_standalone/*]
set designtopgroup [add_wave_group "Design Top Signals"]
set coutputgroup [add_wave_group "C Outputs" -into $designtopgroup]
set return_group [add_wave_group return(wire) -into $coutputgroup]
add_wave /apatb_top_standalone_top/AESL_inst_top_standalone/ap_return -into $return_group -radix hex
set dut_output_group [add_wave_group dut_output(wire) -into $coutputgroup]
add_wave /apatb_top_standalone_top/AESL_inst_top_standalone/dut_output_ap_vld -into $dut_output_group -color #ffff00 -radix hex
add_wave /apatb_top_standalone_top/AESL_inst_top_standalone/dut_output -into $dut_output_group -radix hex
set cinputgroup [add_wave_group "C Inputs" -into $designtopgroup]
set test_input_group [add_wave_group test_input(wire) -into $cinputgroup]
add_wave /apatb_top_standalone_top/AESL_inst_top_standalone/test_input -into $test_input_group -radix hex
set blocksiggroup [add_wave_group "Block-level IO Handshake" -into $designtopgroup]
add_wave /apatb_top_standalone_top/AESL_inst_top_standalone/ap_start -into $blocksiggroup
add_wave /apatb_top_standalone_top/AESL_inst_top_standalone/ap_done -into $blocksiggroup
add_wave /apatb_top_standalone_top/AESL_inst_top_standalone/ap_idle -into $blocksiggroup
add_wave /apatb_top_standalone_top/AESL_inst_top_standalone/ap_ready -into $blocksiggroup
set resetgroup [add_wave_group "Reset" -into $designtopgroup]
add_wave /apatb_top_standalone_top/AESL_inst_top_standalone/ap_rst -into $resetgroup
set clockgroup [add_wave_group "Clock" -into $designtopgroup]
add_wave /apatb_top_standalone_top/AESL_inst_top_standalone/ap_clk -into $clockgroup
save_wave_config top_standalone.wcfg
run all
quit

