#include "Keys.h"

void KeysInit(void)
{
	RCC->AHBENR |= 0x01;//使能PA时钟
	GPIOA->MODER &= 0xFFFFFFF0;//清除PA0、1原有配置
	GPIOA->PUPDR |= 0x05;//上拉
}
