#include "Public.h"

_Task *RunLedTask;               	 //运行指示灯任务控制块
_Task *VolTask;                       	 //电压采集任务控制块
_Task *MainFreqTask;        		//市电频率转换任务控制块
_Task *WindFreqTask;       		 //风电频率转换任务控制块
_Task *CtrTask;                        //端口控制任务控制块
_Task *KeyTask;                        //按键扫描任务控制块
_Task *ParasCheckTask;        //参数检测任务控制块

u8 volComplt = 0;

int main(void)
{
	McuInit();
	my_mem_init();                                                                                        							//初始化内存池
	Uart1Init(115200);
	Uart2Init(115200);

	AdcInit();                                                                                                							//ADC初始化
	BeepInit();                                                                                                							//蜂鸣器初始化
	DispInit();                                                                                                							//初始化液晶显示

	VolTask = CreateTask(0, 0, 0, 0, 5, VolPro);                        	//电压采集任务,每5毫秒转换一次
	WindFreqTask = CreateTask(0, 0, 0, 0, 0, FreqPro);                 //风能/市电频率采集
	ParasCheckTask = CreateTask(0, 0, 0, 0, 0, ParasCheckPro);     //判断系统各项参数是否正常任务
	CtrTask = CreateTask(0, 0, 0, 0, 0, CtrPro);                        	//输出端口控制任务
	KeyTask = CreateTask(0, 0, 0, 0, 20, KeyPro);                		//创建按键检测任务
	CreateTask(0, 0, 0, 0, 50, TemputerPro);								//温度检测任务

	CreateTask(0, 0, 0, 1, 0, DebugPro);                                       //调试任务

	ExecTask();                                                                                                						//循环执行任务链表中的任务
}
