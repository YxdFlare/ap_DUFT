#include "type.h"
#include "common.h"
#include "wrapper_constants.h"

void dataproc_avg(u32 img_set[(MAX_LATENCY-1)*SIZE*SIZE*CH_NBR], float result_set[MAX_LATENCY-1])
{
  u32* pix;
  float pwr = 0;
  for(int j = 0; j < MAX_LATENCY -1; j++) {
    pwr = 0;
    for(int x = 0; x < SIZE; x++) {
      for(int y = 0; y < SIZE; y++) {
        pix = img_set +  y*3 + x*SIZE*3 + j*SIZE*SIZE*3;
        if(pix[1])
          pwr += 1;
        if(pix[2])
          pwr += 2;
      }
    }
    pwr = pwr / SIZE / SIZE;
    *(result_set + j) = pwr;
  }
}