#pragma once

#include "AcadDialog.h"
#include "Resource.h"

class TestDialog : public AcadDialog
{
	DECLARE_DYNAMIC(TestDialog)

public:
	TestDialog(CWnd* pParent = NULL, BOOL bModal = FALSE);
	~TestDialog();

	enum { IDD = IDD_TEST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	//��ʼ���Ի���
public:
	virtual BOOL OnInitDialog();
	//ģ�¶Ի����OK��Cancel��ť����
};
