#ifndef __PARASCHECKPRO_H
#define __PARASCHECKPRO_H

#include "Public.h"

#define	TYPE_48V	0
#define  TPPE_60V	1

#define VACMAX						248		//市电过压保护告警值
#define VACMIN						192		//市电欠压保护告警值

#define VFNMAX						30		//风能过压保护告警值
#define VFNMIN						18		//风能欠压保护告警值

#define VTYMAX						45		//太阳能过压保护告警值
#define VTYMIN						24		//太阳能欠压保护告警值

//48V电源参数配置
#if (TYPE_48V)
	#define VBATMAX				58			//电池过压保护值
	#define VBATUPRES				56			//电池过压恢复值
	#define VBATMIN					41.5		//电池欠压保护值
	#define VBATDOWNRES		45.5		//电池欠压恢复值
	
	#define BATFULL					51.5		//电池电量满
	#define BATNONE				45.5		//无电池电量
	
	#define CUMAX					56			//电流上限值
	#define BATPARA					71.1
	#define VOLREPAIRC			10.50
	#define VOLREPAIRCF			6.5
	#define VOLREPAIRF			7.5
#elif(TPPE_60V)
	#define VBATMAX				72.5		//电池过压保护值
	#define VBATUPRES				70.5		//电池过压恢复值
	#define VBATMIN					51.5		//电池欠压保护值
	#define VBATDOWNRES		63.5		//电池欠压恢复值
	
	#define BATFULL					67		//电池电量满
	#define BATNONE				59		//无电池电量
	
	#define CUMAX					46		//电流上限值
	#define BATPARA					70.129
	
	#define VOLREPAIRC			10.20
	#define VOLREPAIRCF			8.50
	#define VOLREPAIRF			10.15
#endif

typedef enum{
	WAITVOLCOMPLETE,
	DOCHECK,
}_CheckStatus;

typedef struct
{
	_CheckStatus Status;
	u8 volerrflag;
	u8 chargeflag;
	u8 currentflag;
	u16 errflag;
	u8 inoutflag;
	u8 batpercent;
}_ParasChecker;

extern _ParasChecker ParasChecker;
extern void ParasCheckPro(void);

#endif
