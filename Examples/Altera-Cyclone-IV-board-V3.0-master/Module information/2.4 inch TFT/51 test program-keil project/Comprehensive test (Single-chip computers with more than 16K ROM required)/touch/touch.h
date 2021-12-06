#ifndef __TOUCH_H
#define __TOUCH_H	
#include "sys\sys.h"	
/* touch panel interface define */
#define  CMD_RDX  0xD0	 //����IC�����������
#define  CMD_RDY  0x90
//IO����
sbit DCLK	   =    P1^7;   
sbit CS        =    P1^4;
sbit DIN       =    P3^0;	
sbit DOUT      =    P3^1;																						   
sbit Penirq    =    P3^4;   //��ⴥ������Ӧ�ź�

struct tp_pix_
{
	u16 x;
	u16 y;
};
struct tp_pixu32_
{
	u32 x;
	u32 y;
};
extern struct tp_pix_  tp_pixad,tp_pixlcd;	 //��ǰ���������ADֵ,ǰ�������������ֵ   
extern u16 vx,vy;  //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
extern u16 chx,chy;//Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ
u8 tpstate(void);
void spistar(void);  
void Touch_Adjust(void);
void point(void); //��ͼ����
u16 ReadFromCharFrom7843();         //SPI ������
#endif  
	 
	 



