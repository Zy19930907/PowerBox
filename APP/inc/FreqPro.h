#ifndef __FREQPRO_H
#define __FREQPRO_H

#include "Public.h"
#define FREQTEMPCNT			30
#define FREQOFFSET			10
typedef enum
{
	FREQINIT,
	FREQIDLE,
	FREQCAPED,
}_FreqStatus;

typedef struct
{
	_FreqStatus	Status;
	u32 Disp_Freq;
	u8 freq;
	u8 freqtemp[FREQTEMPCNT];
	u16 total;
	u8 cnt;
}_Freq;
extern _Freq Freq;

extern void FreqPro(void);

#endif
