#ifndef __ADC_H
#define __ADC_H

#include "Public.h"
typedef struct
{
	GPIO_TypeDef* PORT;
	u32 PIN;
	u8 ch;
}_ADCCH;

typedef struct
{
	_ADCCH AD0_VAC;
	_ADCCH AD1_VFN;
	_ADCCH AD2_VTY;
	_ADCCH AD3_BAT;
	_ADCCH AD4_IF;
	_ADCCH AD5_IC;
	_ADCCH AD6_IC1;
}_ADCS;
extern _ADCS ADCS;

u16 Get_Adc(u8 ch);
extern void AdcInit(void);
extern u16 Get_Adc_Average(u8 ch,u8 times);
#endif
