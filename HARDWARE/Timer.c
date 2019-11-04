#include "Timer.h"

_Capture Capture;

void CaptureInit(void) {
	RCC->APB1ENR |= 0x02;		//ʹ��TIM3ʱ��
	RCC->AHBENR |= 0x02;		//PBʱ��
//	GPIO_Set(GPIOB, PIN0 | PIN1, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M,GPIO_PUPD_PD);
	GPIO_Set(GPIOB, PIN1, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M,GPIO_PUPD_PD);
//	GPIO_AF_Set(GPIOB, 0, 2);		//PB0,AF2
	GPIO_AF_Set(GPIOB, 1, 2);		//PB1,AF2

	TIM3->ARR = 0xFFFF;  			//���ֵ65535   
	TIM3->PSC = 3199;  				//Ƶ��1K 

	TIM3->SMCR |= 0x100;			//
	TIM3->CCMR2 |= 0x101;
	TIM3->CCER |= 0x1100;
	TIM3->EGR = 1 << 0;	
	TIM3->DIER |= (0x01 << 4);   	//ͨ��4	
	TIM3->DIER |= 1;   				
	MY_NVIC_Init(0, 1, TIM3_IRQn, 0);
	TIM3->CR1 |= 0x01;    			//ʹ�ܶ�ʱ��
}

void TIM3_IRQHandler(void) {
	_CapCh* CapCh;
	u8 sr = TIM3->SR & 0xFF;
	if (sr & 0x10) {
		CapCh = &Capture.CAP2_FFN;
		if (!CapCh->FirstFlag)			//��һ�β���������
		{
			CapCh->FirstFlag = 1;
			CapCh->Cnt1 = TIM3->CCR4;
		} else							//�ڶ��β���������
		{
			CapCh->FirstFlag = 0;
			CapCh->Cnt2 = TIM3->CCR4;
			if (CapCh->Cnt2 >= CapCh->Cnt1)	//��������������ʱ���			
				CapCh->TimDiff = (CapCh->Cnt2 - CapCh->Cnt1);
			else
				CapCh->TimDiff = 0xFFFF + (CapCh->Cnt2 - CapCh->Cnt1);
			CapCh->Freq = ((10000 / CapCh->TimDiff)>>1);
			Freq.Status = FREQCAPED;
		}
	}
	TIM3->SR &= 0xFE;
}
