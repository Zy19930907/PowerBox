#ifndef __UART_H
#define __UART_H

#include "Public.h"

#define MAXUARTBUFLENN		1024

extern void Uart1Init(u32 baud);
extern void Uart2Init(u32 baud);
extern void Uart2SendBuf(u8 *buf);
extern void Uart1SendBuf(u8 *buf);
extern void Uart1SendData(u8 *buf,u16 len);
extern void Uart2SendData(u8 *buf,u16 len);
extern void Uart1SendDat(u8 *buf,u16 len);

#endif
