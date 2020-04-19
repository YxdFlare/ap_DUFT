#include "common.h"
#include "DUFT_ap_ctrl_chain.h"
#include "wrapper_constants.h"

//-------------------------------------------------------------------------------
// Top level
//-------------------------------------------------------------------------------

int top_standalone(u32 test_input, u32* dut_output)
{
  int passing = 1;
  DUFT_ap_ctrl_chain(TEST_IN_BASE, test_input, WRITE);
  *dut_output = DUFT_ap_ctrl_chain(TEST_OUT_BASE, NULL_ADDR, READ);
  passing = (test_input == *dut_output);  
  return passing;
}