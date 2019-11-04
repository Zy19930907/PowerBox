#ifndef __TIMER_H
#define __TIMER_H

#include "Public.h"

typedef struct 
{
	u8 FirstFlag;
	u16 Cnt1;
	u16 Cnt2;
	u16 TimDiff;
	u16 Freq;
}_CapCh;

typedef struct
{
	_CapCh CAP1_FAC;//通道3，市电频率
	_CapCh CAP2_FFN;//通道4，风能频率
}_Capture;
extern _Capture Capture;

extern void CaptureInit(void);

#endif
