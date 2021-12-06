
module uart_txd(clk,rst,data_bus,host_ready,load_tram_datareg,serial_out);
input clk;                                          //ʱ���ź�
input rst;                                          //��λ
input[7:0] data_bus;                                //�����źţ����������������ݣ�8bit
input host_ready;                                   //�����źţ�Ϊ�ߵ�ƽ��ʾ��������׼�����
input load_tram_datareg;                            //�����źţ�Ϊ�ߵ�ƽ��ʾ�������ݼĴ�����������������ȡ����
output serial_out;                                  //�����źţ�UART���͵������ź�


reg[7:0] tram_datareg;                              //UART�������ݼĴ���
reg[8:0] tram_shiftreg;                             //UART����������λ�Ĵ���
reg load_tram_shiftreg;                             //װ����λ�Ĵ�����־λ
reg[1:0] state;
reg[1:0] next_state;
reg[3:0] count;
reg clear;                                          //��count����������
reg shift;                                          //�Ĵ�����λ�ź�
reg start;                                          //��ʼ���������ź�

//����״̬�����С��ȴ�������״̬
parameter[1:0] idle=2'b00;
parameter[1:0] waiting=2'b01;
parameter[1:0] sending=2'b10;

assign serial_out=tram_shiftreg[0];                 //��λ�Ĵ������λ���

always@(posedge clk)
begin
  if(!rst)
    next_state<=idle;
  
  state=next_state;
  case(state)
  idle:begin                                        //����
       clear=0;
       shift=0;
       start=0;
       if(host_ready)
       begin
         load_tram_shiftreg<=1;
         next_state<=waiting;
       end
       end
  waiting:begin                                     //�ȴ�
          start<=1;
          next_state<=sending;
          end
  sending:begin                                     //����
          if(count!=9) 
            shift<=1;
          else
          begin
            clear<=1;
            next_state<=idle;
          end
          end
  default:next_state<=idle;
  endcase
end

always@(posedge clk)
begin
  if(!rst)
  begin
    tram_shiftreg<=9'b1_1111_1111;                       //��λ�Ĵ������ݸ�λ
    count<=0;
  end
  else
  begin
    if(load_tram_datareg)
      tram_datareg<=data_bus;                            //ȡ��������
    if(load_tram_shiftreg)
      tram_shiftreg<={tram_datareg,1'b1};
    if(start)
      tram_shiftreg[0]<=0;                               //��ʼ�����źţ���ʼλΪ0
    if(clear)
      count<=0;                                          //���������
    else if(shift)
      count<=count+1;
    if(shift)
      tram_shiftreg<={1'b1,tram_shiftreg[8:1]};
  end
end
endmodule    
  
        