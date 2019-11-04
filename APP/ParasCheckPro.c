#include "ParasCheckPro.h"

extern u8 volComplt;

_ParasChecker ParasChecker = {
	.errflag = 0,
	.chargeflag = 0,
	.volerrflag = 0,
	.currentflag = 0,
	.inoutflag = 0,
};

//�ж��е������Ƿ�����
void CheckVac(void)
{
	if(Vol.VAC > 240)
		ParasChecker.errflag |=  0x08;		//����е��ѹ��E4��
	else
		ParasChecker.errflag &= ~0x08;		//����е��ѹ��E4��
	
	if(Vol.VAC < 200)
		ParasChecker.errflag |=  0x10;		//����е�Ƿѹ��E5��
	else
		ParasChecker.errflag &= ~0x10;		//����е�Ƿѹ��E5��
	
	if((Vol.VAC > VACMAX) || (Vol.VAC < VACMIN))
		ParasChecker.volerrflag |= 0x01;	//����е������쳣
	if((Vol.VAC < (VACMAX-10)) && (Vol.VAC > (VACMIN+10)))
		ParasChecker.volerrflag &= ~0x01;
}

//�жϷ��������Ƿ��쳣
void CheckVfn(void)
{
	if(Vol.VFN > 28)
		ParasChecker.errflag |=  0x20;		//��Ƿ��ܹ�ѹ��E6��
	else
		ParasChecker.errflag &= ~0x20;		//������ܹ�ѹ���
	
	if(Vol.VFN < 22)
		ParasChecker.errflag |=  0x40;		//��Ƿ���Ƿѹ��E7��
	else
		ParasChecker.errflag &= ~0x40;		//�������Ƿѹ���
	
	if((Vol.VFN < VFNMAX) && (Vol.VFN > VFNMIN))
		ParasChecker.volerrflag &= ~0x02;	//��������쳣���
	else
		ParasChecker.volerrflag |= 0x02;	//��Ƿ��������쳣
}

//�ж�̫���������Ƿ�����
void CheckVty(void)
{
	if(Vol.VTY > 38)
		ParasChecker.errflag |=  0x80;		//���̫���ܹ�ѹ��E8��
	else
		ParasChecker.errflag &= ~0x80;		//���̫���ܹ�ѹ���
	
	if(Vol.VTY < 28)
		ParasChecker.errflag |=  0x100;		//���̫����Ƿѹ��E9��
	else
		ParasChecker.errflag &= ~0x100;		//���̫����Ƿѹ���

	if((Vol.VTY < VTYMAX) && (Vol.VTY > VTYMIN))
		ParasChecker.volerrflag &= ~0x04;	//���̫�����쳣���
	else
		ParasChecker.volerrflag |= 0x04;	//���̫���������쳣
}

//�жϵ�ص�ѹ�Ƿ�����
void CheckBat(void)
{
	if(Vol.BAT > VBATMAX)
	{
		ParasChecker.volerrflag |= 0x08;	//��ǵ�ص�ѹ�쳣
		ParasChecker.errflag |=  0x01;		//��ǵ�ع�ѹ��E1��
	}
	else if((Vol.BAT < VBATMIN))
	{
		ParasChecker.volerrflag |= 0x08;	//��ǵ�ص�ѹ�쳣
		ParasChecker.errflag |=  0x02;		//��ǵ��Ƿѹ��E2��
	}
	
	if(Vol.BAT < VBATUPRES)
		ParasChecker.errflag &= ~0x01;		//�����ع�ѹ���
	if(Vol.BAT > VBATDOWNRES && (!(ParasChecker.volerrflag & 0x01)))
		ParasChecker.errflag &= ~0x02;		//������Ƿѹ���
	if((Vol.BAT < VBATUPRES) && (Vol.BAT > VBATDOWNRES))
		ParasChecker.volerrflag &= ~0x08;	//�������쳣���
	
	Vol.VBAT = Vol.BAT;
	//��ŵ��ص�ѹ����
	Vol.VBAT -= (Vol.IC / VOLREPAIRC);
	if((Vol.IC > 1) && (Vol.IF > 5))
		Vol.VBAT += (Vol.IF / VOLREPAIRCF);
	else if((Vol.IC < 1) && (Vol.IF > 5))
		Vol.VBAT += (Vol.IF / VOLREPAIRF);
	else 
		Vol.VBAT += (Vol.IF / 7.45);
	
	//�����ص���
	if(Vol.VBAT >= BATFULL)
		ParasChecker.batpercent = 5;
	else if(Vol.VBAT > BATNONE)
		ParasChecker.batpercent = ((Vol.VBAT - BATNONE) / 1.5);
	else
		ParasChecker.batpercent = 0;
}

//�жϷŵ�����Ƿ�����
void CheckIf(void)
{
	if(Vol.IF >= CUMAX)
	{
		ParasChecker.currentflag |= 0x01;
		ParasChecker.errflag |= 0x04;
	}
}

void CheckIc(void)
{
//	if(Vol.IC > 1.2)
//		ParasChecker.currentflag |=  0x02;
//	else if(Vol.IC < 0.8)
//		ParasChecker.currentflag &= ~0x02;
}

void ParasCheckPro(void)
{
	switch(ParasChecker.Status)
	{
		case WAITVOLCOMPLETE:
			if(Vol.complate)
				ParasChecker.Status = DOCHECK;
			break;
		//����������Ƿ�����
		case DOCHECK:
			if(volComplt)
			{
				volComplt = 0;
				CheckVac();
				CheckVfn();
				CheckVty();
				CheckBat();
				CheckIf();
				CheckIc();
			}
			break;
	}
}
