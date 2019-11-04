#include "CtrPro.h"
#include "CurInfoPage.h"

_CtrStatus CtrStatus = CTRINIT;

void doCtr(void) 
{	
	u8 diff;
	ParasChecker.chargeflag = 0;
	
	if(Temputer.flag & 0x01)//温度超限，关闭输入和输出
	{
		KACDIS;
		KFNDIS;
		KVODIS;
		return;
	}
	
	if(ParasChecker.errflag & 0x01)//电池过压,关闭所有电源输入
	{
		KACDIS;//关闭市电充电
		KFNDIS;//关闭风能充电
	}else
	{
		if((ParasChecker.volerrflag & 0x07) == 0x06) //风能、太阳能都异常且市电正常
		{
			ParasChecker.chargeflag |= 0x01;	//标记当前选择市电充电
			KACEN;//打开市电输入充电
			KFNDIS;
		}
		else//风能、太阳能中至少有一个正常
			KACDIS;//关闭市电输入充电
		
		if(!(ParasChecker.volerrflag & 0x06))	//风能、太阳能都正常
		{
			diff = (Vol.VFN > Vol.VTY ? (Vol.VFN-Vol.VTY) : (Vol.VTY-Vol.VFN));
			if(diff < 10)//风能、太阳能电压相差不超过10V
			{
				ParasChecker.chargeflag |= 0x06;//显示同时选用风能、太阳能充电
				KFNEN;
			}
			else//超过10V
			{
				KFNEN;
				ParasChecker.chargeflag |= (Vol.VFN > Vol.VTY ? 0x02:0x04);//显示太阳能、风能中电压高者充电
			}
		}else//风能太阳能都不正常或只有其中一个正常
		{
			if(!(ParasChecker.volerrflag & 0x02))//风能正常，使能风能升压充电
			{
				ParasChecker.chargeflag |= 0x02; //标记当前选择风能充电
				KFNEN;
			}
			
			if(!(ParasChecker.volerrflag & 0x04))//太阳能正常，使能升压充电
			{
				ParasChecker.chargeflag |= 0x04;
				KFNEN;
			}
		}
		
		//均充控制
		if(ParasChecker.currentflag & 0x02)
			KJCEN;
		else
			KJCDIS;
	}
	
	//负载过流或电池欠压且没有市电供电时切断输出
	if(((ParasChecker.currentflag & 0x01) || ((ParasChecker.errflag & 0x02) && (ParasChecker.volerrflag & 0x01)))) 
	{
		ParasChecker.inoutflag &= ~0x02;
		KVODIS;
	}
	else
	{
		ParasChecker.inoutflag |= 0x02;
		KVOEN;
	}
}

void CtrPro(void) 
{
	switch (CtrStatus) 
	{
	case DOCTR:
		doCtr();
		break;

	case CTRWAIT://等待各项参数采集稳定再进行控制端口的控制
		if (Vol.complate)
		{
			doCtr();
			CtrStatus = DOCTR;
		}	
		break;

	case CTRINIT:
		ControlIoInit();
		CtrStatus = CTRWAIT;
		break;
	}
}
