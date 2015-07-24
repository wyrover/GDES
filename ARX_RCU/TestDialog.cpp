#include "stdafx.h"
#include "TestDialog.h"

IMPLEMENT_DYNAMIC(TestDialog, AcadDialog)

TestDialog::TestDialog(CWnd* pParent, BOOL bModal)
	: AcadDialog(TestDialog::IDD, pParent, bModal)
{

}

TestDialog::~TestDialog()
{
}

void TestDialog::DoDataExchange(CDataExchange* pDX)
{
	AcadDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(TestDialog, AcadDialog)
END_MESSAGE_MAP()

// TestDlg 消息处理程序

BOOL TestDialog::OnInitDialog()
{
	AcadDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}