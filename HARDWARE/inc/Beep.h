#ifndef __BEEP_H
#define __BEEP_H

#include "Public.h"

#define	BEEPPORT			GPIOC
#define BEEPPIN				PIN10

#define BEEPCHANGE			BEEPPORT->ODR ^= BEEPPIN
#define BEEPON					BEEPPORT->BSRR = (BEEPPIN << 16)
#define BEEPOFF				BEEPPORT->BSRR = BEEPPIN

#define BEEPIOCLKEN			RCC->AHBENR |= 1<<2

extern void BeepInit(void);

#endif
