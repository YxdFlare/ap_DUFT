// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.2.1 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
# 1 "/home/yd383/ap_DUFT/src/top/top_standalone.c"
# 1 "/home/yd383/ap_DUFT/src/top/top_standalone.c" 1
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 149 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/home/yd383/ap_DUFT/src/top/top_standalone.c" 2
# 1 "../../../src/lib/common.h" 1
# 1 "../../../src/inc/type.h" 1
typedef unsigned int u32;
typedef unsigned char u8;
# 2 "../../../src/lib/common.h" 2

void array_rst(u32* arr, u32 X, u32 Y, u32 Z, u32 C);
int array_teq(u32* ,u32 ,u32* ,u32 );
int array_teq2d(u32* arr1, u32 X1, u32 Y1, u32* arr2, u32 X2, u32 Y2);
int array_teq3d(u32* arr1, u32 X1, u32 Y1, u32 Z1, u32* arr2, u32 X2, u32 Y2, u32 Z2);
int waiting_timer(int* timer, int max);
void print_array(u32* arr, u32 len);
void print_array2d(u32* arr, u32 X, u32 Y);
void print_array3d(u32* arr, u32 X, u32 Y, u32 Z);
u32 get_bit(u32 data, int idx);
# 2 "/home/yd383/ap_DUFT/src/top/top_standalone.c" 2
# 1 "../../../src/beh/DUFT_ap_ctrl_chain.h" 1
# 1 "../../../src/inc/type.h" 1
typedef unsigned int u32;
typedef unsigned char u8;
# 2 "../../../src/beh/DUFT_ap_ctrl_chain.h" 2
u32 DUFT_ap_ctrl_chain(u32 addr, u32 wr_data, u32 rd_wr);
# 3 "/home/yd383/ap_DUFT/src/top/top_standalone.c" 2
# 1 "../../../src/inc/wrapper_constants.h" 1
# 4 "/home/yd383/ap_DUFT/src/top/top_standalone.c" 2





int top_standalone(u32 test_input, u32* dut_output)
{
  int passing = 1;
  DUFT_ap_ctrl_chain(0xFF000000, test_input, 0);
  *dut_output = DUFT_ap_ctrl_chain(0xFF000001, 0x12345678, 1);
  passing = (test_input == *dut_output);
  return passing;
}
