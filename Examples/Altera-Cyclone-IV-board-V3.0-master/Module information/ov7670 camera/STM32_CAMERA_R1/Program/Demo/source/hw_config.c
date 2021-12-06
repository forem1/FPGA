
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "main.h"
#include "hw_config.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void RCC_Config(void);
/* Private functions ---------------------------------------------------------*/

/********************************************************************
�������ܣ�ϵͳ��ʼ��
��ڲ������ޡ�
��    �أ��ޡ�
��    ע��
            ����ϵͳʱ��
			��ʼ��������ʱ��
			����Ҫ�õ�����
			�ж�����
			����IO���ţ�����IO����
			��ʼ���洢��
			��ʼ��USB
********************************************************************/
void System_Init(void)
{

  /* RCC configuration */
  RCC_Config();		 //��ʱ����صļĴ�������
  NVIC_Configuration_rtc();
  RTC_Configuration();
  SPI_Configuration();	//SPI����
  SysTick_Config();		//��ʱ������
 
  disk_initialize (0);
  Touch_Config(); 
   
}

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
static void RCC_Config(void)
{

  /* RCC system reset(for debug purpose) */
  RCC_DeInit();	   //��RCC�Ĵ����豸ȱʡֵ

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);  //�ⲿ��������HSE ����

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();	//�ȴ�HSE����

  if (HSEStartUpStatus == SUCCESS)	  //���HSE�����ȶ��Ҿ���
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  //Ԥȡָ����ʹ��

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);	//����FLASH�洢����ʱʱ��������Ϊ2��ʱ����

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  //����AHBʱ�ӣ�ϵͳʱ��

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);	 //����APB2ʱ�� �� HCLK

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);	   //����APB1ʱ�� �� HCLK/2

    /* ADCCLK = PCLK2/6 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);	//����ADCʱ�ӣ�PCLK/6

	//PLL����
	//PLL����ʱ�ӣ�HSEʱ��Ƶ��   ��� 9��Ƶ��HSE��9��
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);     //72M

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)	 //�ȴ�PLL����
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	//ѡ��PLL��Ϊϵͳʱ��

    /* Wait till PLL is used as system clock source */
	//�ȴ�����ϵͳʱ��ΪPLL�ɹ�
	//����ֵΪ	00��HSI  04��HSE  08��PLL
    while (RCC_GetSYSCLKSource() != 0x08)  
    {}
     
     

     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD , ENABLE);
      /* DMA1 and DMA2 clock enable */
     //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);
     // �ı�ָ���ܽŵ�ӳ�� GPIO_Remap_SWJ_JTAGDisable ��JTAG-DP ���� + SW-DP ʹ��
     //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
      Key_GPIO_Init();
     
    
  }
}

/*******************************************************************************
* Function Name  : SysTick_Config
* Description    : Configure a SysTick Base time to 10 ms.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
 void SysTick_Config(void)
{
  /* Configure HCLK clock as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//����SysTickʱ��ΪAHBʱ��
 
  /* SysTick interrupt each 100 Hz with HCLK equal to 72MHz */
  SysTick_SetReload(720000);   //������װֵ

  /* Enable the SysTick Interrupt */
  SysTick_ITConfig(ENABLE);
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length (time base 10 ms).
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(u32 nCount)
{
  TimingDelay = nCount;

  /* Enable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Enable);
  
  while(TimingDelay != 0)
  {
  }

  /* Disable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Disable);

  /* Clear the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Clear);
}

/*******************************************************************************
* Function Name  : Decrement_TimingDelay
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
void Decrement_TimingDelay(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}
void RTC_Configuration(void)
{
  
   struct tm time_now;
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_OFF);//LSE oscillator OFF
  /* Wait till LSE is ready */
  //while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
 // {}

  /* Select LSE as RTC Clock Source */
 // RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
   RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);//HSE clock divided by 128  selected as RTC clock
  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(62499); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  
        time_now.tm_year = 2011;
	time_now.tm_mon = 2;
	time_now.tm_mday = 26;
	time_now.tm_hour = 21;
	time_now.tm_min = 07;
	time_now.tm_sec = 00;
        time_now.tm_isdst=0;//�ռӵ�
	Time_SetCalendarTime(time_now);
}


void NVIC_Configuration_rtc(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif
}


u8 Check_RTC_MY(void)
{
   if(BKP_ReadBackupRegister(BKP_DR1)!=0xA5A5)
    {
              // u32 delay; 
  	
        
        
       //����PWR��BKP��ʱ�ӣ�from APB1��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
        

	//�������
	PWR_BackupAccessCmd(ENABLE);
 
	//���ݼĴ���ģ�鸴λ
	BKP_DeInit();
        
       // RCC_LSEConfig(RCC_LSE_OFF); 
	//�ⲿ32.768K��Ӵż�Ǹ�
	RCC_LSEConfig(RCC_LSE_ON);
        
        return 1; 
    } 
   else 
   {
                  //���RCC�и�λ��־
        RCC_ClearFlag();
        
        //��ȻRTCģ�鲻��Ҫ�������ã��ҵ���������󱸵����Ȼ����
        RCC_RTCCLKCmd(ENABLE);
        //�ȴ�RTCʱ����APB1ʱ��ͬ��
        RTC_WaitForSynchro();
        
        //ʹ�����ж�
        RTC_ITConfig(RTC_IT_SEC,ENABLE);
        //�ȴ��������
        RTC_WaitForLastTask();
        
        RCC_ClearFlag();
    
    /* Enable PWR and BKP clocks */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE);
    
    /* Allow access to BKP Domain */
        PWR_BackupAccessCmd(ENABLE);
        
        return 0;  //RTC OK
     
   }
}


void Setup_RTC(void)
{      
        struct tm time_now;
       //�ȴ��ȶ�
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){};
	//RTCʱ��Դ���ó�LSE���ⲿ32.768K��
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	//RTC����
	RCC_RTCCLKCmd(ENABLE);

	//��������Ҫ�ȴ�APB1ʱ����RTCʱ��ͬ�������ܶ�д�Ĵ���
	RTC_WaitForSynchro();

	//��д�Ĵ���ǰ��Ҫȷ����һ�������Ѿ�����
	RTC_WaitForLastTask();


	//����RTC��Ƶ����ʹRTCʱ��Ϊ1Hz
	//RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
	RTC_SetPrescaler(32767);


	//�ȴ��Ĵ���д�����
	RTC_WaitForLastTask();

	//ʹ�����ж�
	RTC_ITConfig(RTC_IT_SEC, ENABLE);    

	//�ȴ�д�����
	RTC_WaitForLastTask();
        
        
        time_now.tm_year = 2011;
	time_now.tm_mon = 2;
	time_now.tm_mday = 26;
	time_now.tm_hour = 21;
	time_now.tm_min = 07;
	time_now.tm_sec = 00;
        time_now.tm_isdst=0;//�ռӵ�
	Time_SetCalendarTime(time_now);
        BKP_WriteBackupRegister(BKP_DR1,0xA5A5);//������ɺ���󱸼Ĵ�����д�����ַ�0xA5A5
        
        RCC_ClearFlag();
    
        // Enable PWR and BKP clocks
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE);
    
        // Allow access to BKP Domain 
        PWR_BackupAccessCmd(ENABLE);
}
/*

void Setup_RTC(void)
{   
     struct tm time_now;
    if(BKP_ReadBackupRegister(BKP_DR1)!=0xA5A5)
    {
      
        RTC_Configuration();
        
        
	time_now.tm_year = 2010;
	time_now.tm_mon = 6;
	time_now.tm_mday = 30;
	time_now.tm_hour = 12;
	time_now.tm_min = 00;
	time_now.tm_sec = 00;
        time_now.tm_isdst=0;//�ռӵ�
	Time_SetCalendarTime(time_now);
        BKP_WriteBackupRegister(BKP_DR1,0xA5A5);//������ɺ���󱸼Ĵ�����д�����ַ�0xA5A5
    }
    else
    {
             //���RCC�и�λ��־
        RCC_ClearFlag();
        
        //��ȻRTCģ�鲻��Ҫ�������ã��ҵ���������󱸵����Ȼ����
        RCC_RTCCLKCmd(ENABLE);
        //�ȴ�RTCʱ����APB1ʱ��ͬ��
        RTC_WaitForSynchro();
        
        //ʹ�����ж�
        RTC_ITConfig(RTC_IT_SEC,ENABLE);
        //�ȴ��������
        RTC_WaitForLastTask();
     }
    RCC_ClearFlag();
    
    // Enable PWR and BKP clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE);
    
    // Allow access to BKP Domain 
    PWR_BackupAccessCmd(ENABLE);
}
*/
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
