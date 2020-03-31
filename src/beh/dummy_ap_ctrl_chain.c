#include "common.h"
#include "type.h"
extern u32 _reg;
u32 dummy_ap_ctrl_chain(u32 addr, u32 wr_data, u32 rd_wr)
{
  if(addr == 0)
    if(rd_wr)
      return _reg;
    else{
      _reg = wr_data;
      return 0;
    }
  else
    return 0;    
};