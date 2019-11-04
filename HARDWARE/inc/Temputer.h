#ifndef __TEMPUTER_H
#define __TEMPUTER_H

#include "Public.h"

#define TEMPSIOCLKEN				RCC->AHBENR |= (0x09 << 2)
#define GETTEMPIOVALUES				((GPIOC->IDR >> 13) & 0x03)

typedef struct{
	GPIO_TypeDef* PORT;
	u32 PIN;
	u32 HIGH;
	u32 LOW;
}_TEMPIO;

typedef struct{
	_TEMPIO TEMP1;
	_TEMPIO TEMP2;
	_TEMPIO TEMP3;
}_TEMPIOS;
extern _TEMPIOS TEMPIOS;

extern void TempIosInit(void);


#endif
