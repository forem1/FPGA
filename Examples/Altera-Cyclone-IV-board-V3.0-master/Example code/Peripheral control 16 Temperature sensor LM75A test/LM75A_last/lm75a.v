module LM75A_test(clk,rst_n,scl,sda,cs,seg);
input clk,rst_n; //fu��λ�źŵ͵�ƽ��Ч��ʱ��50MHz;
output scl;  //I2Cʱ�ӣ�250KHz��ʱ������Ϊ4us;
inout sda;   //I2C���ݶˣ�
output[3:0] cs;  //�����Ƭѡ��
output[7:0] seg;  //����ܶ�ѡ��
wire done;    // I2C��ȡһ��������ϣ���������ܶ�ѡ�Ĵ�����
wire[15:0] data;//�¶����ݣ�
I2C_READ I2C_READ(
        .clk(clk),
 .rst_n(rst_n),
 .scl(scl),
 .sda(sda),
 .data(data)
              );
SEG_D  SEG_D(
       .clk(clk),
.rst_n(rst_n),
.cs(cs),
.seg(seg),
.data(data)
         );
endmodule