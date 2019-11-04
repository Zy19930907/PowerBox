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
	_CapCh CAP1_FAC;//ͨ��3���е�Ƶ��
	_CapCh CAP2_FFN;//ͨ��4������Ƶ��
}_Capture;
extern _Capture Capture;

extern void CaptureInit(void);

#endif
