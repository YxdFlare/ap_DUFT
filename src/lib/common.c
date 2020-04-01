#include "type.h"
#include <stdio.h>

int array_teq(u32* arr1, u32 len1, u32* arr2, u32 len2) 
{
  if(len1 != len2)
    return 0;
  else {
    int eq = 1;
    for(int i = 0; i < len1; i++)
      eq = eq && (arr1[i] == arr2[i]);
    return eq; 
  }
}

int waiting_timer(int* timer, int max)
{
  if(*timer < max) {
    *timer ++;
    return 0;
  }
  else
    return 1;    
}

int print_array(u32* arr, u32 len)
{
  for(int i = 0; i < len; i++)
    printf("%d\t",arr[i]);
  printf("\n");
}