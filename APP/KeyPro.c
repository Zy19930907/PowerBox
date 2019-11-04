#include "KeyPro.h"

_Key Key = {
	.KeyVal={0,0},
	.Status = KEYINIT,
	.cnt = 0,
};

void ResetKey(void)
{
	Key.KeyVal[0] = 0;
	Key.KeyVal[1] = 0;
	Key.cnt = 0;
}

void KeyPro(void)
{
	u8 key = KEYSVAL;
	switch(Key.Status)
	{
	case KEYINIT:
		KeysInit();
		Key.Status = READKEY;
		break;
	case READKEY:
		if(key)//有按键按下
		{
			Key.KeyVal[Key.cnt++] = key;
			if((Key.cnt == 2) && Key.KeyVal[0] == Key.KeyVal[1])//两次读取按键值消抖
				Key.Status = KEYPRESS;
		}
		else
		{
			ResetKey();
		}
		break;

	case KEYPRESS:
		if(key == 0)
			Key.Status = KEYCLICK;
		break;

	case KEYCLICK:
		switch(Key.KeyVal[1] & 0x03)
		{
		case 0x01://KEY1按下
			Disp_KeyHandler(KEY_ENTER);
			break;
		case 0x02://KEY2按下
			Disp_KeyHandler(KEY_RIGHT);
			break;
		default:
			break;
		}
		ResetKey();
		Key.Status = READKEY;
		break;
	}
}
