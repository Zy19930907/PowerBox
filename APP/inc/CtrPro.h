#ifndef __CTRPRO_H
#define __CTRPRO_H

#include "Public.h"

#define VFNMIN			18
#define VFNRECHARGE		28
#define VTYMIN			24
#define BATMIN			50

#define MAINSMASK		0x10
#define CHARGEMASK		0x08
#define VFNMASK			0x04
#define	OUTPUTMASK		0x02
#define RELAYMASK		0x01

typedef enum
{
	CTRINIT,
	CTRWAIT,
	DOCTR,
}_CtrStatus;

extern void CtrPro(void);
extern void chargeCtr(void);

#endif
