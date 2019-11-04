#ifndef __DISPMANGER_H
#define __DISPMANGER_H

#include "Public.h"


typedef enum
{
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_ENTER,
	KEY_EXIT,
}_KeyVal;

typedef struct _page
{
	struct _page* pre;
	
	void(*EnterPage)(void);
	void(*ExitPage)(void);
	void(*UpKeyHandler)(void);
	void(*DownKeyHandler)(void);
	void(*LeftKeyHandler)(void);
	void(*RightHandler)(void);
	void(*EnterKeyHandler)(void);
	void(*ExitKeyHandler)(void);
}_Page;

extern _Page CurInfoPage;

void DispInit(void);
void Disp_KeyHandler(_KeyVal val);
void PageJump(_Page* toPage);

#endif

/*页面模版
static void EnterPage(void);
static void ExitPage(void);
static void UpKeyHandler(void);
static void DownKeyHandler(void);
static void LeftKeyHandler(void);
static void RightKeyHandler(void);
static void EnterKeyHandler(void);
static void ExitKeyHandler(void);
_Page CurInfoPage = {0,EnterPage,ExitPage,UpKeyHandler,DownKeyHandler,LeftKeyHandler,RightKeyHandler,EnterKeyHandler,ExitKeyHandler};

//页面进入、退出
static void EnterPage(void)
{

}

static void ExitPage(void)
{

}
//页面按键处理函数
//上
static void UpKeyHandler(void)
{

}
//下
static void DownKeyHandler(void)
{

}
//左
static void LeftKeyHandler(void)
{

}
//右
static void RightKeyHandler(void)
{

}
//确认
static void EnterKeyHandler(void)
{

}
//退出
static void ExitKeyHandler(void)
{

}
*/
