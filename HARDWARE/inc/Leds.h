#ifndef __LEDS_H
#define __LEDS_H

#include "Public.h"

typedef struct{
	GPIO_TypeDef* PORT;
	u32 PIN;
	u32 HIGH;
	u32 LOW;
}_LEDIO;

typedef struct{
	_LEDIO LED0;
	_LEDIO LED1;
	_LEDIO LED2;
	_LEDIO LED3;
}_LEDS;
extern _LEDS LEDS;

#define LEDSIOCLKEN				RCC->AHBENR |= (0x09 << 2)

#define LED0CHANGE				LEDS.LED0.PORT->ODR ^= LEDS.LED0.PIN
#define LED0ON					LEDS.LED0.PORT->BSRRL = LEDS.LED0.PIN
#define LED0OFF					LEDS.LED0.PORT->BSRRH = LEDS.LED0.PIN

#define LED1CHANGE				LEDS.LED1.PORT->ODR ^= LEDS.LED1.PIN
#define LED1ON					LEDS.LED1.PORT->BSRRL = LEDS.LED1.PIN
#define LED1OFF					LEDS.LED1.PORT->BSRRH = LEDS.LED1.PIN

#define LED2CHANGE				LEDS.LED2.PORT->ODR ^= LEDS.LED2.PIN
#define LED2ON					LEDS.LED2.PORT->BSRRL = LEDS.LED2.PIN
#define LED2OFF					LEDS.LED2.PORT->BSRRH = LEDS.LED2.PIN

#define LED3CHANGE				LEDS.LED3.PORT->ODR ^= LEDS.LED3.PIN
#define LED3ON					LEDS.LED3.PORT->BSRRL = LEDS.LED3.PIN
#define LED3OFF					LEDS.LED3.PORT->BSRRH = LEDS.LED3.PIN

extern void LedsInit(void);

#endif
