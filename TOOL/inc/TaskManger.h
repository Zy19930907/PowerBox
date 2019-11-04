#ifndef __TASKMANGER_H
#define __TASKMANGER_H

#include "Public.h"

typedef struct _task
{
	unsigned long tick;			//����ִ��ʱ��
	unsigned long period;		//����ִ�м��
	unsigned long time;
	void(*Func)(void);			//�������ִ�к���ָ��
	struct _task *pre;
	struct _task *next;
}_Task;

typedef struct
{
	unsigned long exeTime;
	unsigned long Tick;
	_Task *curTask;
	_Task *firTask;
	_Task *lasTask;
}_TaskManger;
extern _TaskManger TaskManger;

extern _Task* CreateTask(u64 day,u64 hour,u64 min,u64 sec,u64 ms,void(*Func)(void));
extern void SetTaskDelay(u64 day,u64 hour,u64 min,u64 sec,u64 ms,_Task *Task);
extern void DelTask(_Task *delTask);
extern void ExecTask(void);

#endif
