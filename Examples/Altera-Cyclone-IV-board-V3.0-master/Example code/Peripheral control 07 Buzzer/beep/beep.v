/*******************************************************************
**���ǵ�FPGA������
**��վ��www.OurFPGA.com
**�Ա���OurFPGA.taobao.com
**����: OurFPGA@gmail.com
**��ӭ��ҵ�½��վ������FPGA�����Ӽ������ۣ����������Ƶ�̳̼�����
*****************�ļ���Ϣ********************************************
**�������ڣ�   2011.06.01
**�汾�ţ�     version 1.0
**����������   ���������������ʵ�� �Ρ����Ρ����Ρ���
********************************************************************/


module	beep(clk,beep);					//ģ������beep		
input	clk;							//ϵͳʱ��	
output	beep;							//�����������				
reg beep_r;								//�Ĵ���
reg[27:0]count;

assign beep = beep_r;					//�������

always@(posedge clk)
begin
	count <= count + 1'b1;
end

always @(count[9])
begin
	beep_r = !(count[13]&count[24]&count[27]);
end

endmodule