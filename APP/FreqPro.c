#include "FreqPro.h"

_Freq Freq = {FREQINIT,0x003F3F3F};

u32 FreqToDisp(u16 freq)
{
	u32 i,j,k;
	i = NumToDispCode(((freq / 100) % 10));	//百位
	j = NumToDispCode(((freq / 10) % 10));	//十位
	k = NumToDispCode((freq % 10));			//个位
	return ((i<<16) | (j << 8) | k);
}

void FreqPro(void)
{
	u8 i;
	switch(Freq.Status)
	{
		case FREQIDLE:
			break;
		
		case FREQCAPED:												//风电频率采集完成
			Freq.freqtemp[Freq.cnt++] = Capture.CAP2_FFN.Freq;
			if(Freq.cnt >= FREQTEMPCNT)
			{
				Freq.cnt = 0;
				Freq.total = 0;
				mySort_u8(Freq.freqtemp,FREQTEMPCNT);				//排序
				for(i=FREQOFFSET;i<FREQTEMPCNT-FREQOFFSET;i++)		//掐头去尾求和
					Freq.total+=Freq.freqtemp[i];
				Freq.freq = Freq.total / (FREQTEMPCNT-(FREQOFFSET<<1));//取平均值
			}
			Freq.Disp_Freq = FreqToDisp(Capture.CAP2_FFN.Freq);		//将定时器捕获计算的频率转换为数码管段码
			Freq.Status = FREQIDLE;
			break;
		
		case FREQINIT:
			CaptureInit();											//定时器3输入捕获初始化，采集频率信号
			Freq.Status = FREQIDLE;
			break;
	}
}
