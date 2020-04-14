#include "type.h"
#include "common.h"
#include <stdio.h>

void dataproc_avg(u32* img_set, float* result_set, u32 max_latency, u32 size)
{
  u32* pix;
  float pwr = 0;
  for(int j = 0; j < max_latency-1; j++) {
    pwr = 0;
    for(int x = 0; x < size; x++) {
      for(int y = 0; y < size; y++) {
        pix = img_set +  y*3 + x*size*3 + j*size*size*3;
        if(pix[1])
          pwr += 1;
        if(pix[2])
          pwr += 2;
      }
    }
    pwr = pwr / size / size;
    result_set[j] = pwr;
  }
}