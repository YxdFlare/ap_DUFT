#include "common.h"
#include "dataproc.h"
#include "encoder.h"
#include "DUFT_ap_ctrl_chain.h"
#include "wrapper_constants.h"

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
// DUFT helper functions
//-------------------------------------------------------------------------------

int send_op(u32 operation, u32 target_state)
{
  DUFT_ap_ctrl_chain(OPCODE_BASE,operation,WRITE);
  DUFT_ap_ctrl_chain(OPCODE_BASE,NONE,WRITE);
  u32 landed_state = DUFT_ap_ctrl_chain(STATE_BASE,0,READ);
  if (landed_state != target_state) {
    //printf("[ERROR] : Error when transiting into state %d, actual landed at %d\n",target_state,landed_state);
    return 1;
  }
  return 0;
}

int call_dut(u32 input, u32* output)
{
  int err = 0;
  DUFT_ap_ctrl_chain(DUT_IN_BASE,input,WRITE);
  err = send_op(INPUT,INPUT_RDY);
  if(err) return err;
  err = send_op(RUN,OUTPUT_VAL);
  if(err) return err;
  err = send_op(ENDR,IDLE);
  if(err) return err;
  *output = DUFT_ap_ctrl_chain(DUT_OUT_BASE,0,READ);
  return 0;  
}

int call_dft(u32 input,u32* dft_buf)
{
  int err = 0;
  int lat = 0;
  int i = 0;
  DUFT_ap_ctrl_chain(DUT_IN_BASE,input,WRITE);
  err = send_op(INPUT,INPUT_RDY);
  if(err) return err;
  err = send_op(TEST,SCAN_RD);
  if(err) return err;
  while (_dut_state[0] < 9) {
    for(i = 0; i < DUMP_NBR; i++)
      *(dft_buf + lat * DUMP_NBR + i) = DUFT_ap_ctrl_chain(DFT_OUT_BASE + i,0,READ);
    err = send_op(TICK,SCAN_RD);
    if(err) return err;
    lat++;
  }
  for(i = 0; i < DUMP_NBR; i++)
    *(dft_buf + lat * DUMP_NBR + i) = DUFT_ap_ctrl_chain(DFT_OUT_BASE + i,0,READ);
  err = send_op(ENDT,IDLE);
  if(err) return err;
  return 0;
}

//-------------------------------------------------------------------------------
// Top level
//-------------------------------------------------------------------------------

int top(u32* test_inputs, u32* dut_outputs, float* final_results)
{
  // define data structures (memory allocation)
  /* ai,ai+1,ai+2...*/      u32 dcs[MAX_LATENCY][DUMP_NBR]; // dft_collected_states
                            u32* dcs_ptr = &dcs[0][0];
  /* img0i,img1i,img2i...*/ u32 encoded_imgset[MAX_LATENCY-1][SIZE][SIZE][CH_NBR];
                            u32* encoded_imgset_ptr = &encoded_imgset[0][0][0][0];
  int all_passing = 1;
  int err_dft = 0;
  int err_dut = 0;
  // input a number to the DUFT and get dcs
  err_dft = call_dft(*test_inputs,dcs_ptr);
  // input a number to the DUT and get output
  err_dut = call_dut(*test_inputs,dut_outputs);
  // encode dcs into images
  batch_encode(dcs_ptr,encoded_imgset_ptr);
  // process these images
  dataproc_avg(encoded_imgset_ptr,final_results);
  // update testsource status
  all_passing = !err_dft && !err_dut;
    
  return all_passing;
}