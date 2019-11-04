#include "TemputerPro.h"

_Temputer Temputer;

void TemputerPro(void)
{
	Temputer.val[Temputer.cnt++] = GETTEMPIOVALUES;
	if(Temputer.cnt >= 2)
	{
		if(Temputer.val[0] && Temputer.val[1])
			Temputer.flag |= 0x01;
		else if(Temputer.val[0] == 0 && Temputer.val[1] == 0)
			Temputer.flag &= ~0x01;
		Temputer.cnt = 0;
	}
}
