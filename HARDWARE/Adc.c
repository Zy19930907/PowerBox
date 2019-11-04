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
	RCC->AHBENR  |= 0x05;    	//使能PA、PC时钟
	RCC->APB2ENR |= (1<<9);		//使能ADC1时钟
	//AD0、AD1、AD2、AD3模拟输入,下拉
	GPIO_Set(ADCS.AD0_VAC.PORT,ADCS.AD0_VAC.PIN | ADCS.AD1_VFN.PIN | ADCS.AD2_VTY.PIN | ADCS.AD3_BAT.PIN,GPIO_MODE_AIN,0,0,GPIO_PUPD_PD);
	//AD4、AD5模拟输入,下拉
	GPIO_Set(ADCS.AD4_IF.PORT,ADCS.AD4_IF.PIN | ADCS.AD5_IC.PIN,GPIO_MODE_AIN,0,0,GPIO_PUPD_PD);
	//AD6模拟输入,下拉
	GPIO_Set(ADCS.AD6_IC1.PORT,ADCS.AD6_IC1.PIN | ADCS.AD6_IC1.PIN,GPIO_MODE_AIN,0,0,GPIO_PUPD_PD);
	ADC->CCR=1<<16;
	ADC1->CR1=0;   				//CR1设置清零
	ADC1->CR2=0;   				//CR2设置清零	
	ADC1->SQR1&=~(0x0f<<20); 
	ADC1->SMPR3 |= 0xFFF << 12;	//通道4、5、6、7 384个采样周期
	ADC1->SMPR2 |= 0x3F << 15;	//通道14、15 384个采样周期 
	ADC1->SMPR2 |= 0x07 << 9;	//通道8 384个采样周期 
 	ADC1->CR2|=1<<0;	   		//开启AD转换器	 
	delay_ms(2);	
}

//获得ADC值
//ch:通道值 0~18
//返回值:转换结果
u16 Get_Adc(u8 ch)   
{
	//设置转换序列	  		 
	ADC1->SQR5&=0XFFFFFFE0;//规则序列1 通道ch
	ADC1->SQR5|=ch;		  			    
	ADC1->CR2|=1<<30;       //启动规则转换通道 
	ADC1->CR2|=1<<0;	   	//开启AD转换器	 
	while(!(ADC1->SR&(1<<1)));//等待转换结束	 	   
	return ADC1->DR;		//返回adc值	
}
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
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
