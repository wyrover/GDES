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

// TestDlg ��Ϣ�������

BOOL TestDialog::OnInitDialog()
{
	AcadDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}