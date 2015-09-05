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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	//初始化对话框
public:
	virtual BOOL OnInitDialog();
	//模仿对话框的OK和Cancel按钮处理
};
