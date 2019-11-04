#include "Uart.h"

u8 UART1BUF[MAXUARTBUFLENN];
u8 UART2BUF[MAXUARTBUFLENN];

void Uart1Init(u32 baud)
{
	float temp = 500000;
	u16 mantissa;
	u16 fraction;	   
	temp/=baud;//得到USARTDIV@OVER8=0
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分@OVER8=0 
    mantissa<<=4;
	mantissa+=fraction; 
	
	RCC->AHBENR|=1<<1;   	//使能PB口时钟 
	RCC->APB2RSTR |= 1<<14;
	RCC->APB2RSTR &= ~(1<<14);
	delay_ms(5);
	RCC->APB2ENR|=1<<14;  	//使能串口1时钟 
	
	GPIO_Set(GPIOB,PIN6|PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);//PA9,PA10,复用功能,上拉输出
 	GPIO_AF_Set(GPIOB,6,7);	//PB6,AF7
	GPIO_AF_Set(GPIOB,7,7);//PB7,AF7  	   
	//波特率设置
 	USART1->BRR=mantissa; 	//波特率设置	 
	USART1->CR1&=~(1<<15); 	//设置OVER8=0 
	USART1->CR1|=1<<3;  	//串口发送使能 
	//使能接收中断 
	USART1->CR1|=1<<2;  	//串口接收使能
	USART1->CR1 |= 1<<4;    //空闲中断	   	    	
	MY_NVIC_Init(0,14,USART1_IRQn,0);
	USART1->CR1|=1<<13;  	//串口使能
	
	RCC->AHBENR |= 1 << 24; //开启DMA1时钟
	DMA1_Channel4->CPAR = (u32)&(USART1->DR);  	//外设地址
	DMA1_Channel4->CCR = 0x00000000;         	//复位
	DMA1_Channel4->CCR |=  (1<<4);             	//传输方向，从存储器读数据输出到外设
	DMA1_Channel4->CCR &= ~(1<<5);             	//不执行循环操作
	DMA1_Channel4->CCR &= ~(1<<6);             	//外设地址不递增
	DMA1_Channel4->CCR |=  (1<<7);            	//存储器地址递增
	DMA1_Channel4->CCR &= ~(3<<8);            	//外设数据宽度8BIT
	DMA1_Channel4->CCR &= ~(3<<10);           	//存储数据宽度8bit
	DMA1_Channel4->CCR |=  (1<<12);           	//中等优先级
	DMA1_Channel4->CCR &= ~(1<<14);            	//非存储器到存储器 
	
	DMA1_Channel5->CCR &= ~0x00000001;//关闭数据流
	DMA1_Channel5->CCR |= (0x03 << 22);//优先级高
	DMA1_Channel5->CCR |= (1 << 7);//存储器地址递增
	DMA1_Channel5->CCR &= ~(0x01 << 4);//传输方向：外设到存储器
	DMA1_Channel5->CPAR = (u32)&USART1->DR;//从串口数据寄存器读取数据
	DMA1_Channel5->CMAR = (u32)UART1BUF;//数据搬移目的地址
	DMA1_Channel5->CNDTR = MAXUARTBUFLENN;
	
	USART1->CR3 |= 0xC0; //使能UART1 DMA收发
	DMA1_Channel5->CCR |= 0x00000001;//开启数据流
}

void Uart2Init(u32 baud)
{
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)((float)2000000/(float)baud);	//得到USARTDIV@OVER8=0
	mantissa=temp;				 				//得到整数部分
	fraction=(temp-mantissa)*16; 				//得到小数部分@OVER8=0 
    mantissa<<=4;
	mantissa+=fraction;
	
	RCC->AHBENR|=1<<0;   	//使能PORTA口时钟  
	RCC->APB1ENR|=1<<17;  	//使能串口2时钟 
	GPIO_Set(GPIOA,PIN2|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);//PA9,PA10,复用功能,上拉输出
 	GPIO_AF_Set(GPIOA,2,7);	//PA2,AF7
	GPIO_AF_Set(GPIOA,3,7);//PA3,AF7
	//波特率设置
 	USART2->BRR=mantissa; 	//波特率设置	 
	USART2->CR1&=~(1<<15); 	//设置OVER8=0 
	USART2->CR1|=1<<3;  	//串口发送使能 
	//使能接收中断 
	USART2->CR1|=1<<2;  	//串口接收使能
	USART2->CR1 |= 1<<4;    //空闲中断	   	    	
	MY_NVIC_Init(0,13,USART2_IRQn,0);
	USART2->CR1|=1<<13;  	//串口使能
	RCC->AHBENR |= 1 << 24; //开启DMA1时钟
	USART2->CR3 |= 0xC0; //使能UART1 DMA收发
	DMA1_Channel7->CPAR = (u32)&(USART2->DR);  	//外设地址
	DMA1_Channel7->CCR = 0x00000000;         	//复位
	DMA1_Channel7->CCR |=  (1<<4);             	//传输方向，从存储器读数据输出到外设
	DMA1_Channel7->CCR &= ~(1<<5);             	//不执行循环操作
	DMA1_Channel7->CCR &= ~(1<<6);             	//外设地址不递增
	DMA1_Channel7->CCR |=  (1<<7);            	//存储器地址递增
	DMA1_Channel7->CCR &= ~(3<<8);            	//外设数据宽度8BIT
	DMA1_Channel7->CCR &= ~(3<<10);           	//存储数据宽度8bit
	DMA1_Channel7->CCR |=  (1<<12);           	//中等优先级
	DMA1_Channel7->CCR &= ~(1<<14);            	//非存储器到存储器 
	
	DMA1_Channel6->CCR &= ~0x00000001;//关闭数据流
	DMA1_Channel6->CCR |= (0x03 << 22);//优先级高
	DMA1_Channel6->CCR |= (0x01 << 7);//存储器地址递增
	DMA1_Channel6->CCR &= ~(0x00 << 4);//传输方向：外设到存储器
	DMA1_Channel6->CPAR = (u32)&USART2->DR;//从串口数据寄存器读取数据
	DMA1_Channel6->CMAR = (u32)UART2BUF;//数据搬移目的地址
	DMA1_Channel6->CNDTR = MAXUARTBUFLENN;
	DMA1_Channel6->CCR |= 0x01;//开启数据流
}

//串口1接收中断
void USART1_IRQHandler(void)
{
	u16 temp;
	if(USART1 -> SR & 0x10)    // 空闲中断 , 先关闭DMA通道，清零计数，在打开
	{
		temp = MAXUARTBUFLENN - DMA1_Channel5->CNDTR;//串口接收数据个数
		Uart1SendData(UART1BUF,temp);
		temp = USART1->SR;
		temp |= USART1->DR;
		DMA1->IFCR |= 0x0F << 16;//清除DMA标记
		DMA1_Channel5->CCR &= ~0x01;
		DMA1_Channel5->CNDTR = MAXUARTBUFLENN;
		DMA1_Channel5->CCR |= 0x01;
	}
}

//串口2接收中断
void USART2_IRQHandler(void)
{
	u16 temp;
	if(USART2 -> SR & 0x10)    // 空闲中断 , 先关闭DMA通道，清零计数，在打开
	{
		temp = MAXUARTBUFLENN - DMA1_Channel6->CNDTR;//串口接收数据个数
		temp = USART2->SR;
		temp |= USART2->DR;
		DMA1->IFCR |= 0x0F << 20;//清除DMA标记
		DMA1_Channel6->CCR &= ~0x01;
		DMA1_Channel6->CNDTR = MAXUARTBUFLENN;
		DMA1_Channel6->CCR |= 0x01;
	}
}

void Uart1SendDat(u8 *buf,u16 len)
{
	DMA1->IFCR = (0x0F << 12);
	DMA1_Channel4->CCR &= ~(1<<0);
	DMA1_Channel4->CMAR = (u32)buf;
	DMA1_Channel4->CNDTR = len;
	DMA1_Channel4->CCR |= (1<<0);
}

void Uart1SendBuf(u8 *buf)
{
	u16 len = strlen((char*)buf);
	DMA1->IFCR = (0x0F << 12);
	DMA1_Channel4->CCR &= ~(1<<0);
	DMA1_Channel4->CMAR = (u32)buf;
	DMA1_Channel4->CNDTR = len;
	DMA1_Channel4->CCR |= (1<<0);
}

void Uart1SendData(u8 *buf,u16 len)
{
	DMA1->IFCR = (0x0F << 12);
	DMA1_Channel4->CCR &= ~(1<<0);
	DMA1_Channel4->CMAR = (u32)buf;
	DMA1_Channel4->CNDTR = len;
	DMA1_Channel4->CCR |= 0x01;
}

void Uart2SendBuf(u8 *buf)
{
	u16 len = strlen((char*)buf);
	DMA1->IFCR = (0x0F << 24);
	DMA1_Channel7->CCR &= ~(1<<0);
	DMA1_Channel7->CMAR = (u32)buf;
	DMA1_Channel7->CNDTR = len;
	DMA1_Channel7->CCR |= (1<<0);
}

void Uart2SendData(u8 *buf,u16 len)
{
	DMA1->IFCR = (0x0F << 24);
	DMA1_Channel7->CCR &= ~(1<<0);
	DMA1_Channel7->CMAR = (u32)buf;
	DMA1_Channel7->CNDTR = len;
	DMA1_Channel7->CCR |= (1<<0);
}
