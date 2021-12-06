
//�Ѱ��µ����ּ��������������ʾ����

module IR(clk,rst_n,IR,led_cs,led_db);

  input   clk;
  input   rst_n;
  input   IR;
  output [3:0] led_cs;
  output [7:0] led_db;
 
  reg [3:0] led_cs;
  reg [7:0] led_db;
 
  reg [7:0] led1,led2,led3,led4;
  reg [15:0] irda_data;    // save irda data,than send to 7 segment led
  reg [31:0] get_data;     // use for saving 32 bytes irda data
  reg [5:0]  data_cnt;     // 32 bytes irda data counter
  reg [2:0]  cs,ns;
  reg error_flag;          // 32 bytes data�ڼ䣬���ݴ����־

  //----------------------------------------------------------------------------
  reg irda_reg0;       //Ϊ�˱�������̬,������������Ĵ�������һ����ʹ�á�
  reg irda_reg1;       //����ſ���ʹ�ã����³����д���irda��״̬
  reg irda_reg2;       //Ϊ��ȷ��irda�ı��أ��ٴ�һ�μĴ��������³����д���irda��ǰһ״̬
  wire irda_neg_pulse; //ȷ��irda���½���
  wire irda_pos_pulse; //ȷ��irda��������
  wire irda_chang;     //ȷ�irda��������
  
  reg[15:0] cnt_scan;//ɨ��Ƶ�ʼ�����
   
  always @ (posedge clk) //�ڴ˲��ø���Ĵ���
    if(!rst_n)
      begin
        irda_reg0 <= 1'b0;
        irda_reg1 <= 1'b0;
        irda_reg2 <= 1'b0;
      end
    else
      begin
        led_cs <= 4'b0000; //������ܵ�λѡ���ڵ�ͨ״̬
        irda_reg0 <= IR;
        irda_reg1 <= irda_reg0;
        irda_reg2 <= irda_reg1;
      end
     
  assign irda_chang = irda_neg_pulse | irda_pos_pulse;  //IR�����źŵĸı䣬���������½�
  assign irda_neg_pulse = irda_reg2 & (~irda_reg1);  //IR�����ź�irda�½���
  assign irda_pos_pulse = (~irda_reg2) & irda_reg1;      //IR�����ź�irda������


  reg [10:0] counter;  //��Ƶ1750��
  reg [8:0]  counter2; //������Ƶ��ĵ���
  wire check_9ms;  // check leader 9ms time
  wire check_4ms;  // check leader 4.5ms time
  wire low;        // check  data="0" time
  wire high;       // check  data="1" time
 
  //----------------------------------------------------------------------------
  //��Ƶ1750����
  always @ (posedge clk)
    if (!rst_n)
      counter <= 11'd0;
    else if (irda_chang)  //irda��ƽ�����ˣ������¿�ʼ����
      counter <= 11'd0;
    else if (counter == 11'd1750)
      counter <= 11'd0;
    else
      counter <= counter + 1'b1;
  
  //---------------------------------------------------------------------------- 
  always @ (posedge clk)
    if (!rst_n)
      counter2 <= 9'd0;
    else if (irda_chang)  //irda��ƽ�����ˣ������¿�ʼ�Ƶ�
      counter2 <= 9'd0;
    else if (counter == 11'd1750)
      counter2 <= counter2 +1'b1;
  

  assign check_9ms = ((217 < counter2) & (counter2 < 297)); 
  //257  Ϊ�������ȶ��ԣ�ȡһ����Χ
  assign check_4ms = ((88 < counter2) & (counter2 < 168));  //128
  assign low  = ((6 < counter2) & (counter2 < 26));         // 16
  assign high = ((38 < counter2) & (counter2 < 58));        // 48

  //----------------------------------------------------------------------------
  // generate statemachine  ״̬��
    parameter IDLE       = 3'b000, //��ʼ״̬
              LEADER_9   = 3'b001, //9ms
              LEADER_4   = 3'b010, //4ms
              DATA_STATE = 3'b100; //��������
 
  always @ (posedge clk)
    if (!rst_n)
      cs <= IDLE;
    else
      cs <= ns; //״̬λ
     
  always @ ( * )
    case (cs)
      IDLE:
        if (~irda_reg1)
          ns = LEADER_9;
        else
          ns = IDLE;
   
      LEADER_9:
        if (irda_pos_pulse)   //leader 9ms check
          begin
            if (check_9ms)
              ns = LEADER_4;
            else
              ns = IDLE;
          end
        else  //�걸��if---else--- ;��ֹ����latch
          ns =LEADER_9;
   
      LEADER_4:
        if (irda_neg_pulse)  // leader 4.5ms check
          begin
            if (check_4ms)
              ns = DATA_STATE;
            else
              ns = IDLE;
          end
        else
          ns = LEADER_4;
   
      DATA_STATE:
        if ((data_cnt == 6'd32) & irda_reg2 & irda_reg1)
          ns = IDLE;
        else if (error_flag)
          ns = IDLE;
        else
          ns = DATA_STATE;
      default:
        ns = IDLE;
    endcase

  //״̬���е����,��ʱ���·������
  always @ (posedge clk)
    if (!rst_n)
      begin
        data_cnt <= 6'd0;
        get_data <= 32'd0;
        error_flag <= 1'b0;
      end
  
    else if (cs == IDLE)
      begin
        data_cnt <= 6'd0;
        get_data <= 32'd0;
        error_flag <= 1'b0;
      end
  
    else if (cs == DATA_STATE)
      begin
        if (irda_pos_pulse)  // low 0.56ms check
          begin
            if (!low)  //error
              error_flag <= 1'b1;
          end
        else if (irda_neg_pulse)  //check 0.56ms/1.68ms data 0/1
          begin
            if (low)
              get_data[0] <= 1'b0;
            else if (high)
              get_data[0] <= 1'b1;
            else
              error_flag <= 1'b1;
             
            get_data[31:1] <= get_data[30:0];
            data_cnt <= data_cnt + 1'b1;
          end
      end

  always @ (posedge clk)
    if (!rst_n)
      irda_data <= 16'd0;
    else if ((data_cnt ==6'd32) & irda_reg1)
  begin
   led1 <= get_data[7:0];  //���ݷ���
   led2 <= get_data[15:8]; //������
   led3 <= get_data[23:16];//�û���
   led4 <= get_data[31:24];
  end
 
//��ң�����İ��µļ��������������ʾ����
always@(led2) 
begin
	case(led2)
	
	                     //�������������ʾ0��9
        8'b01101000: //ң�ذ�0����ֵ
			led_db=8'b1100_0000;  //��ʾ0

		8'b00110000: //ң�ذ�1����ֵ
			led_db=8'b1111_1001;  //��ʾ1

		8'b00011000: //ң�ذ�2����ֵ
			led_db=8'b1010_0100;  //��ʾ2

		8'b01111010: //ң�ذ�3����ֵ
			led_db=8'b1011_0000;  //��ʾ3

		8'b00010000: //ң�ذ�4����ֵ
			led_db=8'b1001_1001;  //��ʾ4

		8'b00111000: //ң�ذ�5����ֵ
			led_db=8'b1001_0010;  //��ʾ5

		8'b01011010: //ң�ذ�6����ֵ
			led_db=8'b1000_0010;  //��ʾ6

		8'b01000010: //ң�ذ�7����ֵ
			led_db=8'b1111_1000;  //��ʾ7

		8'b01001010: //ң�ذ�8����ֵ
			led_db=8'b1000_0000;  //��ʾ8

		8'b01010010: //ң�ذ�9����ֵ
			led_db=8'b1001_0000;  //��ʾ9
			
	  //��û�а��¼�ʱ����ʾF
	   default: led_db=8'b1000_1110;

	 endcase
end

endmodule 


