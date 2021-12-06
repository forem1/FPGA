
/*FPGAͨ��ps2���ռ������ݣ�Ȼ��ѽ��յ�����ĸA��Z��ֵת����Ӧ��ASII�룬ͨ�����ڷ��͵�PC���ϡ�
ʵ��ʱ����Ҫ�Ӽ��̣���Ҫ�õ������֣����س�����ڼ����ϰ���һ����������A������PC���������Ͽɿ���A
*/


`timescale 1ns / 1ps

module ps2_key(clk,rst_n,ps2k_clk,ps2k_data,rs232_tx);

input clk;			//50Mʱ���ź�
input rst_n;		//��λ�ź�
input ps2k_clk;		//PS2�ӿ�ʱ���ź�
input ps2k_data;	//PS2�ӿ������ź�
output rs232_tx;	// RS232���������ź�


wire[7:0] ps2_byte;	// 1byte��ֵ
wire ps2_state;		//����״̬��־λ

wire bps_start;		//���յ����ݺ󣬲�����ʱ�������ź���λ
wire clk_bps;		// clk_bps�ĸߵ�ƽΪ���ջ��߷�������λ���м������ 

ps2scan			ps2scan(	.clk(clk),			  	//����ɨ��ģ��
								.rst_n(rst_n),				
								.ps2k_clk(ps2k_clk),
								.ps2k_data(ps2k_data),
								.ps2_byte(ps2_byte),
								.ps2_state(ps2_state)
								);

speed_select	speed_select(	.clk(clk),
										.rst_n(rst_n),
										.bps_start(bps_start),
										.clk_bps(clk_bps)
										);

my_uart_tx		my_uart_tx(		.clk(clk),
										.rst_n(rst_n),
										.clk_bps(clk_bps),
										.rx_data(ps2_byte),
										.rx_int(ps2_state),
										.rs232_tx(rs232_tx),
										.bps_start(bps_start)
										);

endmodule
