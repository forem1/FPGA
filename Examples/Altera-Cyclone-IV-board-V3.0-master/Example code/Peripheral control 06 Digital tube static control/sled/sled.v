/*******************************************************************
**���ǵ�FPGA������
**��վ��www.OurFPGA.com
**�Ա���OurFPGA.taobao.com
**����: OurFPGA@gmail.com
**��ӭ��ҵ�½��վ������FPGA�����Ӽ������ۣ����������Ƶ�̳̼�����
*****************�ļ���Ϣ********************************************
**�������ڣ�   2011.06.01
**�汾�ţ�     version 1.0
**����������    ��̬�������ʾʵ��
4���������ʾ��������ʾ��ʾ"0��f"
********************************************************************/




module sled(seg,dig,clock,);
input clock;
output [7:0] seg; //����ܶ������
output [3:0] dig; //�����λ�����
reg [7:0] seg; //����ܶ�������Ĵ���
reg [3:0] dig; //�����λ������Ĵ���
reg [3:0] disp_dat; //��ʾ���ݼĴ���
reg [36:0] count; //��������Ĵ���
always @ (posedge clock )
begin 
 count = count + 1'b1;
 dig= 8'b00000000;//
end
always @ (count[24])
begin
disp_dat = {count[28:25]};
end
always @ (disp_dat)
begin case (disp_dat)
4'h0 : seg = 8'hc0; //��ʾ"0"
4'h1 : seg = 8'hf9; //��ʾ"1"
4'h2 : seg = 8'ha4; //��ʾ"2"
4'h3 : seg = 8'hb0; //��ʾ"3"
4'h4 : seg = 8'h99; //��ʾ"4"
4'h5 : seg = 8'h92; //��ʾ"5"
4'h6 : seg = 8'h82; //��ʾ"6"
4'h7 : seg = 8'hf8; //��ʾ"7"
4'h8 : seg = 8'h80; //��ʾ"8"
4'h9 : seg = 8'h90; //��ʾ"9"
4'ha : seg = 8'h88; //��ʾ"a"
4'hb : seg = 8'h83; //��ʾ"b"
4'hc : seg = 8'hc6; //��ʾ"c"
4'hd : seg = 8'ha1; //��ʾ"d"
4'he : seg = 8'h86; //��ʾ"e"
4'hf : seg = 8'h8e; //��ʾ"f"
endcase
end
endmodule
