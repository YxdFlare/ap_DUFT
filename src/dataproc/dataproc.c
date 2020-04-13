#include "type.h"
#include "common.h"

void dataproc_avg(u32* img_set, float* result_set, u32 max_latency, u32 size)
{
  u32* img;
  img = img_set;
  u32* pix;
  int j,x,y;
  j = 0;
  x = 0;
  y = 0;
  float pwr = 0;
  for(j = 0; j < max_latency-1; j++) {
    img += y*3 + x*size*3 + j*size*size*3;
    pwr = 0;
    for(x = 0; x < size; x++)
      for(y = 0; y < size; y++) {
        pix = img +  y*3 + x*size*3;
        if(pix[1])
          pwr += 1;
        if(pix[2])
          pwr += 2;
      }
    pwr = pwr / size / size;
    result_set[j] = pwr;
  }
}