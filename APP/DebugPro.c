#include "DebugPro.h"

u8 msg[21];

void DebugPro(void)
{
	u16 *p;
	*(u32*)msg = 0x55335533;
	msg[4] = Vol.VAC;
	p = (u16*)&msg[5];
	*p++ = 17;
	*p++ = (u16)(Vol.VFN * 100);
	*p++ = (u16)(Vol.VTY * 100);
	*p++ = (u16)(Vol.BAT * 100);
	*p++ = (u16)(Vol.VBAT * 100);
	*p++ = (u16)(Vol.IF * 100);
	*p++ = (u16)(Vol.IC * 100);
	CrcUtil.SetCrc(msg,21);
	Uart1SendData(msg,21);
	Uart2SendData(msg,21);
}
