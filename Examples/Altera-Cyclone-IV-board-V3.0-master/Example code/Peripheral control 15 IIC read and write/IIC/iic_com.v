

module iic_com(
			clk,rst_n,
			sw1,sw2,
			scl,sda,
			dis_data
		);

input clk;		// 50MHz
input rst_n;	//��λ�źţ�����Ч
input sw1,sw2;	//����1��2,(1����ִ��д�������2����ִ�ж�����)
output scl;		// 24C02��ʱ�Ӷ˿�
inout sda;		// 24C02�����ݶ˿�
output[7:0] dis_data;	//�������ʾ������


		//�������
reg sw1_r,sw2_r;	//��ֵ����Ĵ�����ÿ20ms���һ�μ�ֵ 
reg[19:0] cnt_20ms;	//20ms�����Ĵ���

always @ (posedge clk or negedge rst_n)
	if(!rst_n) cnt_20ms <= 20'd0;
	else cnt_20ms <= cnt_20ms+1'b1;	//���ϼ���

always @ (posedge clk or negedge rst_n)
	if(!rst_n) begin
			sw1_r <= 1'b1;	//��ֵ�Ĵ�����λ��û�м��̰���ʱ��ֵ��Ϊ1
			sw2_r <= 1'b1;
		end
	else if(cnt_20ms == 20'hfffff) begin
			sw1_r <= sw1;	//����1ֵ����
			sw2_r <= sw2;	//����2ֵ����
		end

//---------------------------------------------
		//��Ƶ����
reg[2:0] cnt;	// cnt=0:scl�����أ�cnt=1:scl�ߵ�ƽ�м䣬cnt=2:scl�½��أ�cnt=3:scl�͵�ƽ�м�
reg[8:0] cnt_delay;	//500ѭ������������iic����Ҫ��ʱ��
reg scl_r;		//ʱ������Ĵ���

always @ (posedge clk or negedge rst_n)
	if(!rst_n) cnt_delay <= 9'd0;
	else if(cnt_delay == 9'd499) cnt_delay <= 9'd0;	//������10usΪscl�����ڣ���100KHz
	else cnt_delay <= cnt_delay+1'b1;	//ʱ�Ӽ���

always @ (posedge clk or negedge rst_n) begin
	if(!rst_n) cnt <= 3'd5;
	else begin
		case (cnt_delay)
			9'd124:	cnt <= 3'd1;	//cnt=1:scl�ߵ�ƽ�м�,�������ݲ���
			9'd249:	cnt <= 3'd2;	//cnt=2:scl�½���
			9'd374:	cnt <= 3'd3;	//cnt=3:scl�͵�ƽ�м�,�������ݱ仯
			9'd499:	cnt <= 3'd0;	//cnt=0:scl������
			default: cnt <= 3'd5;
			endcase
		end
end


`define SCL_POS		(cnt==3'd0)		//cnt=0:scl������
`define SCL_HIG		(cnt==3'd1)		//cnt=1:scl�ߵ�ƽ�м�,�������ݲ���
`define SCL_NEG		(cnt==3'd2)		//cnt=2:scl�½���
`define SCL_LOW		(cnt==3'd3)		//cnt=3:scl�͵�ƽ�м�,�������ݱ仯


always @ (posedge clk or negedge rst_n)
	if(!rst_n) scl_r <= 1'b0;
	else if(cnt==3'd0) scl_r <= 1'b1;	//scl�ź�������
   	else if(cnt==3'd2) scl_r <= 1'b0;	//scl�ź��½���

assign scl = scl_r;	//����iic����Ҫ��ʱ��
//---------------------------------------------
		//��Ҫд��24C02�ĵ�ַ������
				
`define	DEVICE_READ	8'b1010_0001	//��Ѱַ������ַ����������
`define DEVICE_WRITE	8'b1010_0000	//��Ѱַ������ַ��д������
`define	WRITE_DATA	8'b1111_0001	//д��EEPROM������
`define BYTE_ADDR		8'b0000_0011	//д��/����EEPROM�ĵ�ַ�Ĵ���	
reg[7:0] db_r;		//��IIC�ϴ��͵����ݼĴ���
reg[7:0] read_data;	//����EEPROM�����ݼĴ���

//---------------------------------------------
		//����дʱ��
parameter 	IDLE 	= 4'd0;
parameter 	START1 	= 4'd1;
parameter 	ADD1 	= 4'd2;
parameter 	ACK1 	= 4'd3;
parameter 	ADD2 	= 4'd4;
parameter 	ACK2 	= 4'd5;
parameter 	START2 	= 4'd6;
parameter 	ADD3 	= 4'd7;
parameter 	ACK3	= 4'd8;
parameter 	DATA 	= 4'd9;
parameter 	ACK4	= 4'd10;
parameter 	STOP1 	= 4'd11;
parameter 	STOP2 	= 4'd12;

reg[3:0] cstate;	//״̬�Ĵ���
reg sda_r;		//������ݼĴ���
reg sda_link;	//�������sda�ź�inout�������λ		
reg[3:0] num;	//


always @ (posedge clk or negedge rst_n) begin
	if(!rst_n) begin
			cstate <= IDLE;
			sda_r <= 1'b1;
			sda_link <= 1'b0;
			num <= 4'd0;
			read_data <= 8'b0000_0000;
		end
	else 	  
		case (cstate)
			IDLE:	begin
					sda_link <= 1'b1;			//������sdaΪoutput
					sda_r <= 1'b1;
					if(!sw1_r || !sw2_r) begin	//SW1,SW2����һ��������			
						db_r <= `DEVICE_WRITE;	//��������ַ��д������
						cstate <= START1;		
						end
					else cstate <= IDLE;	//û���κμ�������
				end
			START1: begin
					if(`SCL_HIG) begin		//sclΪ�ߵ�ƽ�ڼ�
						sda_link <= 1'b1;	//������sdaΪoutput
						sda_r <= 1'b0;		//����������sda��������ʼλ�ź�
						cstate <= ADD1;
						num <= 4'd0;		//num��������
						end
					else cstate <= START1; //�ȴ�scl�ߵ�ƽ�м�λ�õ���
				end
			ADD1:	begin
					if(`SCL_LOW) begin
							if(num == 4'd8) begin	
									num <= 4'd0;			//num��������
									sda_r <= 1'b1;
									sda_link <= 1'b0;		//sda��Ϊ����̬(input)
									cstate <= ACK1;
								end
							else begin
									cstate <= ADD1;
									num <= num+1'b1;
									case (num)
										4'd0: sda_r <= db_r[7];
										4'd1: sda_r <= db_r[6];
										4'd2: sda_r <= db_r[5];
										4'd3: sda_r <= db_r[4];
										4'd4: sda_r <= db_r[3];
										4'd5: sda_r <= db_r[2];
										4'd6: sda_r <= db_r[1];
										4'd7: sda_r <= db_r[0];
										default: ;
										endcase
							//		sda_r <= db_r[4'd7-num];	//��������ַ���Ӹ�λ��ʼ
								end
						end
			//		else if(`SCL_POS) db_r <= {db_r[6:0],1'b0};	//������ַ����1bit
					else cstate <= ADD1;
				end
			ACK1:	begin
					if(/*!sda*/`SCL_NEG) begin	//ע��24C01/02/04/08/16�������Բ�����Ӧ��λ
							cstate <= ADD2;	//�ӻ���Ӧ�ź�
							db_r <= `BYTE_ADDR;	// 1��ַ		
						end
					else cstate <= ACK1;		//�ȴ��ӻ���Ӧ
				end
			ADD2:	begin
					if(`SCL_LOW) begin
							if(num==4'd8) begin	
									num <= 4'd0;			//num��������
									sda_r <= 1'b1;
									sda_link <= 1'b0;		//sda��Ϊ����̬(input)
									cstate <= ACK2;
								end
							else begin
									sda_link <= 1'b1;		//sda��Ϊoutput
									num <= num+1'b1;
									case (num)
										4'd0: sda_r <= db_r[7];
										4'd1: sda_r <= db_r[6];
										4'd2: sda_r <= db_r[5];
										4'd3: sda_r <= db_r[4];
										4'd4: sda_r <= db_r[3];
										4'd5: sda_r <= db_r[2];
										4'd6: sda_r <= db_r[1];
										4'd7: sda_r <= db_r[0];
										default: ;
										endcase
							//		sda_r <= db_r[4'd7-num];	//��EEPROM��ַ����bit��ʼ��		
									cstate <= ADD2;					
								end
						end
			//		else if(`SCL_POS) db_r <= {db_r[6:0],1'b0};	//������ַ����1bit
					else cstate <= ADD2;				
				end
			ACK2:	begin
					if(/*!sda*/`SCL_NEG) begin		//�ӻ���Ӧ�ź�
						if(!sw1_r) begin
								cstate <= DATA; 	//д����
								db_r <= `WRITE_DATA;	//д�������							
							end	
						else if(!sw2_r) begin
								db_r <= `DEVICE_READ;	//��������ַ�������������ض���ַ����Ҫִ�иò������²���
								cstate <= START2;		//������
							end
						end
					else cstate <= ACK2;	//�ȴ��ӻ���Ӧ
				end
			START2: begin	//��������ʼλ
					if(`SCL_LOW) begin
						sda_link <= 1'b1;	//sda��Ϊoutput
						sda_r <= 1'b1;		//����������sda
						cstate <= START2;
						end
					else if(`SCL_HIG) begin	//sclΪ�ߵ�ƽ�м�
						sda_r <= 1'b0;		//����������sda��������ʼλ�ź�
						cstate <= ADD3;
						end	 
					else cstate <= START2;
				end
			ADD3:	begin	//�Ͷ�������ַ
					if(`SCL_LOW) begin
							if(num==4'd8) begin	
									num <= 4'd0;			//num��������
									sda_r <= 1'b1;
									sda_link <= 1'b0;		//sda��Ϊ����̬(input)
									cstate <= ACK3;
								end
							else begin
									num <= num+1'b1;
									case (num)
										4'd0: sda_r <= db_r[7];
										4'd1: sda_r <= db_r[6];
										4'd2: sda_r <= db_r[5];
										4'd3: sda_r <= db_r[4];
										4'd4: sda_r <= db_r[3];
										4'd5: sda_r <= db_r[2];
										4'd6: sda_r <= db_r[1];
										4'd7: sda_r <= db_r[0];
										default: ;
										endcase									
								//	sda_r <= db_r[4'd7-num];	//��EEPROM��ַ����bit��ʼ��		
									cstate <= ADD3;					
								end
						end
				//	else if(`SCL_POS) db_r <= {db_r[6:0],1'b0};	//������ַ����1bit
					else cstate <= ADD3;				
				end
			ACK3:	begin
					if(/*!sda*/`SCL_NEG) begin
							cstate <= DATA;	//�ӻ���Ӧ�ź�
							sda_link <= 1'b0;
						end
					else cstate <= ACK3; 		//�ȴ��ӻ���Ӧ
				end
			DATA:	begin
					if(!sw2_r) begin	 //������
							if(num<=4'd7) begin
								cstate <= DATA;
								if(`SCL_HIG) begin	
									num <= num+1'b1;	
									case (num)
										4'd0: read_data[7] <= sda;
										4'd1: read_data[6] <= sda;  
										4'd2: read_data[5] <= sda; 
										4'd3: read_data[4] <= sda; 
										4'd4: read_data[3] <= sda; 
										4'd5: read_data[2] <= sda; 
										4'd6: read_data[1] <= sda; 
										4'd7: read_data[0] <= sda; 
										default: ;
										endcase																		
					//				read_data[4'd7-num] <= sda;	//�����ݣ���bit��ʼ��
									end
				//				else if(`SCL_NEG) read_data <= {read_data[6:0],read_data[7]};	//����ѭ������
								end
							else if((`SCL_LOW) && (num==4'd8)) begin
								num <= 4'd0;			//num��������
								cstate <= ACK4;
								end
							else cstate <= DATA;
						end
					else if(!sw1_r) begin	//д����
							sda_link <= 1'b1;	
							if(num<=4'd7) begin
								cstate <= DATA;
								if(`SCL_LOW) begin
									sda_link <= 1'b1;		//������sda��Ϊoutput
									num <= num+1'b1;
									case (num)
										4'd0: sda_r <= db_r[7];
										4'd1: sda_r <= db_r[6];
										4'd2: sda_r <= db_r[5];
										4'd3: sda_r <= db_r[4];
										4'd4: sda_r <= db_r[3];
										4'd5: sda_r <= db_r[2];
										4'd6: sda_r <= db_r[1];
										4'd7: sda_r <= db_r[0];
										default: ;
										endcase									
								//	sda_r <= db_r[4'd7-num];	//д�����ݣ���bit��ʼ��
									end
			//					else if(`SCL_POS) db_r <= {db_r[6:0],1'b0};	//д����������1bit
							 	end
							else if((`SCL_LOW) && (num==4'd8)) begin
									num <= 4'd0;
									sda_r <= 1'b1;
									sda_link <= 1'b0;		//sda��Ϊ����̬
									cstate <= ACK4;
								end
							else cstate <= DATA;
						end
				end
			ACK4: begin
					if(/*!sda*/`SCL_NEG) begin
//						sda_r <= 1'b1;
						cstate <= STOP1;						
						end
					else cstate <= ACK4;
				end
			STOP1:	begin
					if(`SCL_LOW) begin
							sda_link <= 1'b1;
							sda_r <= 1'b0;
							cstate <= STOP1;
						end
					else if(`SCL_HIG) begin
							sda_r <= 1'b1;	//sclΪ��ʱ��sda���������أ������źţ�
							cstate <= STOP2;
						end
					else cstate <= STOP1;
				end
			STOP2:	begin
					if(`SCL_LOW) sda_r <= 1'b1;
					else if(cnt_20ms==20'hffff0) cstate <= IDLE;
					else cstate <= STOP2;
				end
			default: cstate <= IDLE;
			endcase
end

assign sda = sda_link ? sda_r:1'bz;
assign dis_data = read_data;



endmodule


