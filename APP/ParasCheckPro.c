#include "ParasCheckPro.h"

extern u8 volComplt;

_ParasChecker ParasChecker = {
	.errflag = 0,
	.chargeflag = 0,
	.volerrflag = 0,
	.currentflag = 0,
	.inoutflag = 0,
};

//判断市电输入是否正常
void CheckVac(void)
{
	if(Vol.VAC > 240)
		ParasChecker.errflag |=  0x08;		//标记市电过压（E4）
	else
		ParasChecker.errflag &= ~0x08;		//清除市电过压（E4）
	
	if(Vol.VAC < 200)
		ParasChecker.errflag |=  0x10;		//标记市电欠压（E5）
	else
		ParasChecker.errflag &= ~0x10;		//清除市电欠压（E5）
	
	if((Vol.VAC > VACMAX) || (Vol.VAC < VACMIN))
		ParasChecker.volerrflag |= 0x01;	//标记市电输入异常
	if((Vol.VAC < (VACMAX-10)) && (Vol.VAC > (VACMIN+10)))
		ParasChecker.volerrflag &= ~0x01;
}

//判断风能输入是否异常
void CheckVfn(void)
{
	if(Vol.VFN > 28)
		ParasChecker.errflag |=  0x20;		//标记风能过压（E6）
	else
		ParasChecker.errflag &= ~0x20;		//清除风能过压标记
	
	if(Vol.VFN < 22)
		ParasChecker.errflag |=  0x40;		//标记风能欠压（E7）
	else
		ParasChecker.errflag &= ~0x40;		//清除风能欠压标记
	
	if((Vol.VFN < VFNMAX) && (Vol.VFN > VFNMIN))
		ParasChecker.volerrflag &= ~0x02;	//清除风能异常标记
	else
		ParasChecker.volerrflag |= 0x02;	//标记风能输入异常
}

//判断太阳能输入是否正常
void CheckVty(void)
{
	if(Vol.VTY > 38)
		ParasChecker.errflag |=  0x80;		//标记太阳能过压（E8）
	else
		ParasChecker.errflag &= ~0x80;		//清除太阳能过压标记
	
	if(Vol.VTY < 28)
		ParasChecker.errflag |=  0x100;		//标记太阳能欠压（E9）
	else
		ParasChecker.errflag &= ~0x100;		//清除太阳能欠压标记

	if((Vol.VTY < VTYMAX) && (Vol.VTY > VTYMIN))
		ParasChecker.volerrflag &= ~0x04;	//清除太阳能异常标记
	else
		ParasChecker.volerrflag |= 0x04;	//标记太阳能输入异常
}

//判断电池电压是否正常
void CheckBat(void)
{
	if(Vol.BAT > VBATMAX)
	{
		ParasChecker.volerrflag |= 0x08;	//标记电池电压异常
		ParasChecker.errflag |=  0x01;		//标记电池过压（E1）
	}
	else if((Vol.BAT < VBATMIN))
	{
		ParasChecker.volerrflag |= 0x08;	//标记电池电压异常
		ParasChecker.errflag |=  0x02;		//标记电池欠压（E2）
	}
	
	if(Vol.BAT < VBATUPRES)
		ParasChecker.errflag &= ~0x01;		//清除电池过压标记
	if(Vol.BAT > VBATDOWNRES && (!(ParasChecker.volerrflag & 0x01)))
		ParasChecker.errflag &= ~0x02;		//清除电池欠压标记
	if((Vol.BAT < VBATUPRES) && (Vol.BAT > VBATDOWNRES))
		ParasChecker.volerrflag &= ~0x08;	//清除电池异常标记
	
	Vol.VBAT = Vol.BAT;
	//充放电电池电压修正
	Vol.VBAT -= (Vol.IC / VOLREPAIRC);
	if((Vol.IC > 1) && (Vol.IF > 5))
		Vol.VBAT += (Vol.IF / VOLREPAIRCF);
	else if((Vol.IC < 1) && (Vol.IF > 5))
		Vol.VBAT += (Vol.IF / VOLREPAIRF);
	else 
		Vol.VBAT += (Vol.IF / 7.45);
	
	//计算电池电量
	if(Vol.VBAT >= BATFULL)
		ParasChecker.batpercent = 5;
	else if(Vol.VBAT > BATNONE)
		ParasChecker.batpercent = ((Vol.VBAT - BATNONE) / 1.5);
	else
		ParasChecker.batpercent = 0;
}

//判断放电电流是否正常
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
		//检查各项参数是否正常
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
