#ifndef __LCD_H
#define __LCD_H		
#include "sys\sys.h"
/* 
������Ի���:��Ƭ��STC12LE5A60S2,����33M  ��Ƭ��������ѹ3.3V
����Ĭ��IO���ӷ�ʽ��
�����ߣ�RS-P3^5;    WR-P3^6;   RD-P3^7;   CS-P1^0;   REST-P1^2;
������: DB0-DB7��������P0^0-P0^7;  DB8-DB15��������P2^0-P2^7;��8λģʽ��DB0-DB7���Բ����ӣ�
�����������ӷ�ʽ��(��ʹ�ô����ɲ�����)
T_CLK-P1^7;  T_CS-P1^4;  T_DIN-P3^0;  T_OUT-P3^1;  T_IRQ-P3^4;
*/	
 //����LCD�ĳߴ�	
#define LCD_W 240
#define LCD_H 320

//#define Bus_16        //16λ����ģʽ,���ʹ��8λģʽ����ע�ʹ���䣬���ʹ��16λģʽ����򿪴˾�,�޸�8λģʽ֮ǰ����ȷ���������ģ���Ƿ���8λ���߽ӿ�
//IO����
#define  LCD_DataPortH P2     //��8λ���ݿ�,8λģʽ��ֻʹ�ø�8λ 
#define  LCD_DataPortL P0     //��8λ���ݿ�	,8λģʽ�µ�8λ���Բ�����,��ȷ��P0���Ѿ�����10K����,����̫С����С4.7K,�Ƽ�10K.
sbit LCD_RS = P3^5;  		 //����/�����л�
sbit LCD_WR = P3^6;		  //д����
sbit LCD_RD =P3^7;		     //������
sbit LCD_CS=P1^0;		//Ƭѡ	
sbit LCD_REST = P1^2;	      //��λ   

extern  u16 BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ

void Lcd_Init(void); 
void LCD_Clear(u16 Color);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_WR_DATA8(char VH,char VL); //��������-8λ����
void LCD_WR_DATA(int da);
void LCD_WR_REG(int da);

void LCD_DrawPoint(u16 x,u16 y);//����
void LCD_DrawPoint_big(u16 x,u16 y);//��һ�����
u16  LCD_ReadPoint(u16 x,u16 y); //����
void Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode);//��ʾһ���ַ�
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len);//��ʾ����
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len);//��ʾ2������
void LCD_ShowString(u16 x,u16 y,const u8 *p);		 //��ʾһ���ַ���,16����
 
void showhanzi(unsigned int x,unsigned int y,unsigned char index);



//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


					  		 
#endif  
	 
	 



