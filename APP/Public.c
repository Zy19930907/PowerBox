#include "Public.h"

//�ⲿ�ж����ú���
//ֻ���GPIOA~I;������PVD,RTC,USB_OTG,USB_HS,��̫�����ѵ�
//����:
//GPIOx:0~8,����GPIOA~I
//BITx:��Ҫʹ�ܵ�λ;
//TRIM:����ģʽ,1,������;2,�Ͻ���;3�������ƽ����
//�ú���һ��ֻ������1��IO��,���IO��,���ε���
//�ú������Զ�������Ӧ�ж�,�Լ�������   	    
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{ 
	u8 EXTOFFSET=(BITx%4)*4;  
	RCC->APB2ENR|=1<<14;  						//ʹ��SYSCFGʱ��  
	SYSCFG->EXTICR[BITx/4]&=~(0x000F<<EXTOFFSET);//���ԭ�����ã�����
	SYSCFG->EXTICR[BITx/4]|=GPIOx<<EXTOFFSET;	//EXTI.BITxӳ�䵽GPIOx.BITx 
	//�Զ�����
	EXTI->IMR|=1<<BITx;					//����line BITx�ϵ��ж�(���Ҫ��ֹ�жϣ��򷴲�������)
	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;	//line BITx���¼��½��ش���
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;	//line BITx���¼��������ش���
} 	

void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx)
{  
	GPIOx->AFR[BITx>>3]&=~(0X0F<<((BITx&0X07)*4));
	GPIOx->AFR[BITx>>3]|=(u32)AFx<<((BITx&0X07)*4);
}   

//GPIOͨ������ 
//GPIOx:GPIOA~GPIOI.
//BITx:0X0000~0XFFFF,λ����,ÿ��λ����һ��IO,��0λ����Px0,��1λ����Px1,��������.����0X0101,����ͬʱ����Px0��Px8.
//MODE:0~3;ģʽѡ��,0,����(ϵͳ��λĬ��״̬);1,��ͨ���;2,���ù���;3,ģ������.
//OTYPE:0/1;�������ѡ��,0,�������;1,��©���.
//OSPEED:0~3;����ٶ�����,0,2Mhz;1,25Mhz;2,50Mhz;3,100Mh. 
//PUPD:0~3:����������,0,����������;1,����;2,����;3,����.
//ע��:������ģʽ(��ͨ����/ģ������)��,OTYPE��OSPEED������Ч!!
void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD)
{  
	u32 pinpos=0,pos=0,curpin=0;
	for(pinpos=0;pinpos<16;pinpos++)
	{
		pos=1<<pinpos;	//һ����λ��� 
		curpin=BITx&pos;//��������Ƿ�Ҫ����
		if(curpin==pos)	//��Ҫ����
		{
			GPIOx->MODER&=~(3<<(pinpos*2));	//�����ԭ��������
			GPIOx->MODER|=MODE<<(pinpos*2);	//�����µ�ģʽ 
			if((MODE==0X01)||(MODE==0X02))	//��������ģʽ/���ù���ģʽ
			{  
				GPIOx->OSPEEDR&=~(3<<(pinpos*2));	//���ԭ��������
				GPIOx->OSPEEDR|=(OSPEED<<(pinpos*2));//�����µ��ٶ�ֵ  
				GPIOx->OTYPER&=~(1<<pinpos) ;		//���ԭ��������
				GPIOx->OTYPER|=OTYPE<<pinpos;		//�����µ����ģʽ
			}  
			GPIOx->PUPDR&=~(3<<(pinpos*2));	//�����ԭ��������
			GPIOx->PUPDR|=PUPD<<(pinpos*2);	//�����µ�������
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
		case NOPU://������������
			Portx->PUPDR &= ~(0x03 << (Pin << 1));
			break;
		
		case IOPU://����
			Portx->PUPDR |= (0x01 << (Pin << 1));
			break;
		
		case IOPD://����
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

//ð������
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
