module core(
    input clk,
    input [31:0]instr_data,
    input [31:0]last_pc,

    output [31:0]instr_addr,
    output [31:0]mem_addr,
    output [31:0]mem_data,
    output mem_we
);

reg [31:0]pc = 32'hFFFFFFFF;
wire [31:0]pc_target = branch_taken ? branch_target : pc + 1;
wire [31:0]pc_next = (pc == last_pc) ? pc : pc_target;

always @(posedge clk) begin
    pc <= pc_next;
`ifdef __ICARUS__
    $strobe("[pc = %h] %h", pc, instr);
    $strobe("taken = %b target = %h", branch_taken, branch_target);
`endif
end

wire [31:0]instr = instr_data;
assign instr_addr = pc_next;

wire [4:0]rd = instr[11:7];
wire [4:0]rs1 = instr[19:15];
wire [4:0]rs2 = instr[24:20];

wire [31:0]rf_rdata0;
wire [4:0]rf_raddr0 = rs1;

wire [31:0]rf_rdata1;
wire [4:0]rf_raddr1 = rs2;

wire [31:0]rf_wdata = alu_result;
wire [4:0]rf_waddr = rd;
wire rf_we;

assign mem_addr = alu_result;
assign mem_data = rf_rdata1;

wire has_imm;
wire [31:0]alu_result;
wire [31:0]alu_b_src = has_imm ? imm32 : rf_rdata1;
wire [2:0]alu_op;
alu alu(
    .src_a(rf_rdata0), .src_b(alu_b_src),
    .op(alu_op),
    .res(alu_result)
);

reg_file rf(
    .clk(clk),
    .raddr0(rf_raddr0), .rdata0(rf_rdata0),
    .raddr1(rf_raddr1), .rdata1(rf_rdata1),
    .waddr(rf_waddr), .wdata(rf_wdata), .we(rf_we)
);

wire [11:0]imm12;
wire [31:0]imm32 = {{20{imm12[11]}}, imm12};

wire cmp_res = /* Problem 2: comparison result */
wire branch_taken = /* Problem 2: is branch taken or not ? */
wire [31:0]branch_target = /* Problem 2: target address bus */
wire branch;

control control(
    .instr(instr),
    .imm12(imm12),
    .rf_we(rf_we),
    .alu_op(alu_op),
    .has_imm(has_imm),
    .mem_we(mem_we),
    .branch(branch)
);

endmodule
