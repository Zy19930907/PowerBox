#include "TaskManger.h"
#include "malloc.h"

unsigned long tick;
static inline void StartTask(void);
static inline void EndTask(void);
extern _Task sTask;
_Task eTask={0,0,0,EndTask,&sTask,&sTask};
_Task sTask={0,0,0,StartTask,&eTask,&eTask};
_TaskManger TaskManger = {0,0,&sTask,&sTask,&eTask};

//��������ִ��ʱ��
static inline u64 TaskTickDiff(u64 tick)
{
	u64 systick = TaskManger.Tick;	
	if(tick > systick)
		return 0xFFFFFFFFFFFFFFFF - tick + systick;
	else
		return systick - tick;
}
//Ĭ�ϴ�������λ����������ʼ�ڵ�
static inline void StartTask(void)
{
	tick = TaskManger.Tick;
}
//Ĭ�ϴ�������λ��������������ڵ�
static inline void EndTask(void)
{
	TaskManger.exeTime = TaskTickDiff(tick);
}

//������������ӵ�������������ڵ����һ�ڵ�
_Task* CreateTask(u64 day,u64 hour,u64 min,u64 sec,u64 ms,void(*Func)(void))
{
	_Task *Task=0;
	Task = mymalloc(sizeof(_Task));
	if(Task != 0)
	{
		Task->Func = Func;
		Task->period = ((day * 86400000) + (hour * 3600000) + (min * 60000) + (sec * 1000) + ms);	//��������ִ������
		Task->pre = TaskManger.lasTask->pre;
		Task->next = TaskManger.lasTask;
		TaskManger.lasTask->pre->next = Task;
		TaskManger.lasTask->pre = Task;
	}
	return Task;
}

//������������ɾ������
void DelTask(_Task *delTask)
{
	if(delTask != 0)
	{
		delTask->pre->next = delTask->next;
		delTask->next->pre = delTask->pre;
		myfree(delTask);
	}
}

//�޸�����ִ������
void SetTaskDelay(u64 day,u64 hour,u64 min,u64 sec,u64 ms,_Task *Task)
{
	Task->period = ((day * 86400000) + (hour * 3600000) + (min * 60000) + (sec * 1000) + ms);	//��������ִ������
}

//ѭ��ִ�����������е�����
void ExecTask(void)
{
	for(;;)
	{
		if(TaskTickDiff(TaskManger.curTask->tick) >= TaskManger.curTask->period)
		{
			TaskManger.curTask->tick = TaskManger.Tick;
			TaskManger.curTask->Func();//������������
			TaskManger.curTask->time = TaskTickDiff(TaskManger.curTask->tick);
		}
		TaskManger.curTask = TaskManger.curTask->next;
	}
}

