#include "CurInfoPage.h"

static void EnterPage(void);

static void ExitPage(void);

static void UpKeyHandler(void);

static void DownKeyHandler(void);

static void LeftKeyHandler(void);

static void RightKeyHandler(void);

static void EnterKeyHandler(void);

static void ExitKeyHandler(void);

_Page CurInfoPage = {0, EnterPage, ExitPage, UpKeyHandler, DownKeyHandler,
		     LeftKeyHandler, RightKeyHandler, EnterKeyHandler, ExitKeyHandler};
_Task *FreshTask;
vu64 numCode = 0;
u8 NumRam[6];

_CurInfo CurInfo = {
    .X1_MAINS = {4, 21, 0},
    .X2_WIND = {3, 21, 0},
    .X3_SUN = {2, 21, 0},
    .X4_IN = {4, 12, 0},
    .X5_OUT = {4, 30, 0},
    .X6_VL = {1, 10, 0},
    .X7_Hz = {1, 20, 0},
    .X8_VR = {3, 9, 0},
    .X9_W = {2, 9, 0},
    .X10_MAINSARROW1 = {2, 20, 0},
    .X11_WINDARROW1 = {3, 20, 0},
    .X12_SUNARROW1 = {4, 20, 0},
    .X13_MAINSARROW2 = {2, 19, 0},
    .X14_WINDARROW2 = {3, 19, 0},
    .X15_SUNARROW2 = {4, 19, 0},
    .X16_BATARROW = {1, 19, 0},
    .X17_LOAD = {4, 18, 0},
    .X22_BAT0 = {3, 18, 0},
    .X21_BAT1 = {3, 16, 0},
    .X20_BAT2 = {2, 16, 0},
    .X19_BAT3 = {1, 16, 0},
    .X18_BAT4 = {2, 18, 0},
    .X23_BATVAL = {1, 9, 0},

    .X = {1, 21, 1}, .XX = {1, 24, 1},

    //�����1
    .NUM1_A = {4, 11, 0}, .NUM1_B = {3, 11, 0}, .NUM1_C = {2, 11, 0},
    .NUM1_D = {1, 11, 0}, .NUM1_E = {2, 10, 0}, .NUM1_F = {4, 10, 0},
    .NUM1_G = {3, 10, 0},
    //�����2
    .NUM2_A = {4, 13, 0}, .NUM2_B = {3, 13, 0}, .NUM2_C = {2, 13, 0},
    .NUM2_D = {1, 13, 0}, .NUM2_E = {1, 12, 0}, .NUM2_F = {3, 12, 0},
    .NUM2_G = {2, 12, 0},
    //�����3
    .NUM3_A = {4, 15, 0}, .NUM3_B = {3, 15, 0}, .NUM3_C = {2, 15, 0},
    .NUM3_D = {1, 15, 0}, .NUM3_E = {2, 14, 0}, .NUM3_F = {4, 14, 0},
    .NUM3_G = {3, 14, 0},
    //�����4
    .NUM4_A = {4, 29, 0}, .NUM4_B = {3, 29, 0}, .NUM4_C = {2, 29, 0},
    .NUM4_D = {1, 29, 0}, .NUM4_E = {2, 24, 0}, .NUM4_F = {4, 24, 0},
    .NUM4_G = {3, 24, 0},
    //�����5
    .NUM5_A = {4, 31, 0}, .NUM5_B = {3, 31, 0}, .NUM5_C = {2, 31, 0},
    .NUM5_D = {1, 31, 0}, .NUM5_E = {1, 30, 0}, .NUM5_F = {3, 30, 0},
    .NUM5_G = {2, 30, 0},
    //�����6
    .NUM6_A = {4, 8, 0}, .NUM6_B = {3, 8, 0}, .NUM6_C = {2, 8, 0},
    .NUM6_D = {1, 8, 0}, .NUM6_E = {2, 7, 0}, .NUM6_F = {4, 7, 0},
    .NUM6_G = {3, 7, 0},

    .NUM_P = {1, 14, 0}, .NUM_P1 = {1, 7, 0},
    .COM_RAM_VAL = {0, 0, 0, 0}, .UserOption = 0, .isShow = 0, .FlashCnt=0,
    .windSun = 0,
    .inoutCnt = 0,
};

u8 NumToDispCode(u8 num)
{
	u8 shwbuf[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};        //����ܶ����
	return *(shwbuf + num);
}

void ClearShowFlag(void)
{
	u8 i;
	_DispPoint *p = (_DispPoint *) &CurInfo;
	for (i = 0; i < SHOWPOINTCNT; i++)                //��ʾ���ת��ΪLCDRAM����
	{
		p->Flag = 0;
		p++;
	}
	for (i = 0; i < 6; i++)
	{
		NumRam[i] = 0;
	}
}

//�������ʾ
void Num2Flag(void)
{
	u8 i;
	u64 j = 0;
	_DispPoint *p = (_DispPoint *) &CurInfo.NUM1_A;

	for (j = 0; j < 6; j++)
	{
		for (i = 0x01; i < 0x80; i <<= 1)
		{
			if (NumRam[j] & i)
				p->Flag = 1;
			else
				p->Flag = 0;
			p++;
		}
	}
}

//��CurInfo�е�Flagת����LCDRAM���ݲ�ˢ����ʾ
void DataToLcdRam(void)
{
	u8 i;
	_DispPoint *p;
	p = (_DispPoint *) (&CurInfo);
	for (i = 0; i < SHOWPOINTCNT; i++)                //��ʾ���ת��ΪLCDRAM����
	{
		if (p->Flag)
			CurInfo.COM_RAM_VAL[p->COM - 1] |= (1 << p->SEG);
		else
			CurInfo.COM_RAM_VAL[p->COM - 1] &= ~(1 << p->SEG);
		p++;
	}
	//��ʾ���ݸ��µ�LCD�Դ�
	COM1RAM = CurInfo.COM_RAM_VAL[0];
	COM2RAM = CurInfo.COM_RAM_VAL[1];
	COM3RAM = CurInfo.COM_RAM_VAL[2];
	COM4RAM = CurInfo.COM_RAM_VAL[3];
	//������ʾ
	LCD->SR |= 1 << 2;
}

//��ʾ�������
void ShowErrCode(u8 code)
{
	if (code < 4)
	{
		NumRam[3] = 0;
		NumRam[4] = 0x79;//"E"
		NumRam[5] = NumToDispCode(code);
		CurInfo.NUM_P1.Flag = 0;
		CurInfo.X8_VR.Flag = 0;
		CurInfo.X9_W.Flag = 0;
	} else
	{
		if (code >= 10)
		{
			NumRam[0] = 0x79;
			NumRam[1] = NumToDispCode(code / 10);
			NumRam[2] = NumToDispCode(code % 10);
		} else
		{
			NumRam[0] = 0;
			NumRam[1] = 0x79;
			NumRam[2] = NumToDispCode(code);
		}
		CurInfo.NUM_P.Flag = 0;
		CurInfo.X6_VL.Flag = 0;
		CurInfo.X7_Hz.Flag = 0;
	}
}

//��ʾ�е��ѹ
void ShowMainsVol(void)
{
	CurInfo.X1_MAINS.Flag = 1;
	if ((CurInfo.isShow && (ParasChecker.errflag & 0x18)))
	{
		if (ParasChecker.errflag & 0x08)
			ShowErrCode(4);
		else if (ParasChecker.errflag & 0x10)
			ShowErrCode(5);
	} else
	{
		NumRam[0] = NumToDispCode((Vol.VAC / 100) % 10);
		NumRam[1] = NumToDispCode((Vol.VAC / 10) % 10);
		NumRam[2] = NumToDispCode((Vol.VAC / 1) % 10);
		CurInfo.X6_VL.Flag = 1;
	}
}

//��ʾ�е�Ƶ��
void ShowMiansFreq(void)
{
	CurInfo.X1_MAINS.Flag = 1;
	if (ParasChecker.chargeflag & 0x01)
	{
		NumRam[0] = ((Freq.Disp_Freq >> 16) & 0xFF);
		NumRam[1] = ((Freq.Disp_Freq >> 8) & 0xFF);
		NumRam[2] = ((Freq.Disp_Freq >> 0) & 0xFF);
	} else
	{
		NumRam[0] = 0x40;
		NumRam[1] = 0x40;
		NumRam[2] = 0x40;
	}
	CurInfo.X7_Hz.Flag = 1;
}

//��ʾ���ܵ�ѹ
void ShowWindVol(void)
{
	CurInfo.X2_WIND.Flag = 1;
	if ((CurInfo.isShow && (ParasChecker.errflag & 0x60)))
	{
		if (ParasChecker.errflag & 0x20)
			ShowErrCode(6);
		else if (ParasChecker.errflag & 0x40)
			ShowErrCode(7);
	} else
	{
		NumRam[0] = NumToDispCode(((u16) (Vol.VFN * 10) / 100) % 10);
		NumRam[1] = NumToDispCode(((u16) (Vol.VFN * 10) / 10) % 10);
		NumRam[2] = NumToDispCode((u16) (Vol.VFN * 10) % 10);
		CurInfo.X6_VL.Flag = 1;
		CurInfo.NUM_P.Flag = 1;
	}
}

//��ʾ����Ƶ��
void ShowWindFreq(void)
{
	CurInfo.X2_WIND.Flag = 1;
	if (ParasChecker.chargeflag & 0x02)
	{
		NumRam[0] = ((Freq.Disp_Freq >> 16) & 0xFF);
		NumRam[1] = ((Freq.Disp_Freq >> 8) & 0xFF);
		NumRam[2] = ((Freq.Disp_Freq >> 0) & 0xFF);
	} else
	{
		NumRam[0] = 0x40;
		NumRam[1] = 0x40;
		NumRam[2] = 0x40;
	}
	CurInfo.X7_Hz.Flag = 1;
}

//��ʾ̫���������ѹ
void ShowSunVol(void)
{
	CurInfo.X3_SUN.Flag = 1;
	if ((CurInfo.isShow && (ParasChecker.errflag & 0x180)))
	{
		if (ParasChecker.errflag & 0x80)
			ShowErrCode(8);
		else if (ParasChecker.errflag & 0x100)
			ShowErrCode(9);
	} else
	{
		NumRam[0] = NumToDispCode(((u16) (Vol.VTY * 10) / 100) % 10);
		NumRam[1] = NumToDispCode(((u16) (Vol.VTY * 10) / 10) % 10);
		NumRam[2] = NumToDispCode((u16) (Vol.VTY * 10) % 10);
		CurInfo.X6_VL.Flag = 1;
		CurInfo.NUM_P.Flag = 1;
	}
}

//��ʾ��ص�ѹ
void ShowVbat(void)
{
	if ((CurInfo.isShow && (ParasChecker.errflag & 0x03)))//��ش����쳣����Ҫ��ʾ�쳣����
	{
		if (ParasChecker.errflag & 0x01)//��ع�ѹ
			ShowErrCode(1);
		else if (ParasChecker.errflag & 0x02)//���Ƿѹ
			ShowErrCode(2);
	} else//��ʾ��ǰ��ص�ѹ
	{
		NumRam[3] = NumToDispCode(((u16) (Vol.BAT * 10) / 100) % 10);
		NumRam[4] = NumToDispCode(((u16) (Vol.BAT * 10) / 10) % 10);
		NumRam[5] = NumToDispCode(((u16) (Vol.BAT * 10) / 1) % 10);
		CurInfo.X8_VR.Flag = 1;
		CurInfo.NUM_P1.Flag = 1;
	}
}

//��ʾ��س�繦��
void ShowPbatOut(void)
{
	NumRam[0] = 0;
	NumRam[1] = 0;
	NumRam[2] = NumToDispCode((Vol.BatInPower / 1000) % 10);
	NumRam[3] = NumToDispCode((Vol.BatInPower / 100) % 10);
	NumRam[4] = NumToDispCode((Vol.BatInPower / 10) % 10);
	NumRam[5] = NumToDispCode(Vol.BatInPower % 10);
	CurInfo.X9_W.Flag = 1;
}

//��ʾ��طŵ繦��
void ShowPbatIn(void)
{
	NumRam[0] = 0;
	NumRam[1] = 0;
	NumRam[2] = NumToDispCode((Vol.BatOutPower / 1000) % 10);
	NumRam[3] = NumToDispCode((Vol.BatOutPower / 100) % 10);
	NumRam[4] = NumToDispCode((Vol.BatOutPower / 10) % 10);
	NumRam[5] = NumToDispCode(Vol.BatOutPower % 10);
	CurInfo.X9_W.Flag = 1;
}

//�����û�ѡ����ʾ
void ShowFromUserSelect(void)
{
	CurInfo.X.Flag = 1;
	CurInfo.XX.Flag = 1;

	switch (CurInfo.ShowItem)
	{
		case MAINSBAT:
			CurInfo.X1_MAINS.Flag = 1;
			switch (CurInfo.BatItem)
			{
				case BATVOL:
					switch (CurInfo.ShowInOut)
					{
						case IN:
							CurInfo.X4_IN.Flag = 1;
							ShowMainsVol();
							break;

						case OUT:
							CurInfo.X5_OUT.Flag = 1;
							ShowVbat();
							break;
					}
					break;
				case BATPOWER:
					switch (CurInfo.ShowInOut)
					{
						case IN:
							CurInfo.X4_IN.Flag = 1;
							ShowPbatIn();
							break;

						case OUT:
							CurInfo.X5_OUT.Flag = 1;
							ShowPbatOut();
							break;
					}
					break;
				case INFREQ:
					ShowMiansFreq();
					break;
			}
			break;

		case WINDBAT:
			CurInfo.X2_WIND.Flag = 1;
			switch (CurInfo.BatItem)
			{
				case BATVOL:
					switch (CurInfo.ShowInOut)
					{
						case IN:
							CurInfo.X4_IN.Flag = 1;
							ShowWindVol();
							break;

						case OUT:
							CurInfo.X5_OUT.Flag = 1;
							ShowVbat();
							break;
					}
					break;
				case BATPOWER:
					switch (CurInfo.ShowInOut)
					{
						case IN:
							CurInfo.X4_IN.Flag = 1;
							ShowPbatIn();
							break;

						case OUT:
							CurInfo.X5_OUT.Flag = 1;
							ShowPbatOut();
							break;
					}
					break;
				case INFREQ:
					ShowWindFreq();
					break;
			}
			break;

		case SUNBAT:
			CurInfo.X3_SUN.Flag = 1;
			switch (CurInfo.BatItem)
			{
				case BATVOL:
					switch (CurInfo.ShowInOut)
					{
						case IN:
							CurInfo.X4_IN.Flag = 1;
							ShowSunVol();
							break;

						case OUT:
							CurInfo.X5_OUT.Flag = 1;
							ShowVbat();
							break;
					}
					break;
				case BATPOWER:
					switch (CurInfo.ShowInOut)
					{
						case IN:
							CurInfo.X4_IN.Flag = 1;
							ShowPbatIn();
							break;

						case OUT:
							CurInfo.X5_OUT.Flag = 1;
							ShowPbatOut();
							break;
					}
					break;
				case INFREQ:
					ShowSunVol();
					break;
			}
			break;
	}
}

//���ݵ�Դ����ȷ����ǰ��ʾ����
void ShowFromPowerIn(void)
{
	switch (ParasChecker.chargeflag & 0x07)
	{
		//��ǰѡ���е���Ϊ��س������
		case 0x01:
			ShowMainsVol();
			ShowVbat();
			break;
			//��ǰѡ�������Ϊ��س������
		case 0x02:
			ShowWindVol();
			ShowVbat();
			break;
			//��ǰѡ��̫������Ϊ��س������
		case 0x04:
			ShowSunVol();
			ShowVbat();
			break;
			//��ǰѡ��̫���ܡ�������Ϊ��س������
		case 0x06:
			if (CurInfo.windSun)
			{
				ShowWindVol();
				ShowVbat();
			} else
			{
				ShowWindVol();
				ShowVbat();
			}
			break;
		default:
			ShowMainsVol();
			ShowVbat();
			break;
	}
}

void ShowTempWarn(void)
{
	if (CurInfo.isShow)
		ShowErrCode(10);
}

//���ݵ�ǰϵͳ״̬ȷ����ʾ����
void FreshCurInfo(void)
{
	u8 i;
	_DispPoint *p;
	ClearShowFlag();                                                //���ԭ����ʾ���
	if (Vol.complate)                                                //��������ɼ����
	{
		if (!(ParasChecker.volerrflag & 0x01))
			CurInfo.X10_MAINSARROW1.Flag = 1;
		if (!(ParasChecker.volerrflag & 0x02))
			CurInfo.X11_WINDARROW1.Flag = 1;
		if (!(ParasChecker.volerrflag & 0x04))
			CurInfo.X12_SUNARROW1.Flag = 1;

		if (ParasChecker.chargeflag & 0x01)
			CurInfo.X13_MAINSARROW2.Flag = 1;
		if (ParasChecker.chargeflag & 0x02)
			CurInfo.X14_WINDARROW2.Flag = 1;
		if (ParasChecker.chargeflag & 0x04)
			CurInfo.X15_SUNARROW2.Flag = 1;

		//��ʾ��ص�ǰ����
		if (Vol.BAT > 40)
		{
			CurInfo.X16_BATARROW.Flag = 1;
			p = (_DispPoint *) &CurInfo.X22_BAT0;
			for (i = 0; i < ParasChecker.batpercent; i++)
			{
				p->Flag = 1;
				p++;
			}
			CurInfo.X23_BATVAL.Flag = 1;
		}

		if (Vol.IF > 2)                                        //��������������2A����Ϊ�и���
		{
			CurInfo.X17_LOAD.Flag = 1;
		}

		if (CurInfo.UserOption) //���û���������
		{
			CurInfo.UserOption--;
			ShowFromUserSelect();//��ʾ�û�ѡ�в���
		} else//û���û����������ݵ�ǰ���״̬��ʾ
			ShowFromPowerIn();

		if (ParasChecker.errflag & 0x04)//���ڱ�������ʾ��������
		{
			if (CurInfo.isShow)
				ShowErrCode(3);
		}

		if (Temputer.flag & 0x01)//�¶ȳ��ޣ���ʾE-10
		{
			ClearShowFlag();
			ShowTempWarn();
		}
	} else//�ȴ������ɼ���ɣ���ʾ"-"
	{
		CurInfo.waitCnt++;
		if (CurInfo.waitCnt >= 7)
			CurInfo.waitCnt = 0;
		for (i = 0; i < CurInfo.waitCnt; i++)
			NumRam[i] = 0x40;
		for (; i < 6; i++)
			NumRam[i] = 0;
	}

	Num2Flag();
	DataToLcdRam();                //��ʾ���ݸ��µ�LCD������ʾ���棬ˢ��LCD��ʾ

	CurInfo.CurDispCnt++;
	if (CurInfo.CurDispCnt >= 40) //ÿ��������ʾ4��
	{
		CurInfo.windSun ^= 0x01;
		CurInfo.CurDispCnt = 0;
		CurInfo.BatItem++;
		if (CurInfo.BatItem > INFREQ)
			CurInfo.BatItem = BATVOL;
	}

	if ((CurInfo.CurDispCnt != 0) && ((CurInfo.CurDispCnt % 20) == 0))
	{
		CurInfo.ShowInOut++;
		if (CurInfo.ShowInOut > OUT)
			CurInfo.ShowInOut = IN;
	}

	CurInfo.FlashCnt++;
	if (CurInfo.FlashCnt >= 5)
	{
		CurInfo.FlashCnt = 0;
		CurInfo.isShow ^= 0x01;
	}
}

//*********************************************************���水������ӿ�*************************************************
static void EnterPage(void)
{
	FreshTask = CreateTask(0, 0, 0, 0, 100, FreshCurInfo);//����ʵʱ����ˢ������
}

static void ExitPage(void)
{
	DelTask(FreshTask);                                                                        //ɾ��ʵʱ����ˢ������
}

static void UpKeyHandler(void)
{

}

static void DownKeyHandler(void)
{

}

static void LeftKeyHandler(void)
{

}

static void RightKeyHandler(void)
{
	ParasChecker.currentflag &= ~0x01;
	ParasChecker.errflag &= ~0x02;                //������Ƿѹ���
	ParasChecker.errflag &= ~0x04;
}

static void EnterKeyHandler(void)
{
	CurInfo.UserOption = 600;                //������û�����,60���ڸ����û�������ʾϵͳ����
	CurInfo.BatItem = BATVOL;
	CurInfo.ShowItem++;                                //�û���һ���л���ʾ����
	if (CurInfo.ShowItem > SUNBAT)
		CurInfo.ShowItem = MAINSBAT;
}

static void ExitKeyHandler(void)
{

}
