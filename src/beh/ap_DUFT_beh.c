typedef unsigned int u32;

u32 DUFT_ap_ctrl_chain(u32 addr, u32 wr_data, u32 rd_wr)
{
  u32 result;
  result = (addr + wr_data) * rd_wr;
  return result;
};