echo "Moving wave data and config of solution 'solution1'"
cp ~/ap_DUFT/build-vivado/zcu102/solution1/sim/verilog/top.wdb ~/ap_DUFT/wave/top.wdb
cp ~/ap_DUFT/build-vivado/zcu102/solution1/sim/verilog/top.wcfg ~/ap_DUFT/wave/top.wcfg
echo "Moving wave data and config of solution 'solution1_SA'"
cp ~/ap_DUFT/build-vivado/zcu102/solution1_SA/sim/verilog/top_standalone.wdb ~/ap_DUFT/wave/top_standalone.wdb
cp ~/ap_DUFT/build-vivado/zcu102/solution1_SA/sim/verilog/top_standalone.wcfg ~/ap_DUFT/wave/top_standalone.wcfg
echo "Done."