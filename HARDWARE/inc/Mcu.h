#ifndef __MCU_H
#define __MCU_H

#include "public.h"

extern void McuInit(void);
extern void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);
extern void delay_ms(u64 nms);

#endif
