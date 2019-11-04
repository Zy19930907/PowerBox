#include "Exti.h"

void ExtiInit(void)
{
	RCC->AHBENR |= 0x02;		//使能PB时钟
	GPIO_Set(GPIOB,PIN0 | PIN1,GPIO_MODE_IN,0,0,GPIO_PUPD_PD); 			//下拉输入 
	Ex_NVIC_Config(GPIO_B,0,RTIR);
	Ex_NVIC_Config(GPIO_B,1,RTIR);
	MY_NVIC_Init(0,1,EXTI0_IRQn,0);		
	MY_NVIC_Init(0,2,EXTI1_IRQn,0);		   
}

//外部中断0服务程序
void EXTI0_IRQHandler(void)
{	 
	if(EXTI->PR & 0x01)
	EXTI->PR |=1<<0;  //清除LINE0上的中断标志位  
}

//外部中断2服务程序
void EXTI1_IRQHandler(void)
{ 
	if(EXTI->PR & 0x02)
	EXTI->PR = 1<<1;  //清除LINE2上的中断标志位  
}
