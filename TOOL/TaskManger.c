#include "TaskManger.h"
#include "malloc.h"

unsigned long tick;
static inline void StartTask(void);
static inline void EndTask(void);
extern _Task sTask;
_Task eTask={0,0,0,EndTask,&sTask,&sTask};
_Task sTask={0,0,0,StartTask,&eTask,&eTask};
_TaskManger TaskManger = {0,0,&sTask,&sTask,&eTask};

//计算任务执行时钟
static inline u64 TaskTickDiff(u64 tick)
{
	u64 systick = TaskManger.Tick;	
	if(tick > systick)
		return 0xFFFFFFFFFFFFFFFF - tick + systick;
	else
		return systick - tick;
}
//默认创建任务，位于任务链表开始节点
static inline void StartTask(void)
{
	tick = TaskManger.Tick;
}
//默认创建任务，位于任务链表结束节点
static inline void EndTask(void)
{
	TaskManger.exeTime = TaskTickDiff(tick);
}

//创建新任务，添加到任务链表结束节点的上一节点
_Task* CreateTask(u64 day,u64 hour,u64 min,u64 sec,u64 ms,void(*Func)(void))
{
	_Task *Task=0;
	Task = mymalloc(sizeof(_Task));
	if(Task != 0)
	{
		Task->Func = Func;
		Task->period = ((day * 86400000) + (hour * 3600000) + (min * 60000) + (sec * 1000) + ms);	//计算任务执行周期
		Task->pre = TaskManger.lasTask->pre;
		Task->next = TaskManger.lasTask;
		TaskManger.lasTask->pre->next = Task;
		TaskManger.lasTask->pre = Task;
	}
	return Task;
}

//从任务链表中删除任务
void DelTask(_Task *delTask)
{
	if(delTask != 0)
	{
		delTask->pre->next = delTask->next;
		delTask->next->pre = delTask->pre;
		myfree(delTask);
	}
}

//修改任务执行周期
void SetTaskDelay(u64 day,u64 hour,u64 min,u64 sec,u64 ms,_Task *Task)
{
	Task->period = ((day * 86400000) + (hour * 3600000) + (min * 60000) + (sec * 1000) + ms);	//计算任务执行周期
}

//循环执行任务链表中的任务
void ExecTask(void)
{
	for(;;)
	{
		if(TaskTickDiff(TaskManger.curTask->tick) >= TaskManger.curTask->period)
		{
			TaskManger.curTask->tick = TaskManger.Tick;
			TaskManger.curTask->Func();//调用任务处理函数
			TaskManger.curTask->time = TaskTickDiff(TaskManger.curTask->tick);
		}
		TaskManger.curTask = TaskManger.curTask->next;
	}
}

