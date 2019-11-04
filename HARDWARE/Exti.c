#include "Exti.h"

void ExtiInit(void)
{
	RCC->AHBENR |= 0x02;		//ʹ��PBʱ��
	GPIO_Set(GPIOB,PIN0 | PIN1,GPIO_MODE_IN,0,0,GPIO_PUPD_PD); 			//�������� 
	Ex_NVIC_Config(GPIO_B,0,RTIR);
	Ex_NVIC_Config(GPIO_B,1,RTIR);
	MY_NVIC_Init(0,1,EXTI0_IRQn,0);		
	MY_NVIC_Init(0,2,EXTI1_IRQn,0);		   
}

//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{	 
	if(EXTI->PR & 0x01)
	EXTI->PR |=1<<0;  //���LINE0�ϵ��жϱ�־λ  
}

//�ⲿ�ж�2�������
void EXTI1_IRQHandler(void)
{ 
	if(EXTI->PR & 0x02)
	EXTI->PR = 1<<1;  //���LINE2�ϵ��жϱ�־λ  
}
