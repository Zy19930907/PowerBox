#ifndef __PARASCHECKPRO_H
#define __PARASCHECKPRO_H

#include "Public.h"

#define	TYPE_48V	0
#define  TPPE_60V	1

#define VACMAX						248		//�е��ѹ�����澯ֵ
#define VACMIN						192		//�е�Ƿѹ�����澯ֵ

#define VFNMAX						30		//���ܹ�ѹ�����澯ֵ
#define VFNMIN						18		//����Ƿѹ�����澯ֵ

#define VTYMAX						45		//̫���ܹ�ѹ�����澯ֵ
#define VTYMIN						24		//̫����Ƿѹ�����澯ֵ

//48V��Դ��������
#if (TYPE_48V)
	#define VBATMAX				58			//��ع�ѹ����ֵ
	#define VBATUPRES				56			//��ع�ѹ�ָ�ֵ
	#define VBATMIN					41.5		//���Ƿѹ����ֵ
	#define VBATDOWNRES		45.5		//���Ƿѹ�ָ�ֵ
	
	#define BATFULL					51.5		//��ص�����
	#define BATNONE				45.5		//�޵�ص���
	
	#define CUMAX					56			//��������ֵ
	#define BATPARA					71.1
	#define VOLREPAIRC			10.50
	#define VOLREPAIRCF			6.5
	#define VOLREPAIRF			7.5
#elif(TPPE_60V)
	#define VBATMAX				72.5		//��ع�ѹ����ֵ
	#define VBATUPRES				70.5		//��ع�ѹ�ָ�ֵ
	#define VBATMIN					51.5		//���Ƿѹ����ֵ
	#define VBATDOWNRES		63.5		//���Ƿѹ�ָ�ֵ
	
	#define BATFULL					67		//��ص�����
	#define BATNONE				59		//�޵�ص���
	
	#define CUMAX					46		//��������ֵ
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
