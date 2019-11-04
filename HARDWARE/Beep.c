#include "Beep.h"

void BeepInit(void)
{
	BEEPIOCLKEN;
	IoInit(BEEPPORT,BEEPPIN,IOOUT,NOPU);	//蜂鸣器引脚初始化为输出模式
	BEEPOFF;
}
