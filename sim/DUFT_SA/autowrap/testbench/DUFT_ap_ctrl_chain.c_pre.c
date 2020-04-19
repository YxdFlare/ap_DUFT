# 1 "/home/yd383/ap_DUFT/src/beh/DUFT_ap_ctrl_chain.c"
# 1 "/home/yd383/ap_DUFT/src/beh/DUFT_ap_ctrl_chain.c" 1
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 149 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/home/yd383/ap_DUFT/src/beh/DUFT_ap_ctrl_chain.c" 2
# 1 "../../../src/beh/DUT.h" 1
# 1 "../../../src/inc/type.h" 1
typedef unsigned int u32;
typedef unsigned char u8;
# 2 "../../../src/beh/DUT.h" 2
void dut_tick(u32* current_value, u32* current_state);
void dut_run(u32* current_value);
# 2 "/home/yd383/ap_DUFT/src/beh/DUFT_ap_ctrl_chain.c" 2

# 1 "../../../src/inc/wrapper_constants.h" 1
# 4 "/home/yd383/ap_DUFT/src/beh/DUFT_ap_ctrl_chain.c" 2
typedef struct {
  u32 opcode;
  u32 state;
  u32 config;
  u32 dut_in_ptr[8];
  u32 dut_out_ptr[8];
  u32 dft_out_ptr[64];
  u32 test_in;
  u32 test_out;
} RF;

extern RF _rf;
extern u32 _dut_value[1];
extern u32 _dut_state[1];

void dft_scan(RF* regfile, int dump_nbr)
{
  for (int i = 0; i < dump_nbr; i++)
    regfile->dft_out_ptr[i] = _dut_value[i];
}

void rf_update(RF* regfile)
{
  switch (regfile->state)
  {
  case 0:
    if(regfile->opcode == 1) {
      _dut_value[0] = regfile->dut_in_ptr[0];
      _dut_state[0] = 0;
      regfile->state = 3;
    }
    break;

  case 3:
    if(regfile->opcode == 2) {
      dut_run(_dut_value);
      regfile->state = 5;
    }
    else if (regfile->opcode == 4) {
      dft_scan(regfile,1);
      regfile->state = 9;
    }
    break;

  case 5:
    if(regfile->opcode == 3) {
      regfile->dut_out_ptr[0] = _dut_value[0];
      regfile->state = 0;
    }
    break;

  case 9:
    if(regfile->opcode == 6) {
      regfile->state = 0;
    }
    else if(regfile->opcode == 10) {
      dut_tick(_dut_value,_dut_state);
      dft_scan(regfile,1);
      regfile->state = 9;
    }
    break;

  default:
    break;
  }
  regfile->test_out = regfile->test_in;
}

u32* addr_map(RF* regfile, u32 addr)
{

  if (0x00000010 <= addr && addr < 0x00000020 + 64) {
    if(0x00000010 <= addr && addr < 0x00000010 + 8)
      return regfile->dut_in_ptr + addr - 0x00000010;
    else if (0x00000018 <= addr && addr < 0x00000018 + 8)
      return regfile->dut_out_ptr + addr - 0x00000018;
    else
      return regfile->dft_out_ptr + addr - 0x00000020;
  }

  else switch (addr) {
    case 0x00000000:
      return &(regfile->opcode);
    case 0x00000001:
      return &(regfile->state);
    case 0x00000002:
      return &(regfile->config);
    case 0xFF000000:
      return &(regfile->test_in);
    case 0xFF000001:
      return &(regfile->test_out);
    default:
      return (u32*) 0x12345678;
  }
}

u32 DUFT_ap_ctrl_chain(u32 addr, u32 wr_data, u32 rd_wr)
{
  u32* ptr;
  ptr = addr_map(&_rf,addr);
  if(rd_wr)
    return *ptr;
  else {
    *ptr = wr_data;
    rf_update(&_rf);
    return 0;
  }
}
