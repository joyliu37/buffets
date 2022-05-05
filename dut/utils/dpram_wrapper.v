module dpram_aha (
				CLK,
				RESET,
				ARADDR,
				WADDR0,
				WVALID0,
				WADDR1,
				WVALID1,
				RDATA,
				WDATA0,
				WDATA1,
				RVALID,
				ARVALID
				);

parameter   ADDR_WIDTH  = 11;
parameter   DATA_WIDTH  = 16;
parameter   SEPARATE_WRITE_PORTS = 0;

input 					CLK, RESET;
input [ADDR_WIDTH-1:0] 	ARADDR;
input [ADDR_WIDTH-1:0] 	WADDR0, WADDR1;
input 					ARVALID;
input 					WVALID0, WVALID1;
input [DATA_WIDTH-1:0]	WDATA0, WDATA1;

output reg [DATA_WIDTH-1:0]		RDATA;
output reg				        RVALID;


// Return the read data
always @(posedge CLK or negedge RESET) begin
	if(~RESET) begin
		RVALID <= 1'b0;
	end
	else begin
		RVALID 	<= (ARVALID)? 1'b1 : 1'b0;
	end
end

wire [ADDR_WIDTH-1:0] WADDR;
wire [DATA_WIDTH-1:0] WDATA;
assign WADDR = WVALID0 ? WADDR0 : WADDR1;
assign WDATA = WVALID0 ? WDATA0 : WDATA1;
wire WEN;

assign WEN = WVALID0 || WVALID1;

N12LP_SDPB_W02048B016M08S2_H macro_dpram(
    .CLK_A(CLK),
    .CLK_B(CLK),
    .CEN_A(~WEN),
    .CEN_B(~ARVALID),
    .RDWEN_A(1'b0),
    .RDWEN_B(1'b1),
    .A_A(WADDR),
    .A_B(ARADDR),
    .D_A(WDATA),
    .D_B(),
    .T_LOGIC(1'b0),
    .T_Q_RST_A(1'b0),
    .T_Q_RST_B(1'b0),
    .Q_A(),
    .Q_B(RDATA),
    .OBSV_CTL_A(),
    .OBSV_CTL_B(),
    .MA_SAWL0(1'b0),
    .MA_SAWL1(1'b0),
    .MA_WL1(1'b0),
    .MA_WL0(1'b0),
    .MA_WRAS1(1'b0),
    .MA_WRAS0(1'b0),
    .MA_VD1(1'b0),
    .MA_VD0(1'b0),
    .MA_WRT(1'b0)
);

endmodule
