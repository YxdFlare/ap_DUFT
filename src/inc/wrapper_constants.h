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

// state def 
#define IDLE           0
#define INPUT_FLATTEN  1
#define INPUT_DUT      2
#define INPUT_RDY      3
#define OUTPUT_WAIT    4
#define OUTPUT_VAL     5
#define OUTPUT_PACK    6
#define SCAN_PREP      7
#define SCAN           8
#define SCAN_RD        9
#define TICK           10
#define INVALID_STATE  0xffffffff

// opcode codebook
#define NONE  0
#define INPUT 1
#define RUN   2
#define ENDR  3
#define TEST  4
#define NEXT  5
#define ENDT  6

// scan chain parameters
#define DUMP_NBR 1
#define CHAIN_NBR 1

// DUT parameter
#define MAX_LATENCY 9

// DUFT operations
#define WRITE  0
#define READ   1

// encoder dimensions
#define CH_NBR 3
#define SIZE   6