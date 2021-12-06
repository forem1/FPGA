#include <reg51.h>
#include <intrins.h>
#include <font\font.h>
#include <sys\sys.h>
#include <lcd\lcd.h>
/* 
������Ի���:��Ƭ��STC12LE5A60S2,����33M  ��Ƭ��������ѹ3.3V
����Ĭ��IO���ӷ�ʽ��
�����ߣ�RS-P3^5;    WR-P3^6;   RD-P3^7;   CS-P1^0;   REST-P1^2;
������: DB0-DB7��������P0^0-P0^7;  DB8-DB15��������P2^0-P2^7;��8λģʽ��DB0-DB7���Բ����ӣ�
�����������ӷ�ʽ��(��ʹ�ô����ɲ�����)
T_CLK-P1^7;  T_CS-P1^4;  T_DIN-P3^0;  T_OUT-P3^1;  T_IRQ-P3^4;
*/	
u16 BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ

#ifdef  Bus_16    //��������-16λ����ģʽ   
void LCD_Writ_Bus(char VH,char VL)   //��������д�뺯��
{
    LCD_DataPortH=VH;	
	LCD_DataPortL=VL;		
	LCD_WR=0;
	LCD_WR=1; 
}
#else			//��������-8λ����ģʽ 
void LCD_Writ_Bus(char VH,char VL)   //��������д�뺯��
{	
    LCD_DataPortH=VH;	
   	LCD_WR=0;
	LCD_WR=1;
	LCD_DataPortH=VL;		
	LCD_WR=0;
	LCD_WR=1;
}
#endif
void LCD_WR_DATA8(char VH,char VL) //��������-8λ����
{
    LCD_RS=1;
	LCD_Writ_Bus(VH,VL);
}  
 void LCD_WR_DATA(int da)
{
    LCD_RS=1;
	LCD_Writ_Bus(da>>8,da);
}	  
void LCD_WR_REG(int da)	 
{	
    LCD_RS=0;
	LCD_Writ_Bus(da>>8,da);
}
 void LCD_WR_REG_DATA(int reg,int da)
{
    LCD_WR_REG(reg);
	LCD_WR_DATA(da);
}
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	LCD_WR_REG(0x0020);LCD_WR_DATA8(x1>>8,x1);	  //����X����λ��
    LCD_WR_REG(0x0021);LCD_WR_DATA8(y1>>8,y1);   //����Y����λ��
    LCD_WR_REG(0x0050);LCD_WR_DATA8(x1>>8,x1);   //��ʼX
	LCD_WR_REG(0x0052);LCD_WR_DATA8(y1>>8,y1);	  //��ʼY
    LCD_WR_REG(0x0051);LCD_WR_DATA8(x2>>8,x2);   //����X
	LCD_WR_REG(0x0053);LCD_WR_DATA8(y2>>8,y2);   //����Y
    LCD_WR_REG(0x0022);							 						 
}
void Lcd_Init(void)
{

//����һ����Щ��������ñ����ʱ����ʾ����
   	LCD_CS =1;
	if(LCD_CS==0)
	{
	   LCD_WR_REG_DATA(0,0);
	   LCD_ShowString(0,0," ");
	   LCD_ShowNum(0,0,0,0);
	   LCD_Show2Num(0,0,0,0);
	   LCD_DrawPoint_big(0,0);
	   LCD_DrawRectangle(0,0,0,0);
	   Draw_Circle(0,0,0);
 	 }
	LCD_RD=1;
	LCD_WR=1;
	LCD_REST=0;
	delayms(20);	//�˴���֤Ҫ��5ms���ϲ��ܱ�֤��λ�ȶ�,�Ƽ�20ms
	LCD_REST=1;	
	delayms(20);	//�˴���֤Ҫ��5ms���ϲ��ܱ�֤��λ�ȶ�,�Ƽ�20ms		
	LCD_CS =0;  //��Ƭѡʹ��

LCD_WR_REG_DATA(0x0001,0x0100); 
 LCD_WR_REG_DATA(0x0002,0x0700); 
 LCD_WR_REG_DATA(0x0003,0x1030); 
 LCD_WR_REG_DATA(0x0004,0x0000); 
 LCD_WR_REG_DATA(0x0008,0x0207);  
 LCD_WR_REG_DATA(0x0009,0x0000);
 LCD_WR_REG_DATA(0x000A,0x0000); 
 LCD_WR_REG_DATA(0x000C,0x0000); 
 LCD_WR_REG_DATA(0x000D,0x0000);
 LCD_WR_REG_DATA(0x000F,0x0000);
//power on sequence VGHVGL
 LCD_WR_REG_DATA(0x0010,0x0000);   
 LCD_WR_REG_DATA(0x0011,0x0007);  
 LCD_WR_REG_DATA(0x0012,0x0000);  
 LCD_WR_REG_DATA(0x0013,0x0000); 
//vgh 
 LCD_WR_REG_DATA(0x0010,0x1290);   
 LCD_WR_REG_DATA(0x0011,0x0227);
 //delayms(100);
 //vregiout 
 LCD_WR_REG_DATA(0x0012,0x001d); //0x001b
 //delayms(100); 
 //vom amplitude
 LCD_WR_REG_DATA(0x0013,0x1500);
 //delayms(100); 
 //vom H
 LCD_WR_REG_DATA(0x0029,0x0018); 
 LCD_WR_REG_DATA(0x002B,0x000D); 

//gamma
 LCD_WR_REG_DATA(0x0030,0x0004);
 LCD_WR_REG_DATA(0x0031,0x0307);
 LCD_WR_REG_DATA(0x0032,0x0002);// 0006
 LCD_WR_REG_DATA(0x0035,0x0206);
 LCD_WR_REG_DATA(0x0036,0x0408);
 LCD_WR_REG_DATA(0x0037,0x0507); 
 LCD_WR_REG_DATA(0x0038,0x0204);//0200
 LCD_WR_REG_DATA(0x0039,0x0707); 
 LCD_WR_REG_DATA(0x003C,0x0405);// 0504
 LCD_WR_REG_DATA(0x003D,0x0F02); 
 //ram
 LCD_WR_REG_DATA(0x0050,0x0000); 
 LCD_WR_REG_DATA(0x0051,0x00EF);
 LCD_WR_REG_DATA(0x0052,0x0000); 
 LCD_WR_REG_DATA(0x0053,0x013F);  
 LCD_WR_REG_DATA(0x0060,0xA700); 
 LCD_WR_REG_DATA(0x0061,0x0001); 
 LCD_WR_REG_DATA(0x006A,0x0000); 
 //
 LCD_WR_REG_DATA(0x0080,0x0000); 
 LCD_WR_REG_DATA(0x0081,0x0000); 
 LCD_WR_REG_DATA(0x0082,0x0000); 
 LCD_WR_REG_DATA(0x0083,0x0000); 
 LCD_WR_REG_DATA(0x0084,0x0000); 
 LCD_WR_REG_DATA(0x0085,0x0000); 
 //
 LCD_WR_REG_DATA(0x0090,0x0010); 
 LCD_WR_REG_DATA(0x0092,0x0600); 
 LCD_WR_REG_DATA(0x0093,0x0003); 
 LCD_WR_REG_DATA(0x0095,0x0110); 
 LCD_WR_REG_DATA(0x0097,0x0000); 
 LCD_WR_REG_DATA(0x0098,0x0000);
 LCD_WR_REG_DATA(0x0007,0x0133);

}
//��������
//Color:Ҫ���������ɫ
void LCD_Clear(u16 Color)
{
	u8 VH,VL;
	u16 i,j;
	VH=Color>>8;
	VL=Color;	
	Address_set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
        	 LCD_WR_DATA8(VH,VL);
	    }

	  }
}
//��ָ��λ����ʾһ������(32*33��С)
//dcolorΪ������ɫ��gbcolorΪ������ɫ
void showhanzi(unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j;
	unsigned char *temp=hanzi;    
    Address_set(x,y,x+31,y+31); //��������      
	temp+=index*128;	
	for(j=0;j<128;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
		 	if((*temp&(1<<i))!=0)
			{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
		temp++;
	 }
}
//����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y)
{
	Address_set(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA(POINT_COLOR); 	    
} 	 
//��һ�����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint_big(u16 x,u16 y)
{
	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
} 
//��ָ�����������ָ����ɫ
//�����С:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	Address_set(xsta,ysta,xend,yend);      //���ù��λ�� 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//���ù��λ�� 	    
	} 					  	    
}  
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//������
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 
//��ָ��λ����ʾһ���ַ�

//num:Ҫ��ʾ���ַ�:" "--->"~"
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//��ָ��λ����ʾһ���ַ�

//num:Ҫ��ʾ���ַ�:" "--->"~"

//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode)
{
    u8 temp;
    u8 pos,t;
	u16 x0=x;
	u16 colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return;	    
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	Address_set(x,y,x+8-1,y+16-1);      //���ù��λ�� 
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(u16)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_WR_DATA(POINT_COLOR);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(u16)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   
//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//color:��ɫ
//num:��ֵ(0~4294967295);	
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len)
{         	
	u8 t,temp;
	u8 enshow=0;
	num=(u16)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0); 
	}
} 
//��ʾ2������
//x,y:�������
//num:��ֵ(0~99);	 
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//��16����
void LCD_ShowString(u16 x,u16 y,const u8 *p)
{         
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;LCD_Clear(RED);}
        LCD_ShowChar(x,y,*p,0);
        x+=8;
        p++;
    }  
}












