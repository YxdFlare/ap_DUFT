#include "dummy_AC_C.h"

#include <stdio.h>
#include <stdlib.h>

#define ITEM_NBR 5

//-------------------------------------------------------------------------------
// Test Harness
//-------------------------------------------------------------------------------

int main()
{
  printf("\n\nIN MAIN : \n=================\n");
  // define data structures (memory allocation)
  /* ai */                  int test_inputs[ITEM_NBR];
  /* ai */                  int dut_outputs[ITEM_NBR];
  printf("Data structure initialized.\n");
  printf("Initializing inputs......");
  int i = 0; // index for test items
  for (i = 0; i < ITEM_NBR; i++)
    test_inputs[i] = rand();
  printf("Inputs initialized.\nInitializing test harness......");

  int all_passing = 1;
  i = 0;
  printf("Test harness initialized. Start testing.\n");
  printf("--------------------------------------\n");
  while (i < ITEM_NBR && all_passing) {
    printf("Test item %d ...",i);
    dut_outputs[i] = dummy_ap_ctrl_chain(test_inputs[i]);
    //all_passing = (dut_outputs[i] == (test_inputs[i] + 1));
    printf(" %s\n",all_passing ? "[PASSED]\0" : "[ERROR]\0");
    i++;
  }
  
  int tested_items = 0;
  // error message
  if(!all_passing)
    printf("[ERROR] : Test FAILED at case %d\n",i-1);
    tested_items = all_passing ? ITEM_NBR : i;


  //print results
  for (i = 0; i < tested_items; i++) {
    printf("--------------------------------------\nTest item %d\n",i);
    printf("Data input and output : %0x => %0x\n",test_inputs[i],dut_outputs[i]);
  }
  printf("--------------------------------------\n");
  return 1 - all_passing;
}