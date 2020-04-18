`timescale 1ns / 100ps
`include "./prewrapped_design.v"

module DUFT_ap_ctrl_chain(
  input [31:0] addr,
  input [31:0] wr_data,
  input        rd_wr, // 1 for read, 0 for write

  output [31:0] ap_return,
  input        ap_rst,
  input        ap_start,
  input        ap_continue,
  input        ap_ce,
  output reg   ap_idle,
  output reg   ap_ready,
  output reg   ap_done,

  input ap_clk
);

wire [31:0] rd_addr;
wire [31:0] wr_addr;

prewrapped core_DUFT (
  .clk(ap_clk),
  .reset(ap_rst),
  .axi_rd_addr(rd_addr),
  .axi_wr_addr(wr_addr),
  .axi_rd_msg(ap_return),
  .axi_wr_msg(wr_data)
);

// control logic
// read it combinational, write needs 1 cycle
reg rden;
reg wren;
assign rd_addr = (rden && ap_ce) ? addr : 32'hFFFFFFFF;
assign wr_addr = (wren && ap_ce) ? addr : 32'hFFFFFFFF;

localparam IDLE     = 3'd0;
localparam WR_ACK   = 3'd1;
localparam WR_DONE  = 3'd2;
localparam RD_DONE  = 3'd3;
localparam RST      = 3'd4;
localparam WR_RDY   = 3'd5;
localparam RD_RDY   = 3'd6;

localparam y = 1'b1;
localparam n = 1'b0;

wire rd_valid;
wire wr_valid;
assign rd_valid = ap_start && rd_wr;
assign wr_valid = ap_start && !rd_wr;

reg [2:0] current_state;
reg [2:0] next_state;

always @(posedge ap_clk ) begin
  if (ap_rst)
    current_state <= RST;
  else if (!ap_ce)
    current_state <= current_state;
  else
    current_state <= next_state;
end

always @(*) begin
  case (current_state)
    IDLE:
      if (rd_valid)
        next_state =  RD_RDY;
      else if (wr_valid)
        next_state = WR_ACK;
      else 
        next_state = IDLE;
    WR_ACK:
      next_state = WR_RDY;
    WR_RDY:
      next_state = WR_DONE;
    RD_RDY:
      next_state = RD_DONE;
    WR_DONE:
      if(ap_continue)
        next_state = IDLE;
      else
        next_state = WR_DONE;
    RD_DONE:
      if(ap_continue)
        next_state = IDLE;
      else
        next_state = RD_DONE;
    RST:
      next_state = IDLE;
    default: 
      next_state = RST;
  endcase
end

task cbout;
  input c_ap_idle;
  input c_ap_ready;
  input c_ap_done;
  input c_rden;
  input c_wren;
  begin
    ap_idle = c_ap_idle;
    ap_ready = c_ap_ready;
    ap_done = c_ap_done;
    rden = c_rden;
    wren = c_wren;
  end
endtask

always @(*) begin
  case (current_state)
    //                  ap    ap    ap    rd    wr
    //                  idle  rdy   done  en    en
    IDLE: 
      if(ap_start)cbout(n,    n,    n,    n,    n);
      else        cbout(y,    n,    n,    n,    n);
    WR_ACK:       cbout(n,    n,    n,    n,    y);
    WR_RDY:       cbout(n,    y,    y,    n,    n);
    RD_RDY:       cbout(n,    y,    y,    y,    n);
    WR_DONE:      cbout(n,    n,    y,    n,    n);
    RD_DONE:      cbout(n,    n,    y,    y,    n);
    RST:          cbout(n,    n,    n,    n,    n);
    default:      cbout(n,    n,    n,    n,    n);
  endcase
end

endmodule // DUFT_ap_ctrl_hs