/* ----------------------------------------------- *
 * Title       : FIFO buffer                       *
 * Project     :                                   *
 * ----------------------------------------------- *
 * File        : FIFO_buffer.v                     *
 * Author      : Andrey Dryupin                    *
 * Last Edit   : 28/07/2022                        *
 * ----------------------------------------------- *
 * Description : 32 bytes FIFO buffer for UART     *
 * ----------------------------------------------- */

 module FIFO_buffer( Clk, 
                   dataIn, 
                   RD, 
                   WR, 
                   EN, 
                   dataOut, 
                   Rst,
                   EMPTY, 
                   FULL 
                   ); 

input  Clk, RD, WR, EN, Rst;
output  EMPTY, FULL;
input   [7:0]    dataIn;

output reg [7:0] dataOut; // internal registers 
reg [2:0]  Count = 0; 
reg [7:0] FIFO [0:31]; 
reg [2:0]  readCounter = 0, writeCounter = 0; 

assign EMPTY = (Count==0)? 1'b1:1'b0; 
assign FULL = (Count==32)? 1'b1:1'b0; 

always @ (posedge Clk) 
begin 

 if (EN==0); 

 else begin 

  if (Rst) begin 

   readCounter = 0; 

   writeCounter = 0; 

  end 

  else if (RD ==1'b1 && Count!=0) begin 

   dataOut  = FIFO[readCounter]; 

   readCounter = readCounter+1; 

  end 

  else if (WR==1'b1 && Count<32) begin
   FIFO[writeCounter]  = dataIn; 

   writeCounter  = writeCounter+1; 

  end 

  else; 

 end 

 if (writeCounter==8) 

  writeCounter=0; 

 else if (readCounter==8) 

  readCounter=0; 

 else;

 if (readCounter > writeCounter) begin 

  Count=readCounter-writeCounter; 

 end 

 else if (writeCounter > readCounter) 

  Count=writeCounter-readCounter; 

 else;

end 

endmodule