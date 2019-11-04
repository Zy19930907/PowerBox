#ifndef __VOLPRO_H
#define __VOLPRO_H

#include "Public.h"
#define MAXVOLCNT		20
#define VOLOFFSET		5

typedef enum
{
	AD0_VAC,
	AD1_VFN,
	AD2_VTY,
	AD3_BAT,
	AD4_IF,
	AD5_IC,
	AD6_IC1,
}_ADCH;

typedef struct
{
	_ADCH CurCh;
	u8 VolCnt;
	u32 VolTemp;
	u32 AD_Temps[MAXVOLCNT];
	u16 ADS[7];

	u8 VAC;
	float VFN;
	float VTY;
	float BAT;
	float VBAT;
	float IF;
	float IC;
	float IC1;
	u16 BatInPower;
	u16 BatOutPower;

	u8 cnt;
	u8 complate;
}_Vol;
extern _Vol Vol;
extern void VolPro(void);

#endif

