// address mapping
#define OPCODE_BASE    0x00000000
#define STATE_BASE     0x00000001
#define CONFIG_BASE    0x00000002
#define DUT_IN_BASE    0x00000010
#define DUT_OUT_BASE   0x00000018
#define DFT_OUT_BASE   0x00000020
#define TEST_IN_BASE   0xFF000000
#define TEST_OUT_BASE  0xFF000001
#define INVALID_ADDR   0xFFFFFFFF
#define NULL_ADDR      0x12345678

typedef unsigned int u32;

typedef struct {
  u32* opcode;
  u32* state;
  u32* config;
  u32* dut_in;
  u32* dut_out;
  u32* dft_out;
  u32* test_in;
  u32* test_out;
} RF;
RF rf_ptr;
u32 opcode_REG = 0;
u32 state_REG = 0;
u32 config_REG = 0;
u32 dut_in_REG[8] = {0};
u32 dut_out_REG[8] = {0};
u32 dft_out_REG[64] = {0};
u32 test_in_REG = 0;
u32 test_out_REG = 0;
rf_ptr.opcode  = &opcode_REG;
rf_ptr.state   = &state_REG;
rf_ptr.config  = &config_REG;
rf_ptr.dut_in  = dut_in_REG;
rf_ptr.dut_out = dut_out_REG;
rf_ptr.dft_out = dft_out_REG;
rf_ptr.test_in = &test_in_REG;
rf_ptr.test_out = &test_out_REG;


void rf_update(RF regfile)
{

};

u32* addr_map(RF regfile, u32 addr)
{
  switch (addr)
  {
  case OPCODE_BASE:
    return regfile.opcode;
  case STATE_BASE:
    return regfile.state;
  case CONFIG_BASE:
    return regfile.config;
  case DUT_IN_BASE:
    return regfile.dut_in + addr - DUT_IN_BASE;
  case DUT_OUT_BASE:
    return regfile.dut_out + addr - DUT_OUT_BASE;
  case DFT_OUT_BASE:
    return regfile.dft_out + addr - DFT_OUT_BASE;
  case TEST_IN_BASE:
    return regfile.test_in;
  case TEST_OUT_BASE:
    return regfile.test_out;  
  default:
    return (u32*) NULL_ADDR;
  }
};



u32 DUFT_ap_ctrl_chain(u32 addr, u32 wr_data, u32 rd_wr)
{
  u32* ptr;
  ptr = addr_map(rf_ptr,addr);
  if(rd_wr)     
    return *ptr;
  else {
    *ptr = wr_data;
    rf_update(rf_ptr);
    return 0;
  }   
};