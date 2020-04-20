echo "Deleting existing wave files"
rm -f ~/ap_DUFT/wave/*.wdb
rm -f ~/ap_DUFT/wave/*.wcfg
echo "Moving wave data and config of solution 'solution1'"
cp ~/ap_DUFT/build-vivado/zcu102/solution1/sim/verilog/top.wdb ~/ap_DUFT/wave/top.wdb
cp ~/ap_DUFT/build-vivado/zcu102/solution1/sim/verilog/top.wcfg ~/ap_DUFT/wave/top.wcfg
echo "Moving wave data and config of solution 'solution1_SA'"
cp ~/ap_DUFT/build-vivado/zcu102/solution1_SA/sim/verilog/top_standalone.wdb ~/ap_DUFT/wave/top_standalone.wdb
cp ~/ap_DUFT/build-vivado/zcu102/solution1_SA/sim/verilog/top_standalone.wcfg ~/ap_DUFT/wave/top_standalone.wcfg
echo "Moving wave data and config of solution 'solution1_AC_C'"
cp ~/ap_DUFT/build-vivado/zcu102/solution1_AC_C/sim/verilog/dummy_ap_ctrl_chain.wdb ~/ap_DUFT/wave/dummy_ap_ctrl_chain.wdb
cp ~/ap_DUFT/build-vivado/zcu102/solution1_AC_C/sim/verilog/dummy_ap_ctrl_chain.wcfg ~/ap_DUFT/wave/dummy_ap_ctrl_chain.wcfg
echo "Done."