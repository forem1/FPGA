/* ----------------------------------------------- *
 * Title       : UART transmitter                  *
 * Project     :                                   *
 * ----------------------------------------------- *
 * File        : uart_tx.v                         *
 * Author      : Andrey Dryupin                    *
 * Last Edit   : 28/07/2022                        *
 * ----------------------------------------------- *
 * Description : Simple UART transmitter           *
 * ----------------------------------------------- */

module uart_tx (
    input clk,
    input start,
    input [7:0]data,
	 input [2:0]bps,
    output reg q = 1'b1
);

reg [12:0]cnt = 13'b0; // UART speed counter
reg [17:0]bps_cnt; // UART speed selector
reg [3:0]bit_num = 4'b1111;
wire bit_start = (cnt == bps_cnt); 
wire idle = (bit_num == 4'hF);

always @(posedge clk) begin
    if (start && idle) begin
		case(bps)
			3'b000:  bps_cnt <= 18'd5208; //9600 // 5208 = 50mhz/9600
			3'b001:  bps_cnt <= 18'd2603; //19200
			3'b010:  bps_cnt <= 18'd1301; //38400
			3'b011:  bps_cnt <= 18'd867;  //57600
			3'b100:  bps_cnt <= 18'd433;  //115200
		endcase
      cnt <= 13'b0;
	 end
    else if (bit_start)
        cnt <= 13'b0;
    else
        cnt <= cnt + 13'b1;
end

always @(posedge clk) begin
    if (start && idle) begin
        bit_num <= 4'h0;
        q <= 1'b0; // Start
    end
    else if (bit_start) begin
        case (bit_num)
        4'h0: begin bit_num <= 4'h1; q <= data[0]; end
        4'h1: begin bit_num <= 4'h2; q <= data[1]; end
        4'h2: begin bit_num <= 4'h3; q <= data[2]; end
        4'h3: begin bit_num <= 4'h4; q <= data[3]; end
        4'h4: begin bit_num <= 4'h5; q <= data[4]; end
        4'h5: begin bit_num <= 4'h6; q <= data[5]; end
        4'h6: begin bit_num <= 4'h7; q <= data[6]; end
        4'h7: begin bit_num <= 4'h8; q <= data[7]; end
        4'h8: begin bit_num <= 4'h9; q <= 1'b1; end // Stop
        default: begin bit_num <= 4'hF; end
        endcase
    end
end

endmodule
