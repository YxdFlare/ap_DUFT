#include "common.h"
#include "DUFT_ap_ctrl_chain.h"
#include "wrapper_constants.h"

//-------------------------------------------------------------------------------
// Top level
//-------------------------------------------------------------------------------

int DUFT(u32 addr, u32 wr_data, u32 rd_wr)
{
  if(rd_wr == READ)
    return DUFT_ap_ctrl_chain(addr, 0 , READ);
  else if (rd_wr == WRITE) {
    DUFT_ap_ctrl_chain(addr, wr_data, WRITE);
    return 0;
  }
  else
    return 0;  
}