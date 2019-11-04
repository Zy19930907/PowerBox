#include "Uart.h"

u8 UART1BUF[MAXUARTBUFLENN];
u8 UART2BUF[MAXUARTBUFLENN];

void Uart1Init(u32 baud)
{
	float temp = 500000;
	u16 mantissa;
	u16 fraction;	   
	temp/=baud;//�õ�USARTDIV@OVER8=0
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������@OVER8=0 
    mantissa<<=4;
	mantissa+=fraction; 
	
	RCC->AHBENR|=1<<1;   	//ʹ��PB��ʱ�� 
	RCC->APB2RSTR |= 1<<14;
	RCC->APB2RSTR &= ~(1<<14);
	delay_ms(5);
	RCC->APB2ENR|=1<<14;  	//ʹ�ܴ���1ʱ�� 
	
	GPIO_Set(GPIOB,PIN6|PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);//PA9,PA10,���ù���,�������
 	GPIO_AF_Set(GPIOB,6,7);	//PB6,AF7
	GPIO_AF_Set(GPIOB,7,7);//PB7,AF7  	   
	//����������
 	USART1->BRR=mantissa; 	//����������	 
	USART1->CR1&=~(1<<15); 	//����OVER8=0 
	USART1->CR1|=1<<3;  	//���ڷ���ʹ�� 
	//ʹ�ܽ����ж� 
	USART1->CR1|=1<<2;  	//���ڽ���ʹ��
	USART1->CR1 |= 1<<4;    //�����ж�	   	    	
	MY_NVIC_Init(0,14,USART1_IRQn,0);
	USART1->CR1|=1<<13;  	//����ʹ��
	
	RCC->AHBENR |= 1 << 24; //����DMA1ʱ��
	DMA1_Channel4->CPAR = (u32)&(USART1->DR);  	//�����ַ
	DMA1_Channel4->CCR = 0x00000000;         	//��λ
	DMA1_Channel4->CCR |=  (1<<4);             	//���䷽�򣬴Ӵ洢�����������������
	DMA1_Channel4->CCR &= ~(1<<5);             	//��ִ��ѭ������
	DMA1_Channel4->CCR &= ~(1<<6);             	//�����ַ������
	DMA1_Channel4->CCR |=  (1<<7);            	//�洢����ַ����
	DMA1_Channel4->CCR &= ~(3<<8);            	//�������ݿ��8BIT
	DMA1_Channel4->CCR &= ~(3<<10);           	//�洢���ݿ��8bit
	DMA1_Channel4->CCR |=  (1<<12);           	//�е����ȼ�
	DMA1_Channel4->CCR &= ~(1<<14);            	//�Ǵ洢�����洢�� 
	
	DMA1_Channel5->CCR &= ~0x00000001;//�ر�������
	DMA1_Channel5->CCR |= (0x03 << 22);//���ȼ���
	DMA1_Channel5->CCR |= (1 << 7);//�洢����ַ����
	DMA1_Channel5->CCR &= ~(0x01 << 4);//���䷽�����赽�洢��
	DMA1_Channel5->CPAR = (u32)&USART1->DR;//�Ӵ������ݼĴ�����ȡ����
	DMA1_Channel5->CMAR = (u32)UART1BUF;//���ݰ���Ŀ�ĵ�ַ
	DMA1_Channel5->CNDTR = MAXUARTBUFLENN;
	
	USART1->CR3 |= 0xC0; //ʹ��UART1 DMA�շ�
	DMA1_Channel5->CCR |= 0x00000001;//����������
}

void Uart2Init(u32 baud)
{
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)((float)2000000/(float)baud);	//�õ�USARTDIV@OVER8=0
	mantissa=temp;				 				//�õ���������
	fraction=(temp-mantissa)*16; 				//�õ�С������@OVER8=0 
    mantissa<<=4;
	mantissa+=fraction;
	
	RCC->AHBENR|=1<<0;   	//ʹ��PORTA��ʱ��  
	RCC->APB1ENR|=1<<17;  	//ʹ�ܴ���2ʱ�� 
	GPIO_Set(GPIOA,PIN2|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);//PA9,PA10,���ù���,�������
 	GPIO_AF_Set(GPIOA,2,7);	//PA2,AF7
	GPIO_AF_Set(GPIOA,3,7);//PA3,AF7
	//����������
 	USART2->BRR=mantissa; 	//����������	 
	USART2->CR1&=~(1<<15); 	//����OVER8=0 
	USART2->CR1|=1<<3;  	//���ڷ���ʹ�� 
	//ʹ�ܽ����ж� 
	USART2->CR1|=1<<2;  	//���ڽ���ʹ��
	USART2->CR1 |= 1<<4;    //�����ж�	   	    	
	MY_NVIC_Init(0,13,USART2_IRQn,0);
	USART2->CR1|=1<<13;  	//����ʹ��
	RCC->AHBENR |= 1 << 24; //����DMA1ʱ��
	USART2->CR3 |= 0xC0; //ʹ��UART1 DMA�շ�
	DMA1_Channel7->CPAR = (u32)&(USART2->DR);  	//�����ַ
	DMA1_Channel7->CCR = 0x00000000;         	//��λ
	DMA1_Channel7->CCR |=  (1<<4);             	//���䷽�򣬴Ӵ洢�����������������
	DMA1_Channel7->CCR &= ~(1<<5);             	//��ִ��ѭ������
	DMA1_Channel7->CCR &= ~(1<<6);             	//�����ַ������
	DMA1_Channel7->CCR |=  (1<<7);            	//�洢����ַ����
	DMA1_Channel7->CCR &= ~(3<<8);            	//�������ݿ��8BIT
	DMA1_Channel7->CCR &= ~(3<<10);           	//�洢���ݿ��8bit
	DMA1_Channel7->CCR |=  (1<<12);           	//�е����ȼ�
	DMA1_Channel7->CCR &= ~(1<<14);            	//�Ǵ洢�����洢�� 
	
	DMA1_Channel6->CCR &= ~0x00000001;//�ر�������
	DMA1_Channel6->CCR |= (0x03 << 22);//���ȼ���
	DMA1_Channel6->CCR |= (0x01 << 7);//�洢����ַ����
	DMA1_Channel6->CCR &= ~(0x00 << 4);//���䷽�����赽�洢��
	DMA1_Channel6->CPAR = (u32)&USART2->DR;//�Ӵ������ݼĴ�����ȡ����
	DMA1_Channel6->CMAR = (u32)UART2BUF;//���ݰ���Ŀ�ĵ�ַ
	DMA1_Channel6->CNDTR = MAXUARTBUFLENN;
	DMA1_Channel6->CCR |= 0x01;//����������
}

//����1�����ж�
void USART1_IRQHandler(void)
{
	u16 temp;
	if(USART1 -> SR & 0x10)    // �����ж� , �ȹر�DMAͨ��������������ڴ�
	{
		temp = MAXUARTBUFLENN - DMA1_Channel5->CNDTR;//���ڽ������ݸ���
		Uart1SendData(UART1BUF,temp);
		temp = USART1->SR;
		temp |= USART1->DR;
		DMA1->IFCR |= 0x0F << 16;//���DMA���
		DMA1_Channel5->CCR &= ~0x01;
		DMA1_Channel5->CNDTR = MAXUARTBUFLENN;
		DMA1_Channel5->CCR |= 0x01;
	}
}

//����2�����ж�
void USART2_IRQHandler(void)
{
	u16 temp;
	if(USART2 -> SR & 0x10)    // �����ж� , �ȹر�DMAͨ��������������ڴ�
	{
		temp = MAXUARTBUFLENN - DMA1_Channel6->CNDTR;//���ڽ������ݸ���
		temp = USART2->SR;
		temp |= USART2->DR;
		DMA1->IFCR |= 0x0F << 20;//���DMA���
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
