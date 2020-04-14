#include "type.h"
#include "wrapper_constants.h"
#include "common.h"
#include <stdio.h>

// reform the current state into 1-channel square-image
// current_cycle[0],[1]...[dump_nbr-1] => img[x][y] = {0 or 1}
void reform(u32* current_cycle, u32* img, int dump_nbr, int size)
{
  int i,j;
  int bit_idx;
  int arr_idx;
  for(i = 0;i<size;i++)
    for(j =0;j<size;j++) {
      bit_idx = i * size + j;
      arr_idx = bit_idx / 32;
      bit_idx = bit_idx % 32;
      if (arr_idx < dump_nbr)
        *(img + i * size + j) = get_bit(current_cycle[arr_idx],bit_idx);
      else
        *(img + i * size + j) = 0;
    }
}

void set_color(u32* pix, u32 R, u32 G, u32 B)
{
  *pix = R;
  *(pix + 1) = G;
  *(pix + 2) = B;
}

//the first channel is 1 if the signal bit does not switch
//the second channel is 1 if the signal bit switches from 0 to 1
//the third channel is 1 if the signal bit switches from 1 to 0
void encode(u32* prev_cycle, u32* current_cycle, u32* diff_img, int dump_nbr, int size)
{
  int i,j,k;
  u32* pix;
  for(i = 0;i<size;i++)
    for(j =0;j<size;j++) {
        pix = diff_img + i * size * 3 + j*3;
      if(*(prev_cycle + i*size + j) == 1 && *(current_cycle + i*size + j) == 0)
        set_color(pix,0,0,1);
      else if(*(prev_cycle + i*size + j) == 0 && *(current_cycle + i*size + j) == 1)
        set_color(pix,0,1,0);
      else
        set_color(pix,1,0,0);
    }
}

void batch_encode(u32* collected_states, u32* img_set)
{
  int cycle_idx;
  u32 prev[SIZE][SIZE];
  u32 current[SIZE][SIZE];
  u32* img_set_ptr;
  for(cycle_idx = 1; cycle_idx < MAX_LATENCY; cycle_idx++) {
    reform(&collected_states[cycle_idx-1],&prev[0][0],DUMP_NBR,SIZE);
    reform(&collected_states[cycle_idx],&current[0][0],DUMP_NBR,SIZE);
    img_set_ptr = img_set + (cycle_idx - 1) * SIZE * SIZE * 3;
    encode(&prev[0][0],&current[0][0],img_set_ptr,DUMP_NBR,SIZE);
  }
}