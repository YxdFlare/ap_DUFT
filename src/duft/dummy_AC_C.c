int dummy_ap_ctrl_chain(int input)
{
  #pragma HLS interface ap_ctrl_chain port=return bundle=CRTL_BUS
  int result = 0;
  for (int i = 0; i < (input & 0x1111); i++) {
    #pragma HLS loop_flatten off
    result += (input & 0x00007216) * i; 
  }  
  return result;
}