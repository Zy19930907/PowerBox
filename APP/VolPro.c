#include "VolPro.h"

extern u8 volComplt;
_Vol Vol = {
	.ADS = { 0, 0, 0, 0, 0, 0 },
	.complate = 0,
	.cnt = 0,
};

float temp;
void CalculatedAverage(void) 
{
	u8 i;
	mySort(Vol.AD_Temps, MAXVOLCNT);										//升序排列AD值
	Vol.VolTemp = 0;
	for (i = VOLOFFSET; i < MAXVOLCNT - VOLOFFSET; i++)			//两端AD值不计算
		Vol.VolTemp += Vol.AD_Temps[i];
	Vol.VolTemp /= (MAXVOLCNT - (VOLOFFSET << 1));
	Vol.ADS[Vol.CurCh] = Vol.VolTemp;

	switch (Vol.CurCh) 
	{
	case AD0_VAC://计算市电输入电压
		temp = ((((float) Vol.ADS[0]) / 4096) * 3.3);
		if(temp<0.4)
		{
			Vol.VAC = 0;
			Capture.CAP1_FAC.Freq = 0;
		}
		else
		{
			temp /= 2.96;
			temp *= 32.672;
			temp *= 8.8;		//变压器倍数
			Vol.VAC = temp+4;
		}
		break;
	case AD1_VFN://计算风能输入电压
		temp = ((((float) Vol.ADS[1]) / 4096) * 3.275);
		temp /= 3;
		temp *= 56.5;
		Vol.VFN = temp;
		if(Vol.VFN <= 16)
			Capture.CAP2_FFN.Freq = 0;
		else
			Capture.CAP2_FFN.Freq = (Vol.VFN / 5) + 45;
		break;
	case AD2_VTY://计算太阳能输入电压
		temp = ((((float) Vol.ADS[2]) / 4096) * 3.275);
		temp /= 3;
		temp *= 50.16;
		Vol.VTY = temp;
		break;
	case AD3_BAT://计算电池电压
		temp = ((((float) Vol.ADS[3]) / 4096) * 3.275);
		temp /= 3;
		temp *= BATPARA;
		Vol.BAT = temp;
		break;
	case AD4_IF://计算放电电流
		temp = ((((float) Vol.ADS[4]) / 4096) * 3.275);
		temp /= 0.056;
		Vol.IF = temp;
		Vol.BatOutPower = Vol.IF * Vol.BAT;
		break;
	case AD5_IC://计算充电电流
		temp = ((((float) Vol.ADS[5]) / 4096) * 3.275);
		temp /= 0.42;
		Vol.IC = temp;
		break;
	case AD6_IC1:
		temp = ((((float) Vol.ADS[6]) / 4096) * 3.275);
		temp /= 1.9;
		Vol.IC1 = temp;
	
		if(ParasChecker.chargeflag & 0x02)
			Vol.BatInPower = Vol.IC1 * Vol.VFN;
		else if(ParasChecker.chargeflag & 0x04)
			Vol.BatInPower = Vol.IC1 * Vol.VTY;
		else if((ParasChecker.chargeflag & 0x06) == 0x06)
			Vol.BatInPower = (Vol.VFN > Vol.VFN ? (Vol.IC1*Vol.VFN) : (Vol.IC1*Vol.VTY));
		
		Vol.cnt++;
		if(Vol.cnt >= 5)
			Vol.complate = 1;
		volComplt = 1;
		break;
	}
}

//循环采集各个通道输入电压
void VolPro(void)	
{
	u8 ch=0;
	switch (Vol.CurCh) 
	{
	case AD0_VAC:
		ch = ADCS.AD0_VAC.ch;		//市电输入电压采集
		break;
	case AD1_VFN:
		ch = ADCS.AD1_VFN.ch;		//风能输入电压采集
		break;
	case AD2_VTY:
		ch = ADCS.AD2_VTY.ch;		//太阳能输入电压采集
		break;
	case AD3_BAT:
		ch = ADCS.AD3_BAT.ch;		//电池电压采集
		break;
	case AD4_IF:
		ch = ADCS.AD4_IF.ch;		//放电电流采集
		break;
	case AD5_IC:
		ch = ADCS.AD5_IC.ch;		//充电电流采集
		break;
	case AD6_IC1:
		ch = ADCS.AD6_IC1.ch;		//充电电流采集
		break;
	}
	Vol.AD_Temps[Vol.VolCnt++] = Get_Adc(ch);
	if (Vol.VolCnt >= MAXVOLCNT) 
	{
		CalculatedAverage();
		Vol.VolCnt = 0;
		if (Vol.CurCh == AD6_IC1)
			Vol.CurCh = AD0_VAC;
		else
			Vol.CurCh++;
	}
}
