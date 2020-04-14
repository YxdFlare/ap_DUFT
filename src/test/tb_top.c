#include "common.h"
#include "wrapper_constants.h"
#include "top.h"
#include "encoder.h"

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

#define ITEM_NBR 5

//-------------------------------------------------------------------------------
// Test Harness
//-------------------------------------------------------------------------------

int main()
{
  printf("\n\nIN MAIN : \n=================\n");
  // define data structures (memory allocation)
  int SIZE = calc_size(DUMP_NBR);
  /* ai */                  u32 test_inputs[ITEM_NBR];
                            u32* test_inputs_ptr = &test_inputs[0];
  /* ai+8*/                 u32 dut_outputs[ITEM_NBR];
                            u32* dut_outputs_ptr = &dut_outputs[0];
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
    printf("Test item %d ...",i);
    all_passing = top(test_inputs_ptr,dut_outputs_ptr,final_results_ptr,MAX_LATENCY,DUMP_NBR);   
    printf(" %s\n",all_passing ? "[NO ERROR]\0" : "[ERROR]\0");
    i++;
    // pointer bumping
    test_inputs_ptr += 1;
    dut_outputs_ptr += 1;
    final_results_ptr += (MAX_LATENCY-1);
  }
  int tested_items =0;
  // error message
  if(!all_passing)
    printf("[ERROR] : Test FAILED at case %d\n",i-1);
    tested_items = all_passing ? ITEM_NBR : i;


  //print results
  int j = 0; // index for cycles
  int k = 0; // index for dumps
  for (i = 0; i < tested_items; i++) {
    printf("--------------------------------------\nTest item %d\n",i);
    printf("Data input and output : %0x => %0x\n",test_inputs[i],dut_outputs[i]);
    printf("Analysed results :\t");
    for (j = 0; j < MAX_LATENCY-1; j++) {
      printf("%5.3f ",final_results[i][j]);
    }
    printf("\n");
  }
  printf("--------------------------------------\n");
  return 1 - all_passing;
}