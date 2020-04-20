set part_name "xczu9eg-ffvb1156-2-e"
set project_name "zcu102"

open_project -reset $project_name

add_files  ../src/duft/dummy_AC_C.c \
-cflags ""

# Set the top-level function to be top from "../src/top/top.c"
set_top dummy_ap_ctrl_chain

# SPECIFY MODEL TO TEST HERE
add_files -tb ../src/test/HLS_test_ap_ctrl_chain.c \
-cflags "-I../src/duft"

open_solution "solution1_AC_C"
set_part $part_name

create_clock -period 10

csim_design -ldflags "--verbose"

csynth_design

# CHOOSE EITHER VCD FILE GENERATION (1st option) or COSIM EXECUTION (2nd option)
cosim_design -ldflags "" -trace_level all
