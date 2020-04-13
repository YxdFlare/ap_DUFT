#include "common.h"
#include "dataproc.h"
#include "encoder.h"
#include "DUFT_ap_ctrl_chain.h"
#include "wrapper_constants.h"

#include <stdio.h>
#include <stdlib.h>

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
RF _rf = {.opcode = NONE,.state = IDLE};
u32 _dut_state[1] = {0};
u32 _dut_value[1] = {0};

//-------------------------------------------------------------------------------
// DUFT helper functions
//-------------------------------------------------------------------------------

int send_op(u32 operation, u32 target_state)
{
  DUFT_ap_ctrl_chain(OPCODE_BASE,operation,WRITE);
  DUFT_ap_ctrl_chain(OPCODE_BASE,NONE,WRITE);
  u32 landed_state = DUFT_ap_ctrl_chain(STATE_BASE,0,READ);
  if (landed_state != target_state) {
    printf("[ERROR] : Error when transiting into state %d, actual landed at %d\n",target_state,landed_state);
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

int call_dft(u32 input,u32* dft_buf,int dump_nbr)
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
    for(i = 0; i < dump_nbr; i++)
      *(dft_buf + lat * DUMP_NBR + i) = DUFT_ap_ctrl_chain(DFT_OUT_BASE + i,0,READ);
    err = send_op(TICK,SCAN_RD);
    if(err) return err;
    lat++;
  }
  for(i = 0; i < dump_nbr; i++)
    *(dft_buf + lat * DUMP_NBR + i) = DUFT_ap_ctrl_chain(DFT_OUT_BASE + i,0,READ);
  err = send_op(ENDT,IDLE);
  if(err) return err;
  return 0;
}

#define ITEM_NBR 10
#define CH_NBR 3

//-------------------------------------------------------------------------------
// Test Harness
//-------------------------------------------------------------------------------

int main()
{
  printf("\n\n\nIN MAIN : \n=================\n");
  // define data structures (memory allocation)
  int SIZE = calc_size(DUMP_NBR);
  /* ai */                  u32 test_inputs[ITEM_NBR];
  /* ai+8*/                 u32 dut_outputs[ITEM_NBR];
  /* ai,ai+1,ai+2...*/      u32 dcs[ITEM_NBR][MAX_LATENCY][DUMP_NBR]; // dft_collected_states
                            u32* dcs_ptr = &dcs[0][0][0];
  /* img0i,img1i,img2i...*/ u32 encoded_imgset[ITEM_NBR][MAX_LATENCY-1][SIZE][SIZE][CH_NBR];
                            u32* encoded_imgset_ptr = &encoded_imgset[0][0][0][0][0];
  /* pwr0i,pwr1i,pwr2i...*/ float final_results[ITEM_NBR][MAX_LATENCY-1];
                            float* final_results_ptr = &final_results[0][0];
  printf("Data structure initialized.\n");
  printf("Initializing inputs......");
  int i = 0; // index for test items
  for (i = 0; i < ITEM_NBR; i++)
    test_inputs[i] = rand();
  printf("Inputs initialized.\nInitializing test harness......");

  int all_passing = 1;
  int err_dft = 0;
  int err_dut = 0;
  i = 0;
  printf("Test harness initialized. Start testing.\n");
  printf("--------------------------------------\n");
  while (i < ITEM_NBR && all_passing) {
    printf("Test case %d : ",i);
    // pointer bumping
    dcs_ptr += i * MAX_LATENCY * DUMP_NBR;
    printf(".");
    encoded_imgset_ptr += i * (MAX_LATENCY-1) * SIZE * SIZE * CH_NBR;
    printf(".");
    final_results_ptr += i * (MAX_LATENCY-1);
    printf(".");
    // input a number to the DUFT and get dcs
    err_dft = call_dft(test_inputs[i],dcs_ptr,DUMP_NBR);
    printf(".");
    // input a number to the DUT and get output
    err_dut = call_dut(test_inputs[i],&dut_outputs[i]);
    printf(".");
    // encode dcs into images
    batch_encode(dcs_ptr,encoded_imgset_ptr,DUMP_NBR,MAX_LATENCY);
    printf(".");
    // process these images
    dataproc_avg(encoded_imgset_ptr,final_results_ptr,MAX_LATENCY,SIZE);
    printf(".");
    // update testsource status
    all_passing = all_passing && !err_dft && !err_dut;
    printf(" %s\n",all_passing ? "[NO ERROR]\0" : "[ERROR]\0");
    i++;
  }
  // error message
  if(!all_passing)
    printf("[ERROR] : Test FAILED at case %d\n",i-1);

  //print results
  int j = 0; // index for cycles
  int k = 0; // index for dumps
  for (i = 0; i < ITEM_NBR; i++) {
    // pointer bumping
    printf("--------------------------------------\nTest case %d\n",i);
    printf("Data input and output : %d => %d\n",test_inputs[i],dut_outputs[i]);
    printf("Collected states : ");
    for (j = 0; j < MAX_LATENCY; j++) {
      printf("(");
      for (k = 0; k < DUMP_NBR; k++) {
        printf("%d,",dcs[i][j][k]);
      }
      printf(")\t");
    }
    printf("\n");      
    printf("Analysed results : ");
    for (j = 0; j < MAX_LATENCY-1; j++) {
      printf("%f\t",final_results[i][j]);
    }
    printf("\n");
  }
  printf("--------------------------------------\n");
  return 1 - all_passing;
}