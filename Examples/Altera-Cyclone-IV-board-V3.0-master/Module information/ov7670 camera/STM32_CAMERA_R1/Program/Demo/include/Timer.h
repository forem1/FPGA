#ifndef __TIMER_H__
#define __TIMER_H__




/*-----------------------------------------------------------------------
	˵����
		��Ӳ��ʱ��0���������ʱ��
		TMR0ÿ10mS��һ���ж�
-------------------------------------------------------------------------*/
#define TIMER_NUMBER	7

/*------------------------------------------------------------------------
		����Ը�����ʱ������;�Ͷ�ʱ��ʱ����������

--------------------------------------------------------------------------*/
// ��ʾ����		2mS
#define T_SETPARA				0		
#define T_SETPARA_TIME			2

// ��ʾѡ������		2mS
#define T_SELECT				1		
#define T_SELECT_TIME			2

// ��ʾ����		2mS
#define T_WORK					1		
#define T_WORK_TIME				2

// ����ɨ�趨ʱ��   20ms
#define T_SCAN_KEYBOARD			3	   		
#define T_SCAN_KEYBOARD_TIME	15

// 
#define T_SCAN_SD_CARD			4		
#define T_SCAN_SD_CARD_TIME		3

// 
#define T_GuiWelcome			5		
#define T_GuiWelcome_TIME		15

// POWER
#define T_POWER					6		
#define T_POWER_TIME			10000


#define OSTask_Init()	InitTaskTimer() 


/*------------------------ ������ --------------------------------*/
void InitTaskTimer(void);
void SetTimer(u8 TimerNO, u16 Time1);
void Timer(void);
//byte TimerOver(byte TimerNo);
bool TimerOver(u8 TimerNo);
void KillTimer(u16  TimerNO);
bool TimerExist(u16 TimerNO);

#endif
