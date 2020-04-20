#include "DUT.h"
//#include "type.h"
#include "wrapper_constants.h"
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
  case IDLE:
    if(regfile->opcode == INPUT) {
      _dut_value[0] = regfile->dut_in_ptr[0];
      _dut_state[0] = 0;
      regfile->state = INPUT_RDY;
    }
    break;
  
  case INPUT_RDY:
    if(regfile->opcode == RUN) {
      dut_run(_dut_value);
      regfile->state = OUTPUT_VAL;
    }
    else if (regfile->opcode == TEST) {
      dft_scan(regfile,DUMP_NBR);
      regfile->state = SCAN_RD;
    }
    break;
  
  case OUTPUT_VAL:
    if(regfile->opcode == ENDR) {
      regfile->dut_out_ptr[0] = _dut_value[0];
      regfile->state = IDLE;
    }
    break;
  
  case SCAN_RD:
    if(regfile->opcode == ENDT) {
      regfile->state = IDLE;
    }
    else if(regfile->opcode == TICK) {
      dut_tick(_dut_value,_dut_state);
      dft_scan(regfile,DUMP_NBR);
      regfile->state = SCAN_RD;
    }
    break;
    
  default:
    break;
  }
  regfile->test_out = regfile->test_in;
}

u32* addr_map(RF* regfile, u32 addr)
{
  // locating register files
  if (DUT_IN_BASE <= addr && addr < DFT_OUT_BASE + 64) {
    if(DUT_IN_BASE <= addr && addr < DUT_IN_BASE + 8)
      return regfile->dut_in_ptr + addr - DUT_IN_BASE;
    else if (DUT_OUT_BASE <= addr && addr < DUT_OUT_BASE + 8)
      return regfile->dut_out_ptr + addr - DUT_OUT_BASE;
    else
      return regfile->dft_out_ptr + addr - DFT_OUT_BASE;
  }
  // locating single registers
  else switch (addr) {
    case OPCODE_BASE:
      return &(regfile->opcode);
    case STATE_BASE:
      return &(regfile->state);
    case CONFIG_BASE:
      return &(regfile->config);
    case TEST_IN_BASE:
      return &(regfile->test_in);
    case TEST_OUT_BASE:
      return &(regfile->test_out);  
    default:
      return (u32*) NULL_ADDR;
  }
}

u32 DUFT_ap_ctrl_chain(u32 addr, u32 wr_data, u32 rd_wr)
{
  #pragma HLS latency min=3 max=3
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