

/*
PC���Ͽ����ڵ�������.
����һ���ַ�(������9600������λ8λ��ֹͣλ1λ)
�������壨�м�ͨ��������������
FPGA�յ��ַ��󣬻ط���PC���ϣ��ڴ�����������ʾ
*/



`timescale 1ns / 1ps

module my_uart_top(
				clk,rst_n,
				rs232_rx,rs232_tx
				);

input clk;			// 50MHz��ʱ��
input rst_n;		//�͵�ƽ��λ�ź�

input rs232_rx;		// RS232���������ź�
output rs232_tx;	//	RS232���������ź�

wire bps_start1,bps_start2;	//���յ����ݺ󣬲�����ʱ�������ź���λ
wire clk_bps1,clk_bps2;		// clk_bps_r�ߵ�ƽΪ��������λ���м������,ͬʱҲ��Ϊ�������ݵ����ݸı�� 
wire[7:0] rx_data;	//�������ݼĴ���������ֱ����һ����������
wire rx_int;		//���������ж��ź�,���յ������ڼ�ʼ��Ϊ�ߵ�ƽ
//----------------------------------------------------
//������ĸ�ģ���У�speed_rx��speed_tx��������ȫ������Ӳ��ģ�飬�ɳ�֮Ϊ�߼�����
//��������Դ����������е�ͬһ���ӳ�����ò��ܻ�Ϊһ̸��
////////////////////////////////////////////
speed_select		speed_rx(	
							.clk(clk),	//������ѡ��ģ��
							.rst_n(rst_n),
							.bps_start(bps_start1),
							.clk_bps(clk_bps1)
						);

my_uart_rx			my_uart_rx(		
							.clk(clk),	//��������ģ��
							.rst_n(rst_n),
							.rs232_rx(rs232_rx),
							.rx_data(rx_data),
							.rx_int(rx_int),
							.clk_bps(clk_bps1),
							.bps_start(bps_start1)
						);

///////////////////////////////////////////						
speed_select		speed_tx(	
							.clk(clk),	//������ѡ��ģ��
							.rst_n(rst_n),
							.bps_start(bps_start2),
							.clk_bps(clk_bps2)
						);

my_uart_tx			my_uart_tx(		
							.clk(clk),	//��������ģ��
							.rst_n(rst_n),
							.rx_data(rx_data),
							.rx_int(rx_int),
							.rs232_tx(rs232_tx),
							.clk_bps(clk_bps2),
							.bps_start(bps_start2)
						);

endmodule
