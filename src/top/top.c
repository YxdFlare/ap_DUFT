#include "common.h"
#include "dataproc.h"
#include "encoder.h"
#include "DUFT_ap_ctrl_chain.h"
#include "wrapper_constants.h"
#include "top_constants.h"

// register file used for the FL model of DUFT
typedef struct {
  u32 opcode;
  u32 state;
  u32 config;
  u32 dut_in_ptr[8];
  u32 dut_out_ptr[8];
  u32 dft_out_ptr[64];
  u32 test_in;
  u32 test_out;
} RF;
extern RF _rf;
extern u32 _dut_state[1];
extern u32 _dut_value[1];


//-------------------------------------------------------------------------------
// Top level
//-------------------------------------------------------------------------------

int top(int func, u32 addr, u32 data, int rd_wr, u32 dcs[MAX_LATENCY*DUMP_NBR], float final_results[MAX_LATENCY-1])
{
  // define data structures (memory allocation)
  /* img0i,img1i,img2i...*/ u32 encoded_imgset[MAX_LATENCY-1][SIZE][SIZE][CH_NBR];
                            u32* encoded_imgset_ptr = &encoded_imgset[0][0][0][0];
  int DUFT_return = 0;
  switch (func) {
  case DUFT:
    DUFT_return = DUFT_ap_ctrl_chain(addr,data,rd_wr);
    break;
  case ENCODE:
    batch_encode(dcs,encoded_imgset_ptr);
    break;
  case PROCESS:
    dataproc_avg(encoded_imgset_ptr,final_results);
    break;  
  default:
    break;
  } 
  return DUFT_return;
}