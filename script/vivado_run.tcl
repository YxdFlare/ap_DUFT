set part_name "xczu9eg-ffvb1156-2-e"
set project_name "zcu102"

open_project -reset $project_name

add_files -tb ../src/lib/common.c \
-cflags "-I../src/beh -I../src/lib -I../src/inc"

add_files -blackbox ../src/blackbox/dummy_bkb.json

# Set the top-level function to be main from "../src/test/HLS_test_ap_dummy.c"
set_top main

# SPECIFY MODEL TO TEST HERE
add_files -tb ../src/test/HLS_test_ap_dummy.c \
-cflags "-I../src/beh -I../src/lib -I../src/inc"

open_solution "solution1"
set_part $part_name

create_clock -period 10

csim_design -ldflags "--verbose"

csynth_design

# CHOOSE EITHER VCD FILE GENERATION (1st option) or COSIM EXECUTION (2nd option)
cosim_design -ldflags "" -trace_level port
