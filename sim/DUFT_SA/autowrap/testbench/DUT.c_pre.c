# 1 "/home/yd383/ap_DUFT/src/beh/DUT.c"
# 1 "/home/yd383/ap_DUFT/src/beh/DUT.c" 1
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 149 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/home/yd383/ap_DUFT/src/beh/DUT.c" 2
# 1 "../../../src/inc/type.h" 1
typedef unsigned int u32;
typedef unsigned char u8;
# 2 "/home/yd383/ap_DUFT/src/beh/DUT.c" 2

void dut_tick(u32* current_value, u32* current_state)
{
  if (*current_state < 9) {
    (*current_state) ++;
    (*current_value) ++;
  }
  else
    *current_state = 0;
}

void dut_run(u32* current_value)
{
  *current_value += 8;
}
