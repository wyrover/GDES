#pragma once

#include "MyMsg.h"

class UIHelper
{
public:
	//��ʾ�곡��Ʒ�ģ̬�Ի���
	static void ShowRcuDesignDockBar();
	//�����곡��ƶԻ���
	static void DestroyRcuDesignDockBar();
	//���곡��ƶԻ�������Ϣ
	static void SendMessage(unsigned int msgCode, ArxMsg* msgParam);

	static void TestDlg1();
	static void TestDlg2();
};
