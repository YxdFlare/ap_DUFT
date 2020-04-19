set part_name "xczu9eg-ffvb1156-2-e"
set project_name "zcu102"

open_project -reset $project_name

add_files  ../src/lib/common.c \
-cflags "-I../src/beh -I../src/lib -I../src/inc -I../src/top"
add_files   ../src/top/top_standalone.c \
-cflags "-I../src/beh -I../src/lib -I../src/inc -I../src/top"
add_files   ../src/beh/DUFT_ap_ctrl_chain.c \
-cflags "-I../src/beh -I../src/lib -I../src/inc -I../src/top"
add_files   ../src/beh/DUT.c \
-cflags "-I../src/beh -I../src/lib -I../src/inc -I../src/top"

add_files -blackbox ../src/blackbox/DUFT_bkb.json

# Set the top-level function to be top from "../src/top/top.c"
set_top top_standalone

# SPECIFY MODEL TO TEST HERE
add_files -tb ../src/test/tb_top_SA.c \
-cflags "-I../src/beh -I../src/lib -I../src/inc -I../src/top"

open_solution "solution1_SA"
set_part $part_name

create_clock -period 10

#csim_design -ldflags "--verbose"

csynth_design

# CHOOSE EITHER VCD FILE GENERATION (1st option) or COSIM EXECUTION (2nd option)
cosim_design -ldflags "" -trace_level all
