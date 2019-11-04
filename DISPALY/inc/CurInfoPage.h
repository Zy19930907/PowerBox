#ifndef __CURINFOPAGE_H
#define __CURINFOPAGE_H

#include "DispManger.h"
#define SHOWPOINTCNT			69	//Òº¾§¶ÎÂëµãÊý
#define DISPCNT					20

typedef enum
{
	BATVOL,
	BATPOWER,
	INFREQ,
}_BatItem;

typedef enum
{
	IN,
	OUT,
}_ShowInOut;

typedef enum
{
	MAINSBAT,
	WINDBAT,
	SUNBAT,
}_ShowItem;

typedef struct
{
	u8 COM;
	u8 SEG;
	u8 Flag;
}_DispPoint;

typedef struct
{
	_DispPoint X1_MAINS;			
	_DispPoint X2_WIND;				
	_DispPoint X3_SUN;				
	_DispPoint X4_IN;				
	_DispPoint X5_OUT;				
	_DispPoint X6_VL;				
	_DispPoint X7_Hz;				
	_DispPoint X8_VR;				
	_DispPoint X9_W;				
	_DispPoint X10_MAINSARROW1; 	
	_DispPoint X11_WINDARROW1;		
	_DispPoint X12_SUNARROW1;		
	_DispPoint X13_MAINSARROW2;		
	_DispPoint X14_WINDARROW2;		
	_DispPoint X15_SUNARROW2;		
	_DispPoint X16_BATARROW;		
	_DispPoint X17_LOAD;
	_DispPoint X22_BAT0;			
	_DispPoint X21_BAT1;			
	_DispPoint X20_BAT2;			
	_DispPoint X19_BAT3;			
	_DispPoint X18_BAT4;			
	_DispPoint X23_BATVAL;			
	
	_DispPoint X;
	_DispPoint XX;
	
	_DispPoint NUM1_A;
	_DispPoint NUM1_B;
	_DispPoint NUM1_C;
	_DispPoint NUM1_D;
	_DispPoint NUM1_E;
	_DispPoint NUM1_F;
	_DispPoint NUM1_G;

	_DispPoint NUM2_A;
	_DispPoint NUM2_B;
	_DispPoint NUM2_C;
	_DispPoint NUM2_D;
	_DispPoint NUM2_E;
	_DispPoint NUM2_F;
	_DispPoint NUM2_G;

	_DispPoint NUM3_A;
	_DispPoint NUM3_B;
	_DispPoint NUM3_C;
	_DispPoint NUM3_D;
	_DispPoint NUM3_E;
	_DispPoint NUM3_F;
	_DispPoint NUM3_G;

	_DispPoint NUM4_A;
	_DispPoint NUM4_B;
	_DispPoint NUM4_C;
	_DispPoint NUM4_D;
	_DispPoint NUM4_E;
	_DispPoint NUM4_F;
	_DispPoint NUM4_G;

	_DispPoint NUM5_A;
	_DispPoint NUM5_B;
	_DispPoint NUM5_C;
	_DispPoint NUM5_D;
	_DispPoint NUM5_E;
	_DispPoint NUM5_F;
	_DispPoint NUM5_G;

	_DispPoint NUM6_A;
	_DispPoint NUM6_B;
	_DispPoint NUM6_C;
	_DispPoint NUM6_D;
	_DispPoint NUM6_E;
	_DispPoint NUM6_F;
	_DispPoint NUM6_G;

	_DispPoint NUM_P;
	_DispPoint NUM_P1;
	
	u64 COM_RAM_VAL[4];
	u8 inoutCnt;
	u8 CurDispCnt;
	u8 FlashCnt;
	_ShowItem ShowItem;
	_ShowInOut ShowInOut;
	_BatItem BatItem;
	u16 UserOption;
	u8 isShow;
	u8 waitCnt;
	u8 windSun;
}_CurInfo;
extern _CurInfo CurInfo;


#endif
