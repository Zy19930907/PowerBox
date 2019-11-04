#include "DispManger.h"

_Page* CurPage ;

//页面跳转
void PageJump(_Page* toPage)
{
	CurPage->ExitPage();	//退出当前页面
	toPage->EnterPage();	//进入目标页面
	toPage->pre = CurPage;	//标记从当前页面跳转到目标页面
	CurPage = toPage;
}

//按键按下后由此接口调用当前界面的对应按键处理函数
void Disp_KeyHandler(_KeyVal val)
{
	switch(val)
	{
		case KEY_UP:
			CurPage->UpKeyHandler();
			break;
		case KEY_DOWN:
			CurPage->DownKeyHandler();
			break;
		case KEY_LEFT:
			CurPage->LeftKeyHandler();
			break;
		case KEY_RIGHT:
			CurPage->RightHandler();
			break;
		case KEY_ENTER:
			CurPage->EnterKeyHandler();
			break;
		case KEY_EXIT:
			CurPage->EnterKeyHandler();
			break;
	}
}


void DispInit(void)
{
	RML1028B_Init();				//初始化LCD控制器
	CurPage = &CurInfoPage;
	PageJump(&CurInfoPage);
}
