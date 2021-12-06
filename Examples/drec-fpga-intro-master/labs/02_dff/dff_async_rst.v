/*
*   Introduction to FPGA and Verilog
*
*   Viktor Prutyanov, 2019
*
*   Module which is synthesized as D-flip-flop with asynchronous reset
*/

module dff2(
    input clk,
    input d,
    input reset,

    output reg q
);

always @(posedge clk or negedge reset) begin
    if (!reset)
        q <= 0;
    else
        q <= d;
end

endmodule
