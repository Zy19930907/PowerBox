#include "Public.h"

//外部中断配置函数
//只针对GPIOA~I;不包括PVD,RTC,USB_OTG,USB_HS,以太网唤醒等
//参数:
//GPIOx:0~8,代表GPIOA~I
//BITx:需要使能的位;
//TRIM:触发模式,1,下升沿;2,上降沿;3，任意电平触发
//该函数一次只能配置1个IO口,多个IO口,需多次调用
//该函数会自动开启对应中断,以及屏蔽线   	    
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{ 
	u8 EXTOFFSET=(BITx%4)*4;  
	RCC->APB2ENR|=1<<14;  						//使能SYSCFG时钟  
	SYSCFG->EXTICR[BITx/4]&=~(0x000F<<EXTOFFSET);//清除原来设置！！！
	SYSCFG->EXTICR[BITx/4]|=GPIOx<<EXTOFFSET;	//EXTI.BITx映射到GPIOx.BITx 
	//自动设置
	EXTI->IMR|=1<<BITx;					//开启line BITx上的中断(如果要禁止中断，则反操作即可)
	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;	//line BITx上事件下降沿触发
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;	//line BITx上事件上升降沿触发
} 	

void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx)
{  
	GPIOx->AFR[BITx>>3]&=~(0X0F<<((BITx&0X07)*4));
	GPIOx->AFR[BITx>>3]|=(u32)AFx<<((BITx&0X07)*4);
}   

//GPIO通用设置 
//GPIOx:GPIOA~GPIOI.
//BITx:0X0000~0XFFFF,位设置,每个位代表一个IO,第0位代表Px0,第1位代表Px1,依次类推.比如0X0101,代表同时设置Px0和Px8.
//MODE:0~3;模式选择,0,输入(系统复位默认状态);1,普通输出;2,复用功能;3,模拟输入.
//OTYPE:0/1;输出类型选择,0,推挽输出;1,开漏输出.
//OSPEED:0~3;输出速度设置,0,2Mhz;1,25Mhz;2,50Mhz;3,100Mh. 
//PUPD:0~3:上下拉设置,0,不带上下拉;1,上拉;2,下拉;3,保留.
//注意:在输入模式(普通输入/模拟输入)下,OTYPE和OSPEED参数无效!!
void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD)
{  
	u32 pinpos=0,pos=0,curpin=0;
	for(pinpos=0;pinpos<16;pinpos++)
	{
		pos=1<<pinpos;	//一个个位检查 
		curpin=BITx&pos;//检查引脚是否要设置
		if(curpin==pos)	//需要设置
		{
			GPIOx->MODER&=~(3<<(pinpos*2));	//先清除原来的设置
			GPIOx->MODER|=MODE<<(pinpos*2);	//设置新的模式 
			if((MODE==0X01)||(MODE==0X02))	//如果是输出模式/复用功能模式
			{  
				GPIOx->OSPEEDR&=~(3<<(pinpos*2));	//清除原来的设置
				GPIOx->OSPEEDR|=(OSPEED<<(pinpos*2));//设置新的速度值  
				GPIOx->OTYPER&=~(1<<pinpos) ;		//清除原来的设置
				GPIOx->OTYPER|=OTYPE<<pinpos;		//设置新的输出模式
			}  
			GPIOx->PUPDR&=~(3<<(pinpos*2));	//先清除原来的设置
			GPIOx->PUPDR|=PUPD<<(pinpos*2);	//设置新的上下拉
		}
	}
} 

void IoInit(GPIO_TypeDef* Portx,u32 Pinx,_IOMode mode,_IOPuPd pupd)
{
	u8 Pin = 0;
	Pinx >>= 1;
	while(Pinx > 0)
	{
		Pin++;
		Pinx >>= 1;
	}
	Portx->MODER &= ~(0x03 << (Pin << 1));
	Portx->OTYPER &= ~(0x01 << Pin);
	Portx->PUPDR &= ~(0x03 << (Pin << 1));
	Portx->OSPEEDR &= ~(3<<(Pin<<1));
	switch(pupd)
	{
		case NOPU://不上拉不下拉
			Portx->PUPDR &= ~(0x03 << (Pin << 1));
			break;
		
		case IOPU://上拉
			Portx->PUPDR |= (0x01 << (Pin << 1));
			break;
		
		case IOPD://下拉
			Portx->PUPDR |= (0x02 << (Pin << 1));
			break;
	}
	
	switch(mode)
	{
	case IOOUT:
		Portx->MODER |= (0x01 << (Pin << 1));
		Portx->OSPEEDR |= (0x01 << (Pin << 1));
	break;
	
	case IOIN:
		if(pupd == IOPU)
			Portx->ODR |= (0x01<<Pin);
		else if(pupd == IOPD)
			Portx->ODR &= ~(0x01<<Pin);
	break;
	case IOFUNC:
		Portx->MODER |= (0x02 << (Pin << 1));
		break;
	}
}

u8 Pin2Num(u32 Pinx)
{
	u8 i=0;
	Pinx>>=1;
	while(Pinx>0)
	{
		i++;
		Pinx >>= 1;
	}
	return i;
}

//冒泡排序
void mySort(u32 *buf,u16 len)
{
	u16 i,k;
	u32 temp;
	for(i=0;i<len-1;i++)
	{
		for(k = i+1;k < len;k ++)
		{
			if(buf[i] > buf[k])
			{
				temp = buf[i];
				buf[i] = buf[k];
				buf[k] = temp;
			}
		}
	}
}

void mySort_u8(u8 *buf,u16 len)
{
	u16 i,k;
	u8 temp;
	for(i=0;i<len-1;i++)
	{
		for(k = i+1;k < len;k ++)
		{
			if(buf[i] > buf[k])
			{
				temp = buf[i];
				buf[i] = buf[k];
				buf[k] = temp;
			}
		}
	}
}
