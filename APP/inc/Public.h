#ifndef __PUBLIC_H
#define __PUBLIC_H

#include <stm32l152xe.h>
#include <string.h>

#define DEBUG				1
//GPIO设置专用宏定义
#define GPIO_MODE_IN    	0		//普通输入模式
#define GPIO_MODE_OUT		1		//普通输出模式
#define GPIO_MODE_AF		2		//AF功能模式
#define GPIO_MODE_AIN		3		//模拟输入模式

#define GPIO_SPEED_2M		0		//GPIO速度2Mhz
#define GPIO_SPEED_25M		1		//GPIO速度25Mhz
#define GPIO_SPEED_50M		2		//GPIO速度50Mhz
#define GPIO_SPEED_100M		3		//GPIO速度100Mhz

#define GPIO_PUPD_NONE		0		//不带上下拉
#define GPIO_PUPD_PU		1		//上拉
#define GPIO_PUPD_PD		2		//下拉
#define GPIO_PUPD_RES		3		//保留 

#define GPIO_OTYPE_PP		0		//推挽输出
#define GPIO_OTYPE_OD		1		//开漏输出 

//Ex_NVIC_Config专用定义
#define GPIO_A 				0
#define GPIO_B 				1
#define GPIO_C				2
#define GPIO_D 				3
#define GPIO_E 				4
#define GPIO_F 				5
#define GPIO_G 				6 
#define GPIO_H 				7 
#define GPIO_I 				8 
#define FTIR   				1  		//下降沿触发
#define RTIR   				2  		//上升沿触发

//GPIO引脚编号定义
#define PIN0				(u32)0x01
#define PIN1				(u32)0x02
#define PIN2				(u32)0x04
#define PIN3				(u32)0x08
#define PIN4				(u32)0x10
#define PIN5				(u32)0x20
#define PIN6				(u32)0x40
#define PIN7				(u32)0x80
#define PIN8				(u32)0x100
#define PIN9				(u32)0x200
#define PIN10				(u32)0x400
#define PIN11				(u32)0x800
#define PIN12				(u32)0x1000
#define PIN13				(u32)0x2000
#define PIN14				(u32)0x4000
#define PIN15				(u32)0x8000

typedef enum
{
	NOPU,
	IOPU,
	IOPD,
}_IOPuPd;

typedef enum
{
	IOIN,
	IOOUT,
	IOFUNC,
}_IOMode;

typedef uint64_t  	u64;
typedef volatile 	uint64_t  vu64;
typedef uint32_t  	u32;
typedef volatile 	uint32_t  vu32;
typedef uint16_t 	u16;
typedef uint8_t  	u8;

//HARDWARE
#include "RML1028B.h"
#include "Mcu.h"
#include "Uart.h"
#include "Temputer.h"
#include "Adc.h"
#include "Timer.h"
#include "Beep.h"
#include "Control.h"
#include "Exti.h"
#include "Keys.h"
//TOOL
#include "malloc.h"
#include "TaskManger.h"
#include "DispManger.h"
#include "CrcUtil.h"
//APP
#include "VolPro.h"
#include "FreqPro.h"
#include "CtrPro.h"
#include "DebugPro.h"
#include "KeyPro.h"
#include "ParasCheckPro.h"
#include "TemputerPro.h"

extern void IoInit(GPIO_TypeDef* Portx,u32 Pinx,_IOMode mode,_IOPuPd pupd);
extern void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx);
extern void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD);
extern u8 NumToDispCode(u8 num);
extern u8 Pin2Num(u32 Pinx);
extern void mySort(u32 *buf,u16 len);
extern void mySort_u8(u8 *buf,u16 len);
extern void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM);

#endif
