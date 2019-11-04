#ifndef __CONTROL_H
#define __CONTROL_H

#include "Public.h"
typedef struct
{
	GPIO_TypeDef* PORT;
	u32 PIN;
}_CTRIO;

typedef struct
{
	_CTRIO KAC;		//交流充电控制
	_CTRIO KJC;		//均充控制
	_CTRIO KFN;		//风能充电控制
	_CTRIO KVO;		//输出控制
	_CTRIO KJD;		//继电器控制
}_Ctrol;
extern _Ctrol Ctrol;

#define KACEN		Ctrol.KAC.PORT->BSRR = Ctrol.KAC.PIN
#define KACDIS		Ctrol.KAC.PORT->BSRR = (Ctrol.KAC.PIN<<16)

#define KJCEN		Ctrol.KJC.PORT->BSRR = Ctrol.KJC.PIN
#define KJCDIS		Ctrol.KJC.PORT->BSRR = (Ctrol.KJC.PIN<<16)

#define KFNEN		Ctrol.KFN.PORT->BSRR = Ctrol.KFN.PIN
#define KFNDIS		Ctrol.KFN.PORT->BSRR = (Ctrol.KFN.PIN<<16)

#define KVOEN		Ctrol.KVO.PORT->BSRR = (Ctrol.KVO.PIN<<16)
#define KVODIS		Ctrol.KVO.PORT->BSRR = Ctrol.KVO.PIN

#define KJDEN		Ctrol.KJD.PORT->BSRR = Ctrol.KJD.PIN
#define KJDDIS		Ctrol.KJD.PORT->BSRR = (Ctrol.KJD.PIN<<16)

extern void ControlIoInit(void);
extern void KJD_DIS(void);
extern void KJD_EN(void);
extern void KVO_DIS(void);
extern void KVO_EN(void);
extern void KFN_DIS(void);
extern void KFN_EN(void);
extern void KJC_DIS(void);
extern void KJC_EN(void);
extern void KAC_DIS(void);
extern void KAC_EN(void);

#endif
