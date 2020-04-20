#include "common.h"
#include "DUFT_ap_ctrl_chain.h"
#include "wrapper_constants.h"

//-------------------------------------------------------------------------------
// Top level
//-------------------------------------------------------------------------------

int top_standalone(u32 test_input)
{
  int passing = 1;
  DUFT_ap_ctrl_chain(TEST_IN_BASE, test_input, WRITE);
  int dut_output = DUFT_ap_ctrl_chain(TEST_OUT_BASE, 0, READ);
  passing = (test_input == dut_output);
  return passing;
}

int top_simple_wrapper(u32 addr, u32 data, u32 rd_wr)
{
  return DUFT_ap_ctrl_chain(addr, data, rd_wr);
}