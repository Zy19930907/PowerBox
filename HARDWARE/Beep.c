#include "Beep.h"

void BeepInit(void)
{
	BEEPIOCLKEN;
	IoInit(BEEPPORT,BEEPPIN,IOOUT,NOPU);	//���������ų�ʼ��Ϊ���ģʽ
	BEEPOFF;
}
