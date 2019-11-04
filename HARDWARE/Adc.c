#include "Adc.h"

_ADCS ADCS = {
	.AD0_VAC={GPIOA,PIN4,4},
	.AD1_VFN={GPIOA,PIN5,5},
	.AD2_VTY={GPIOA,PIN6,6},
	.AD3_BAT={GPIOA,PIN7,7},
	.AD4_IF={GPIOC,PIN4,14},
	.AD5_IC={GPIOC,PIN5,15},
	.AD6_IC1={GPIOB,PIN0,8},
};

void AdcInit(void)
{
	RCC->APB2RSTR |= 1<<9;
	delay_ms(2);
	RCC->APB2RSTR &= ~(1<<9);
	delay_ms(2);
	RCC->AHBENR  |= 0x05;    	//ʹ��PA��PCʱ��
	RCC->APB2ENR |= (1<<9);		//ʹ��ADC1ʱ��
	//AD0��AD1��AD2��AD3ģ������,����
	GPIO_Set(ADCS.AD0_VAC.PORT,ADCS.AD0_VAC.PIN | ADCS.AD1_VFN.PIN | ADCS.AD2_VTY.PIN | ADCS.AD3_BAT.PIN,GPIO_MODE_AIN,0,0,GPIO_PUPD_PD);
	//AD4��AD5ģ������,����
	GPIO_Set(ADCS.AD4_IF.PORT,ADCS.AD4_IF.PIN | ADCS.AD5_IC.PIN,GPIO_MODE_AIN,0,0,GPIO_PUPD_PD);
	//AD6ģ������,����
	GPIO_Set(ADCS.AD6_IC1.PORT,ADCS.AD6_IC1.PIN | ADCS.AD6_IC1.PIN,GPIO_MODE_AIN,0,0,GPIO_PUPD_PD);
	ADC->CCR=1<<16;
	ADC1->CR1=0;   				//CR1��������
	ADC1->CR2=0;   				//CR2��������	
	ADC1->SQR1&=~(0x0f<<20); 
	ADC1->SMPR3 |= 0xFFF << 12;	//ͨ��4��5��6��7 384����������
	ADC1->SMPR2 |= 0x3F << 15;	//ͨ��14��15 384���������� 
	ADC1->SMPR2 |= 0x07 << 9;	//ͨ��8 384���������� 
 	ADC1->CR2|=1<<0;	   		//����ADת����	 
	delay_ms(2);	
}

//���ADCֵ
//ch:ͨ��ֵ 0~18
//����ֵ:ת�����
u16 Get_Adc(u8 ch)   
{
	//����ת������	  		 
	ADC1->SQR5&=0XFFFFFFE0;//��������1 ͨ��ch
	ADC1->SQR5|=ch;		  			    
	ADC1->CR2|=1<<30;       //��������ת��ͨ�� 
	ADC1->CR2|=1<<0;	   	//����ADת����	 
	while(!(ADC1->SR&(1<<1)));//�ȴ�ת������	 	   
	return ADC1->DR;		//����adcֵ	
}
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
}  
