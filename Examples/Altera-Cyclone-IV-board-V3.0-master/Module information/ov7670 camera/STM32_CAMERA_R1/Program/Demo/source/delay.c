/*
GUANFU_WANG@126.COM
QQ:21578936
δ����ɽ�ֹ�κ���ҵ��;
*/
#include "delay.h"

void delay_us(unsigned int i)
 {     
    while( i--)
       {	
         asm("NOP");
        }
 }		  


