//Stub for GF macro
module N12LP_SDPB_W02048B016M08S2_H
  (
    input CLK_A,
    input CLK_B,
    input CEN_A,
    input CEN_B,
    input RDWEN_A,
    input RDWEN_B,
    input [10:0] A_A,
    input [10:0] A_B,
    input [15:0] D_A,
    input [15:0] D_B,
    input T_LOGIC,
    input T_Q_RST_A,
    input T_Q_RST_B,
    output reg [15:0] Q_A,
    output reg [15:0] Q_B,
    output [1:0] OBSV_CTL_A,
    output [1:0] OBSV_CTL_B,
    input MA_SAWL1,
    input MA_SAWL0,
    input MA_WL1,
    input MA_WL0,
    input MA_WRAS1,
    input MA_WRAS0,
    input MA_VD1,
    input MA_VD0,
    input MA_WRT
);
reg [15:0] memory[2047:0];

always @(posedge CLK_A) begin
    if(~CEN_A ) begin
      if(RDWEN_A) begin
        Q_A <= memory[A_A];
      end
      else begin
        memory[A_A] <= D_A;
      end
    end

    if(~CEN_B) begin
      if (RDWEN_B) begin
        Q_B <= memory[A_B];
      end
      else begin
        memory[A_B] <= D_B;
      end
    end
end

endmodule
