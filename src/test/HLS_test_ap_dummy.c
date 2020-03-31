#include <stdio.h>
#include "dummy_ap_ctrl_chain.h"
#include "common.h"
#include "type.h"

u32 _reg = 0;
#define SIZE 10

int main()
{
  printf("In main\n");
  u32 a[SIZE];
  u32 b[SIZE];
  int i;
  for (i = 0; i < SIZE; i++) {
    a[i] = 0x7216 + i;
    b[i] = 0;
  }
  u32 test_addr = 0;
  for (i = 0; i < SIZE; i++) {
    dummy_ap_ctrl_chain(test_addr,a[i],0);
    b[i] = dummy_ap_ctrl_chain(test_addr,0,1);
  }
  
  int passed = 0;
  passed = array_teq(a,SIZE,b,SIZE);
  if(passed) {
    printf("Test PASSED\n");
    return 0;
  }
  else {
    printf("Test FAILED\n");
    return 1;
  }
};
