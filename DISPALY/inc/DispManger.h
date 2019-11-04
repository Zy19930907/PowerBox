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

/*ҳ��ģ��
static void EnterPage(void);
static void ExitPage(void);
static void UpKeyHandler(void);
static void DownKeyHandler(void);
static void LeftKeyHandler(void);
static void RightKeyHandler(void);
static void EnterKeyHandler(void);
static void ExitKeyHandler(void);
_Page CurInfoPage = {0,EnterPage,ExitPage,UpKeyHandler,DownKeyHandler,LeftKeyHandler,RightKeyHandler,EnterKeyHandler,ExitKeyHandler};

//ҳ����롢�˳�
static void EnterPage(void)
{

}

static void ExitPage(void)
{

}
//ҳ�水��������
//��
static void UpKeyHandler(void)
{

}
//��
static void DownKeyHandler(void)
{

}
//��
static void LeftKeyHandler(void)
{

}
//��
static void RightKeyHandler(void)
{

}
//ȷ��
static void EnterKeyHandler(void)
{

}
//�˳�
static void ExitKeyHandler(void)
{

}
*/
