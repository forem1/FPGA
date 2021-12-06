/*
GUANFU_WANG@126.COM
QQ:21578936
δ����ɽ�ֹ�κ���ҵ��;
*/

#include "sccb.h"
#include "delay.h"
#include "stm32f10x_lib.h"
/*
-----------------------------------------------
   ����: ��ʼ��ģ��SCCB�ӿ�
   ����: ��
 ����ֵ: ��
-----------------------------------------------
*/
void SCCB_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
   /* Enable GPIOC  clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  SCCB_SIC_BIT|SCCB_SID_BIT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
}
void SCCB_SID_GPIO_OUTPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
   /* Enable GPIOC  clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  SCCB_SID_BIT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void SCCB_SID_GPIO_INPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
   /* Enable GPIOC  clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  SCCB_SID_BIT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*
-----------------------------------------------
   ����: start����,SCCB����ʼ�ź�
   ����: ��
 ����ֵ: ��
-----------------------------------------------
*/
void startSCCB(void)
{
    SCCB_SID_H();     //�����߸ߵ�ƽ
    delay_us(15);

    SCCB_SIC_H();	   //��ʱ���߸ߵ�ʱ���������ɸ�����
    delay_us(15);
 
    SCCB_SID_L();
    delay_us(15);

    SCCB_SIC_L();	 //�����߻ָ��͵�ƽ��������������Ҫ
    delay_us(15);


}
/*
-----------------------------------------------
   ����: stop����,SCCB��ֹͣ�ź�
   ����: ��
 ����ֵ: ��
-----------------------------------------------
*/
void stopSCCB(void)
{
    SCCB_SID_L();
    delay_us(15);
 
    SCCB_SIC_H();	
    delay_us(15);
  

    SCCB_SID_H();	
    delay_us(15);
   
}

/*
-----------------------------------------------
   ����: noAck,����������ȡ�е����һ����������
   ����: ��
 ����ֵ: ��
-----------------------------------------------
*/
void noAck(void)
{
	
	SCCB_SID_H();	
	delay_us(15);
	
	SCCB_SIC_H();	
	delay_us(15);
	
	SCCB_SIC_L();	
	delay_us(15);
	
	SCCB_SID_L();	
	delay_us(15);

}

/*
-----------------------------------------------
   ����: д��һ���ֽڵ����ݵ�SCCB
   ����: д������
 ����ֵ: ���ͳɹ�����1������ʧ�ܷ���0
-----------------------------------------------
*/
unsigned char SCCBwriteByte(unsigned char m_data)
{
	unsigned char j,tem;

	for(j=0;j<8;j++) //ѭ��8�η�������
	{
		if((m_data<<j)&0x80)
		{
			SCCB_SID_H();	
		}
		else
		{
			SCCB_SID_L();	
		}
		delay_us(14);
		SCCB_SIC_H();	
		delay_us(15);
		SCCB_SIC_L();	
		delay_us(1);

	}
	delay_us(5);
	SCCB_SID_IN;/*����SDAΪ����*/
	delay_us(10);
	SCCB_SIC_H();	
	delay_us(25);
	if(SCCB_SID_STATE){tem=0;}   //SDA=1����ʧ�ܣ�����0}
	else {tem=1;}   //SDA=0���ͳɹ�������1
	SCCB_SIC_L();	
	delay_us(10);	
        SCCB_SID_OUT;/*����SDAΪ���*/

	return (tem);  
}

/*
-----------------------------------------------
   ����: һ���ֽ����ݶ�ȡ���ҷ���
   ����: ��
 ����ֵ: ��ȡ��������
-----------------------------------------------
*/
unsigned char SCCBreadByte(void)
{
	unsigned char read,j;
	read=0x00;
	
	SCCB_SID_IN;/*����SDAΪ����*/
	delay_us(10);
	for(j=8;j>0;j--) //ѭ��8�ν�������
	{		     
		delay_us(10);
		SCCB_SIC_H();
		delay_us(10);
		read=read<<1;
		if(SCCB_SID_STATE) 
		{
			read=read+1;
		}
		SCCB_SIC_L();
		delay_us(2);
	}
        SCCB_SID_OUT;/*����SDAΪ���*/	
	return(read);
    
}
