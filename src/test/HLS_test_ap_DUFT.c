#include "DUFT_ap_ctrl_chain.h"
#include "common.h"
#include "wrapper_constants.h"
#include <stdio.h>
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
u32 _dut_value[1] = {0};
u32 _dut_state[1] = {0};


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

int call_dut(u32 input)
{
  int err = 0;
  DUFT_ap_ctrl_chain(DUT_IN_BASE,input,WRITE);
  err = send_op(INPUT,INPUT_RDY);
  if(err) return err;
  err = send_op(RUN,OUTPUT_VAL);
  if(err) return err;
  err = send_op(ENDR,IDLE);
  if(err) return err;
  return DUFT_ap_ctrl_chain(DUT_OUT_BASE,0,READ);  
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

int basic_dut_test(u32* inputs, u32* collected, int item_nbr)
{
  for (int i = 0; i < item_nbr; i++)
    collected[i] = call_dut(inputs[i]);
  return 0;  
}

int basic_duft_test(u32* inputs, u32* collected, int item_nbr)
{
  for (int i = 0; i < item_nbr; i++)
    call_dft(inputs[i],collected + i * MAX_LATENCY,DUMP_NBR);
  return 0;
}

#define basic_dut_test_items 10
#define basic_duft_test_items 10

int main()
{
  int all_passing = 1;

  u32 basic_dut_test_inputs[basic_dut_test_items];
  u32 basic_dut_test_expected[basic_dut_test_items];
  u32 basic_dut_test_collected[basic_dut_test_items];
  int i,j,k;
  for (i = 0; i < basic_dut_test_items; i++) {
    basic_dut_test_inputs[i] = 1 + i;
    basic_dut_test_expected[i] = basic_dut_test_inputs[i] + 8;
  }
  basic_dut_test(basic_dut_test_inputs,basic_dut_test_collected,basic_dut_test_items);
  int basic_dut_test_passed = array_teq(basic_dut_test_expected,basic_dut_test_items,basic_dut_test_collected,basic_dut_test_items);
  all_passing = all_passing && basic_dut_test_passed;
  if(basic_dut_test_passed) {
    printf("Basic Directed DUT Test PASSED\n");
  }
  else {
    printf("Basic Directed DUT Test FAILED\n");
    printf("Expected data:------------------------------------------------------------\n\t");
    print_array(basic_dut_test_expected,basic_dut_test_items);
    printf("Collected data:-----------------------------------------------------------\n\t");
    print_array(basic_dut_test_collected,basic_dut_test_items);
  }

  u32 basic_duft_test_inputs[basic_duft_test_items];
  u32 basic_duft_test_expected[basic_duft_test_items][MAX_LATENCY][DUMP_NBR];
  u32 basic_duft_test_collected[basic_duft_test_items][MAX_LATENCY][DUMP_NBR];
  for (i = 0; i < basic_duft_test_items; i++)
    basic_duft_test_inputs[i] = 1 + 2*i;
  for (i = 0; i < basic_duft_test_items; i++)
    for(j = 0; j < MAX_LATENCY; j++)
      for(k = 0; k < DUMP_NBR; k++)
          basic_duft_test_expected[i][j][k] = basic_duft_test_inputs[i] + j;

    
  basic_duft_test(basic_duft_test_inputs,basic_duft_test_collected,basic_duft_test_items);
  int basic_duft_test_passed = 1;
  for (i = 0; i < basic_duft_test_items; i++)
    for(j = 0; j < MAX_LATENCY; j++)
        basic_duft_test_passed = basic_duft_test_passed && array_teq(basic_duft_test_expected[i][j],DUMP_NBR,basic_duft_test_collected[i][j],DUMP_NBR);

  all_passing = all_passing && basic_duft_test_passed;
  if(basic_duft_test_passed) {
    printf("Basic Directed DUFT Test PASSED\n");
  }
  else {
    printf("Basic Directed DUFT Test FAILED\n");
    printf("Expected data:------------------------------------------------------------\n");
    for(i = 0; i < basic_duft_test_items; i++) {
      printf("\n");
      for(j = 0; j < MAX_LATENCY; j++) {
        printf("\t");
        printf("(");
        for(k = 0; k < DUMP_NBR; k++) {
          printf("%d,",basic_duft_test_expected[i][j][k]);
        }
        printf(")");
      }
    }
    printf("\nCollected data:-----------------------------------------------------------\n");
    for(i = 0; i < basic_duft_test_items; i++) {
      printf("\n");
      for(j = 0; j < MAX_LATENCY; j++) {
        printf("\t");
        printf("(");
        for(k = 0; k < DUMP_NBR; k++) {
          printf("%d,",basic_duft_test_collected[i][j][k]);
        }
        printf(")");
      }
    }
  }

  return all_passing ? 0 : 1;
}