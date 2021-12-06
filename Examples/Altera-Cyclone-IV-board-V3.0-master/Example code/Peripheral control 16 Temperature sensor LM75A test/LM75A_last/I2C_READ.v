module I2C_READ(
       clk,
rst_n,
scl,sda,data
              );
 
 input clk;//����ʱ�� 50MHz  
input rst_n;//�첽��λ,�͵�ƽ��Ч  
  
output scl;//SCL ʱ��  
inout  sda;// SDA ��������  
output [15:0] data;//�¶�����  
  
reg [15:0]data_r;//�¶����ݼĴ���  
reg scl;//SCL ���߼Ĵ���  
reg sda_r;//SDA ���߼Ĵ���  
reg sda_link;//SDA �������ݷ����־  
reg [7:0]scl_cnt;//SCL ʱ�Ӳ���������  
reg [2:0]cnt;//�������SCLʱ�Ӽ�����  
reg [25:0]timer_cnt;//��ʱ��,ÿ��2s ��ȡһ���¶�����  
reg [3:0]data_cnt;//���ݴ���ת���Ĵ���  
reg [7:0]address_reg;//������ַ�Ĵ���  
reg [8:0]state;//״̬�Ĵ���  
//////////////////////////////////////////////////////////////////////////////////  
//����1��2��3������SCL ����ʱ��  
always@(posedge clk or negedge rst_n)  
    begin  
        if(!rst_n)  
            scl_cnt <= 8'd0;  
        else if(scl_cnt == 8'd199)  
            scl_cnt <= 8'd0;  
        else  
            scl_cnt <= scl_cnt + 1'b1;  
    end  
always@(posedge clk or negedge rst_n)  
    begin  
        if(!rst_n)  
            cnt <= 3'd5;  
        else   
            case(scl_cnt)  
                8'd49: cnt <= 3'd1;//�ߵ�ƽ�м�  
                8'd99: cnt <= 3'd2;//�½���  
                8'd149:cnt <= 3'd3;//�͵�ƽ�м�  
                8'd199:cnt <= 3'd0;//������  
               default: cnt <= 3'd5;  
            endcase  
    end  
`define SCL_HIG (cnt == 3'd1)  
`define SCL_NEG (cnt == 3'd2)  
`define SCL_LOW (cnt == 3'd3)  
`define SCL_POS (cnt == 3'd0)  
always@(posedge clk or negedge rst_n)  
    begin  
        if(!rst_n)  
            scl <= 1'b0;  
        else if(`SCL_POS)  
            scl <= 1'b1;  
        else if(`SCL_NEG)  
            scl <= 1'b0;  
    end  
//////////////////////////////////////////////////////////////////////////////////  
//����4����ʱ��,ÿ��1s ��ȡһ���¶�����  
always@(posedge clk or negedge rst_n)  
    begin  
        if(!rst_n)  
            timer_cnt <= 26'd0;  
        else if(timer_cnt == 26'd49999999)  
            timer_cnt <= 26'd0;  
        else   
            timer_cnt <= timer_cnt + 1'b1;  
    end  
//////////////////////////////////////////////////////////////////////////////////  
//״̬������  
parameter IDLE  = 9'b0_0000_0000,  
             START  = 9'b0_0000_0010,  
             ADDRESS    = 9'b0_0000_0100,  
             ACK1       = 9'b0_0000_1000,  
             READ1  = 9'b0_0001_0000,  
             ACK2       = 9'b0_0010_0000,  
             READ2  = 9'b0_0100_0000,  
             NACK       = 9'b0_1000_0000,  
             STOP       = 9'b1_0000_0000;  
`define DEVICE_ADDRESS 8'b1001_0001//������ַ,������  
//////////////////////////////////////////////////////////////////////////////////  
//����5��״̬������  
always@(posedge clk or negedge rst_n)  
    begin  
        if(!rst_n)  
            begin  
                data_r  <= 16'd0;  
                sda_r       <= 1'b1;  
                sda_link    <= 1'b1;  
                state       <= IDLE;  
                address_reg <= 15'd0;  
                data_cnt    <= 4'd0;  
            end  
        else   
            case(state)  
                IDLE:  
                    begin  
                        sda_r   <= 1'b1;  
                        sda_link <= 1'b1;  
                        if(timer_cnt == 26'd49999999)  
                            state <= START;  
                        else  
                            state <= IDLE;  
                    end  
                START://������ʼ�ź�  
                    begin  
                        if(`SCL_HIG)  
                            begin  
                                sda_r       <= 1'b0;  
                                sda_link    <= 1'b1;  
                                address_reg <= `DEVICE_ADDRESS;  
                                state           <= ADDRESS;  
                                data_cnt        <= 4'd0;  
                            end  
                        else  
                            state <= START;  
                    end  
                ADDRESS://��������������Ѱַ  
                    begin  
                        if(`SCL_LOW)  
                            begin  
                                if(data_cnt == 4'd8)//Ѱַ���,SDA�ı䷽��,����׼�����Ӧ��Ѷ��  
                                    begin  
                                        state   <= ACK1;  
                                        data_cnt <=  4'd0;  
                                        sda_r       <= 1'b1;  
                                        sda_link    <= 1'b0;  
                                    end  
                                else//Ѱַ������,SDA��������Ϊ����  
                                    begin  
                                        state   <= ADDRESS;  
                                        data_cnt <= data_cnt + 1'b1;  
                                        case(data_cnt)  
                                            4'd0: sda_r <= address_reg[7];  
                                            4'd0: sda_r <= address_reg[7];  
                                            4'd1: sda_r <= address_reg[6];  
                                            4'd2: sda_r <= address_reg[5];  
                                            4'd3: sda_r <= address_reg[4];  
                                            4'd4: sda_r <= address_reg[3];  
                                            4'd5: sda_r <= address_reg[2];  
                                            4'd6: sda_r <= address_reg[1];  
                                            4'd7: sda_r <= address_reg[0];  
                                            default: ;  
                                        endcase  
                                    end  
                            end  
                        else  
                            state <= ADDRESS;  
                    end  
                ACK1://�������Ӧ���ź�  
                    begin  
                        if(!sda && (`SCL_HIG))  
                            state <= READ1;  
                        else if(`SCL_NEG)  
                            state <= READ1;  
                        else  
                            state <= ACK1;  
                    end  
                READ1://����������,���ֽ�  
                    begin  
                        if((`SCL_LOW) && (data_cnt == 4'd8))//�����ֽ��������,SDA�ı䷽��,����׼�����Ӧ��Ѷ��  
                            begin  
                                state   <= ACK2;  
                                data_cnt <= 4'd0;  
                                sda_r       <= 1'b1;  
                                sda_link    <= 1'b1;  
                            end  
                        else if(`SCL_HIG)//�����ݹ�����,������Ϊ���  
                            begin  
                                data_cnt <= data_cnt + 1'b1;  
                                case(data_cnt)  
                                    4'd0: data_r[15] <= sda;  
                                    4'd1: data_r[14] <= sda;  
                                    4'd2: data_r[13] <= sda;  
                                    4'd3: data_r[12] <= sda;  
                                    4'd4: data_r[11] <= sda;  
                                    4'd5: data_r[10] <= sda;  
                                    4'd6: data_r[9]  <= sda;  
                                    4'd7: data_r[8]  <= sda;  
                                    default: ;  
                                endcase  
                            end  
                        else  
                            state <= READ1;  
                    end  
                ACK2://�������Ӧ��Ѷ��  
                    begin     
                        if(`SCL_LOW)  
                            sda_r <= 1'b0;  
                        else if(`SCL_NEG)  
                            begin  
                                sda_r   <= 1'b1;  
                                sda_link    <= 1'b0;  
                                state       <= READ2;  
                            end  
                        else  
                            state <= ACK2;  
                    end  
                READ2://�����ֽ�����  
                    begin  
                        if((`SCL_LOW) && (data_cnt == 4'd8))  
                            begin  
                                state   <= NACK;  
                                data_cnt <= 4'd0;  
                                sda_r       <= 1'b1;  
                                sda_link    <= 1'b1;  
                            end  
                        else if(`SCL_HIG)  
                            begin  
                                data_cnt <= data_cnt + 1'b1;  
                                case(data_cnt)  
                                    4'd0: data_r[7] <= sda;  
                                    4'd1: data_r[6] <= sda;  
                                    4'd2: data_r[5] <= sda;  
                                    4'd3: data_r[4] <= sda;  
                                    4'd4: data_r[3] <= sda;  
                                    4'd5: data_r[2] <= sda;  
                                    4'd6: data_r[1]  <= sda;  
                                    4'd7: data_r[0]  <= sda;  
                                    default: ;  
                                endcase  
                            end  
                        else  
                            state <= READ2;  
                    end  
                NACK://������Ӧ��  
                    begin  
                        if(`SCL_LOW)  
                            begin  
                                state <= STOP;  
                                sda_r   <= 1'b0;  
                            end  
                        else  
                            state <= NACK;  
                    end  
                STOP:  
                    begin  
                        if(`SCL_HIG)  
                            begin  
                                state <= IDLE;  
                                sda_r <= 1'b1;  
                            end  
                        else  
                            state <= STOP;  
                    end  
                default: state <= IDLE;  
            endcase  
    end  
assign sda   = sda_link ? sda_r: 1'bz;  
assign data  = data_r;  
endmodule
