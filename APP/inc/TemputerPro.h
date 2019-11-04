#ifndef __TEMPUTERPRO_H
#define __TEMPUTERPRO_H

#include "Public.h"

typedef struct{
	u8 val[2];
	u8 cnt;
	u8 flag;
}_Temputer;
extern _Temputer Temputer;
extern void TemputerPro(void);

#endif
