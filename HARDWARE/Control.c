#include "Control.h"

_Ctrol Ctrol = {
	.KAC={GPIOC,PIN7},		//����������
	.KJC={GPIOC,PIN8},		//�������
	.KFN={GPIOC,PIN9},		//���ܳ�����
	.KVO={GPIOA,PIN11},		//�������
	.KJD={GPIOA,PIN12},		//�̵�������
};

void ControlIoInit(void)
{
	//������������Ϊ���ģʽ
	IoInit(Ctrol.KAC.PORT,Ctrol.KAC.PIN,IOOUT,NOPU);
	IoInit(Ctrol.KFN.PORT,Ctrol.KFN.PIN,IOOUT,NOPU);
	IoInit(Ctrol.KJC.PORT,Ctrol.KJC.PIN,IOOUT,NOPU);
	IoInit(Ctrol.KJD.PORT,Ctrol.KJD.PIN,IOOUT,NOPU);
	IoInit(Ctrol.KVO.PORT,Ctrol.KVO.PIN,IOOUT,NOPU);
	KACEN;
	KFNEN;
	KJCEN;
	//KJDDIS;
	KVOEN;
}

void KAC_EN(void)
{
	KACEN;
}

void KAC_DIS(void)
{
	KACDIS;
}

void KJC_EN(void)
{
	KJCEN;
}

void KJC_DIS(void)
{
	KJCDIS;
}

void KFN_EN(void)
{
	KFNEN;
}

void KFN_DIS(void)
{
	KFNDIS;
}

void KVO_EN(void)
{
	KVOEN;
}

void KVO_DIS(void)
{
	KACDIS;
}

void KJD_EN(void)
{
	KJDEN;
}

void KJD_DIS(void)
{
	KJDDIS;
}
