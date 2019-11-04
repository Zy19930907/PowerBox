#include "Mcu.h"

vu64 SYS_TICK;
u64 MsTickDiff(u64 tick)
{
	unsigned long systick = SYS_TICK;
	if(tick > systick)
		return 0xFFFFFFFFFFFFFFFF - tick + systick;
	else
		return systick - tick;
}
//����������ƫ�Ƶ�ַ
//NVIC_VectTab:��ַ
//Offset:ƫ����		 
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab,u32 Offset)	 
{ 	   	  
	SCB->VTOR=NVIC_VectTab|(Offset&(u32)0xFFFFFE00);//����NVIC��������ƫ�ƼĴ���,VTOR��9λ����,��[8:0]������
}
//����NVIC����
//NVIC_Group:NVIC���� 0~4 �ܹ�5�� 		   
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//ȡ����λ
	temp1<<=8;
	temp=SCB->AIRCR;  //��ȡ��ǰ������
	temp&=0X0000F8FF; //�����ǰ����
	temp|=0X05FA0000; //д��Կ��
	temp|=temp1;		
	SCB->AIRCR=temp;  //���÷���	    	  				   
}
//����NVIC 
//NVIC_PreemptionPriority:��ռ���ȼ�
//NVIC_SubPriority       :��Ӧ���ȼ�
//NVIC_Channel           :�жϱ��
//NVIC_Group             :�жϷ��� 0~4
//ע�����ȼ����ܳ����趨����ķ�Χ!����������벻���Ĵ���
//�黮��:
//��0:0λ��ռ���ȼ�,4λ��Ӧ���ȼ�
//��1:1λ��ռ���ȼ�,3λ��Ӧ���ȼ�
//��2:2λ��ռ���ȼ�,2λ��Ӧ���ȼ�
//��3:3λ��ռ���ȼ�,1λ��Ӧ���ȼ�
//��4:4λ��ռ���ȼ�,0λ��Ӧ���ȼ�
//NVIC_SubPriority��NVIC_PreemptionPriority��ԭ����,��ֵԽС,Խ����	   
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	  
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//���÷���
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;								//ȡ����λ
	NVIC->ISER[NVIC_Channel/32]|=1<<NVIC_Channel%32;//ʹ���ж�λ(Ҫ����Ļ�,����ICER��ӦλΪ1����)
	NVIC->IP[NVIC_Channel]|=temp<<4;				//������Ӧ���ȼ����������ȼ�   	    	  				   
} 

void SystemInit (void)
{
  /*!< Set MSION bit */
  RCC->CR |= (uint32_t)0x00000100;

  /*!< Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], MCOSEL[2:0] and MCOPRE[2:0] bits */
  RCC->CFGR &= (uint32_t)0x88FFC00C;
  
  /*!< Reset HSION, HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xEEFEFFFE;

  /*!< Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /*!< Reset PLLSRC, PLLMUL[3:0] and PLLDIV[1:0] bits */
  RCC->CFGR &= (uint32_t)0xFF02FFFF;

  /*!< Disable all interrupts */
  RCC->CIR = 0x00000000;

#ifdef DATA_IN_ExtSRAM
  SystemInit_ExtMemCtl(); 
#endif /* DATA_IN_ExtSRAM */
    
  /* Configure the System clock frequency, AHB/APBx prescalers and Flash settings */
 // SetSysClock();
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM. */
#else
  SCB->VTOR |= 0x08000000 | 0; /* Vector Table Relocation in Internal FLASH. */
#endif
}

void SYS_CLK_CONFIG(void)
{
	RCC->CR |= 0x01;							//��HSI
	while(!(RCC->CR & 0x02));					//�ȴ�HSI����
	RCC->APB1ENR |= 1<<28;
	PWR->CR = (1<<11);
	RCC->CFGR |= (5<<11);
	RCC->CFGR |= (1<<18);
	RCC->CFGR |= (1<<22);
	FLASH->ACR |= 1<<2;
	FLASH->ACR |= 1;
	RCC->CR |= (1<<24);
	while(!((RCC->CR>>25)&0x01));
	RCC->CFGR |= 0x03;	//PLL��Ϊʱ��ϵͳʱ��
	
	while(((RCC->CFGR >> 2) & 0x03) != 0x03){}	//�ȴ�PLL�����Ϊϵͳʱ��
}

//1msϵͳʱ����ʱ����ʼ��
void SystemTickInit(void)
{
	RCC->APB1ENR|=0x01;				//TIM2ʱ��ʹ��    
 	TIM2->ARR = 1000;  				//�趨�������Զ���װֵ 
	TIM2->PSC = 31;  				//Ԥ��Ƶ��	  
	TIM2->DIER |=1<<0;   			//��������ж�	  
	TIM2->CR1|=0x01;    			//ʹ�ܶ�ʱ��2
  	MY_NVIC_Init(0,0,TIM2_IRQn,0);	//������ȼ���ֹ�������ж���ʹ����ʱ������������	
}

void McuInit(void)
{
	//SCB->VTOR = 0x08005000;
	SYS_CLK_CONFIG();
	SystemTickInit();
}

//��ʱ��2�жϷ������	 
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//����ж�
	{
		SYS_TICK++;			
		TaskManger.Tick++;
	}		
	TIM2->SR&=~(1<<0);//����жϱ�־λ 	    
}

void delay_ms(u64 nms)
{
	u64 tick = SYS_TICK;
	while(MsTickDiff(tick) <= nms);
}
