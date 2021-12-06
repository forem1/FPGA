#include <reg51.h>
#include <intrins.h>
#include <sys\sys.h>
#include <touch\touch.h>
#include <lcd\lcd.h>

//***���������β�ͬ��ԭ��Ĭ�ϵ�У׼����ֵ���ܻ�������ʶ��׼������У׼����ʹ�ã�������ʹ�ù̶���Ĭ��У׼����
u16 vx=13581,vy=10879;  //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
u16 chx=3676,chy=3886;//Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ
//***���������β�ͬ��ԭ��Ĭ�ϵ�У׼����ֵ���ܻ�������ʶ��׼������У׼����ʹ�ã�������ʹ�ù̶���Ĭ��У׼����

struct tp_pix_  tp_pixad,tp_pixlcd;	 //��ǰ���������ADֵ,ǰ�������������ֵ   


u8 tpstate(void)
{
	return 	Penirq;
}
//**********************************************************
void spistar(void)                                     //SPI��ʼ
{
CS=1;
DCLK=1;
DIN=1;
DCLK=1;
}
//**********************************************************
void WriteCharTo7843(unsigned char num)          //SPIд����
{
unsigned char count=0;
DCLK=0;
for(count=0;count<8;count++)
{
num<<=1;
DIN=CY;
DCLK=0; _nop_();_nop_();_nop_();                //��������Ч
DCLK=1; _nop_();_nop_();_nop_();
}
}
//**********************************************************
u16 ReadFromCharFrom7843()             //SPI ������
{
	u8 count=0;
	u16 Num=0;
	for(count=0;count<12;count++)
	{
		Num<<=1;		
		DCLK=1; _nop_();_nop_();_nop_();                //�½�����Ч
		DCLK=0; _nop_();_nop_();_nop_();
		if(DOUT)
		{
			Num|=1;
		}
		
	}

return(Num);
}	
//��7846/7843/XPT2046/UH7843/UH7846��ȡadcֵ	  0x90=y   0xd0-x
u16 ADS_Read_AD(unsigned char CMD)          
{
u16 l;
CS=0;
WriteCharTo7843(CMD);        //�Ϳ����ּ��ò�ַ�ʽ��X���� ��ϸ����й�����
DCLK=1; _nop_();_nop_();_nop_();_nop_();
DCLK=0; _nop_();_nop_();_nop_();_nop_();
l=ReadFromCharFrom7843();
CS=1;
return l;
}		   
//��ȡһ������ֵ
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 
#define READ_TIMES 15 //��ȡ����
#define LOST_VAL 5	  //����ֵ
u16 ADS_Read_XY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=ADS_Read_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
//���˲��������ȡ
//��Сֵ��������100.
u8 Read_ADS(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	 									   
	if(xtemp<100||ytemp<100)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}
//2�ζ�ȡADS7846,������ȡ2����Ч��ADֵ,�������ε�ƫ��ܳ���
//50,��������,����Ϊ������ȷ,�����������.	   
//�ú����ܴ�����׼ȷ��
#define ERR_RANGE 20 //��Χ 
u8 Read_ADS2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=Read_ADS(&x1,&y1);   
    if(flag==0)return(0);
    flag=Read_ADS(&x2,&y2);	
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-ERR_RANGE��
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)>>1;
        *y=(y1+y2)>>1;		
        return 1;
    }else return 0;	  
} 
//��ȷ��ȡһ������,У׼��ʱ���õ�	   
u8 Read_TP_Once(void)
{
	u8 re=0;
	u16 x1,y1;
	while(re==0)
	{
		while(!Read_ADS2(&tp_pixad.x,&tp_pixad.y));
		delayms(10);
		while(!Read_ADS2(&x1,&y1));
		if(tp_pixad.x==x1&&tp_pixad.y==y1)
		{
			re=1; 
		}
	} 
	return re;
}
//////////////////////////////////////////////////
//��LCD�����йصĺ���  
//��һ��������
//����У׼�õ�
void Drow_Touch_Point(u16 x,u16 y)
{
	LCD_DrawLine(x-12,y,x+13,y);//����
	LCD_DrawLine(x,y-12,x,y+13);//����
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
//	Draw_Circle(x,y,6);//������Ȧ
}	  
//ת�����
//���ݴ�������У׼����������ת����Ľ��,������X0,Y0��
u8 Convert_Pos(void)
{		 	 
	u8 l=0; 
	if(Read_ADS2(&tp_pixad.x,&tp_pixad.y))
	{
		l=1;
		tp_pixlcd.x=tp_pixad.x>chx?((u32)tp_pixad.x-(u32)chx)*1000/vx:((u32)chx-(u32)tp_pixad.x)*1000/vx;
		tp_pixlcd.y=tp_pixad.y>chy?((u32)tp_pixad.y-(u32)chy)*1000/vy:((u32)chy-(u32)tp_pixad.y)*1000/vy;
	}
	return l;
}	   
//������У׼����
//�õ��ĸ�У׼����
#define tp_pianyi 50   //У׼����ƫ����	
#define tp_xiaozhun 1000   //У׼����
void Touch_Adjust(void)
{	
	float vx1,vx2,vy1,vy2;  //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
	u16 chx1,chx2,chy1,chy2;//Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ
	u16 lx,ly;				 
	struct tp_pixu32_ p[4];
	u8  cnt=0;	 
	cnt=0;				
	POINT_COLOR=BLUE;
	BACK_COLOR =WHITE;
	LCD_Clear(WHITE);//����   
	POINT_COLOR=RED;//��ɫ 
	LCD_Clear(WHITE);//���� 
	Drow_Touch_Point(tp_pianyi,tp_pianyi);//����1 
	while(1)
	{
		if(Penirq==0)//����������
		{
			if(Read_TP_Once())//�õ����ΰ���ֵ
			{  								   
				p[cnt].x=tp_pixad.x;
				p[cnt].y=tp_pixad.y;
				cnt++; 
			}			 
			switch(cnt)
			{			   
				case 1:
					LCD_Clear(WHITE);//���� 
					while(!Penirq)  //�ȴ�����
					{
					}
					Drow_Touch_Point(LCD_W-tp_pianyi-1,tp_pianyi);//����2
					break;
				case 2:
					LCD_Clear(WHITE);//���� 
					while(!Penirq)  //�ȴ�����
					{
					}
					Drow_Touch_Point(tp_pianyi,LCD_H-tp_pianyi-1);//����3
					break;
				case 3:
					LCD_Clear(WHITE);//���� 
					while(!Penirq)  //�ȴ�����
					{
					}
					Drow_Touch_Point(LCD_W-tp_pianyi-1,LCD_H-tp_pianyi-1);//����4
					break;
				case 4:	 //ȫ���ĸ����Ѿ��õ�
	    		   	LCD_Clear(WHITE);//���� 
				   	while(!Penirq)  //�ȴ�����
					{
					}
			   		vx1=p[1].x>p[0].x?(p[1].x-p[0].x+1)*1000/(LCD_W-tp_pianyi-tp_pianyi):(p[0].x-p[1].x-1)*1000/(LCD_W-tp_pianyi-tp_pianyi);
				 	chx1=p[1].x>p[0].x?p[0].x-(vx1*tp_pianyi)/1000:p[0].x+(vx1*tp_pianyi)/1000;
				   	vy1=p[2].y>p[0].y?(p[2].y-p[0].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi):(p[0].y-p[2].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi);
					chy1=p[2].y>p[0].y?p[0].y-(vy1*tp_pianyi)/1000:p[0].y+(vy1*tp_pianyi)/1000; 
					
					vx2=p[3].x>p[2].x?(p[3].x-p[2].x+1)*1000/(LCD_W-tp_pianyi-tp_pianyi):(p[2].x-p[3].x-1)*1000/(LCD_W-tp_pianyi-tp_pianyi);
					chx2=p[3].x>p[2].x?p[2].x-(vx2*tp_pianyi)/1000:p[2].x+(vx2*tp_pianyi)/1000;
				   	vy2=p[3].y>p[1].y?(p[3].y-p[1].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi):(p[1].y-p[3].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi);
					chy2=p[3].y>p[1].y?p[1].y-(vy2*tp_pianyi)/1000:p[1].y+(vy2*tp_pianyi)/1000; 


					if((vx1>vx2&&vx1>vx2+tp_xiaozhun)||(vx1<vx2&&vx1<vx2-tp_xiaozhun)||(vy1>vy2&&vy1>vy2+tp_xiaozhun)||(vy1<vy2&&vy1<vy2-tp_xiaozhun))
					{
						cnt=0;
						LCD_Clear(WHITE);//���� 
						Drow_Touch_Point(tp_pianyi,tp_pianyi);//����1 
						continue;
					}
					vx=(vx1+vx2)/2;vy=(vy1+vy2)/2;
					chx=(chx1+chx2)/2;chy=(chy1+chy2)/2;	
																
					//��ʾУ׼��Ϣ
					LCD_Clear(WHITE);//���� 
					POINT_COLOR=BLACK;
					BACK_COLOR=BLUE;	
			
					lx=0;ly=50;			
					LCD_ShowString(lx,ly,"VX1:");lx+=40;LCD_ShowNum(lx,ly,vx1,5);					
					lx=0;ly+=20;
					LCD_ShowString(lx,ly,"Vy1:");lx+=40;LCD_ShowNum(lx,ly,vy1,5);					
					lx=0;ly+=20; 
					LCD_ShowString(lx,ly,"CHX1:");lx+=40;LCD_ShowNum(lx,ly,chx1,5);					
				    lx=0;ly+=20; 
					LCD_ShowString(lx,ly,"CHY1:");lx+=40;LCD_ShowNum(lx,ly,chy1,5);

					lx=100;ly=50;			
					LCD_ShowString(lx,ly,"VX2:");lx+=40;LCD_ShowNum(lx,ly,vx2,5);					
					lx=100;ly+=20;
					LCD_ShowString(lx,ly,"Vy2:");lx+=40;LCD_ShowNum(lx,ly,vy2,5);					
					lx=100;ly+=20; 
					LCD_ShowString(lx,ly,"CHX2:");lx+=40;LCD_ShowNum(lx,ly,chx2,5);					
				    lx=100;ly+=20; 
					LCD_ShowString(lx,ly,"CHY2:");lx+=40;LCD_ShowNum(lx,ly,chy2,5);
				
					lx=50;ly=150;			
					LCD_ShowString(lx,ly,"VX:");lx+=40;LCD_ShowNum(lx,ly,vx,5);					
					lx=50;ly+=20;
					LCD_ShowString(lx,ly,"Vy:");lx+=40;LCD_ShowNum(lx,ly,vy,5);					
					lx=50;ly+=20; 
					LCD_ShowString(lx,ly,"CHX:");lx+=40;LCD_ShowNum(lx,ly,chx,5);					
				    lx=50;ly+=20; 
					LCD_ShowString(lx,ly,"CHY:");lx+=40;LCD_ShowNum(lx,ly,chy,5);

					lx=30;ly+=30;
					LCD_ShowString(lx,ly,"Adjust OK!  Touch Anywhere To Continue");										  
					Read_TP_Once(); //�ȴ�����������

					LCD_Clear(WHITE);//����
					return;//У�����				 
			}
		}
	} 
}
void point(void) //��ͼ����
{
	double t=0;
	
    while(1)
	{  	
		if(Penirq==0)
		{
			t=0;
			if(Convert_Pos())	//�õ�����ֵ
			{
			//	LCD_ShowString(10,250,"X:");LCD_ShowNum(30,250,(u32)tp_pixad.x,6);	
				//LCD_ShowString(180,250,"Y:");LCD_ShowNum(200,250,(u32)tp_pixad.y,6);	
				LCD_ShowString(10,250,"X:");LCD_ShowNum(30,250,tp_pixad.x,4);
				LCD_ShowString(180,250,"Y:");LCD_ShowNum(200,250,tp_pixad.y,4);					
				LCD_DrawPoint_big(tp_pixlcd.x,tp_pixlcd.y);   
			 }
			
		}
		else
		{	
			t++;		
			if(t>65000)
			{
				return;
			}
		}		

		}
}	    	
