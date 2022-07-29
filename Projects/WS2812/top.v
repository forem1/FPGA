module top (
	input clk, rst,
	output led
);

	reg [1:0] stateLed;
	reg [24:0] counter;

	assign led = stateLed;

	always @ (posedge clk)
	begin
		counter<=counter+1;
		
		if(counter < 25'd1275 && counter > 25'd11379) stateLed <= 1'b1;
		if(counter==25'd1275) stateLed <= 1'b0; //Reset led
		
		//Green
		if(counter==25'd1675) stateLed <= 1'b1;
		if(counter==25'd1696) stateLed <= 1'b0;
		
		if(counter==25'd2096) stateLed <= 1'b1;
		if(counter==25'd2117) stateLed <= 1'b0;
		
		if(counter==25'd2517) stateLed <= 1'b1;
		if(counter==25'd2538) stateLed <= 1'b0;
		
		if(counter==25'd2938) stateLed <= 1'b1;
		if(counter==25'd2959) stateLed <= 1'b0;
		
		if(counter==25'd3359) stateLed <= 1'b1;
		if(counter==25'd3380) stateLed <= 1'b0;
		
		if(counter==25'd3780) stateLed <= 1'b1;
		if(counter==25'd3801) stateLed <= 1'b0;
		
		if(counter==25'd4201) stateLed <= 1'b1;
		if(counter==25'd4222) stateLed <= 1'b0;
		
		if(counter==25'd4622) stateLed <= 1'b1;
		if(counter==25'd4643) stateLed <= 1'b0;
		
		
		//Blue
		if(counter==25'd4664) stateLed <= 1'b1;
		if(counter==25'd5064) stateLed <= 1'b0;
		
		if(counter==25'd5085) stateLed <= 1'b1;
		if(counter==25'd5485) stateLed <= 1'b0;
		
		if(counter==25'd5506) stateLed <= 1'b1;
		if(counter==25'd5906) stateLed <= 1'b0;
		
		if(counter==25'd5927) stateLed <= 1'b1;
		if(counter==25'd6327) stateLed <= 1'b0;
		
		if(counter==25'd6348) stateLed <= 1'b1;
		if(counter==25'd6748) stateLed <= 1'b0;
		
		if(counter==25'd6769) stateLed <= 1'b1;
		if(counter==25'd7169) stateLed <= 1'b0;
		
		if(counter==25'd7190) stateLed <= 1'b1;
		if(counter==25'd7590) stateLed <= 1'b0;
		
		if(counter==25'd7611) stateLed <= 1'b1;
		if(counter==25'd8011) stateLed <= 1'b0;
		
		
		//Red
		if(counter==25'd8032) stateLed <= 1'b1;
		if(counter==25'd8432) stateLed <= 1'b0;
		
		if(counter==25'd8453) stateLed <= 1'b1;
		if(counter==25'd8853) stateLed <= 1'b0;
		
		if(counter==25'd8874) stateLed <= 1'b1;
		if(counter==25'd9274) stateLed <= 1'b0;
		
		if(counter==25'd9295) stateLed <= 1'b1;
		if(counter==25'd9695) stateLed <= 1'b0;
		
		if(counter==25'd9716) stateLed <= 1'b1;
		if(counter==25'd10116) stateLed <= 1'b0;
		
		if(counter==25'd10137) stateLed <= 1'b1;
		if(counter==25'd10537) stateLed <= 1'b0;
		
		if(counter==25'd10558) stateLed <= 1'b1;
		if(counter==25'd10958) stateLed <= 1'b0;
		
		if(counter==25'd10979) stateLed <= 1'b1;
		if(counter==25'd11379) stateLed <= 1'b0;
		
		
		//Reset
		if(counter==25'd11385)
		begin
			counter<=0;
		end
		
	end

endmodule