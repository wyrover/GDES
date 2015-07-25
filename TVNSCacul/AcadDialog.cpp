#include "stdafx.h"
#include "AcadDialog.h"

IMPLEMENT_DYNAMIC(AcadDialog, CDialog)

AcadDialog::AcadDialog(UINT nIDTemplate, CWnd* pParent, BOOL bModal)
	: CDialog(nIDTemplate, pParent),
	m_mouseInWindow(TRUE), 
	m_bTracking(FALSE),
	m_bModal(bModal),
	m_nIDTemplate(nIDTemplate)
{

}

AcadDialog::~AcadDialog()
{
}

void AcadDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(AcadDialog, CDialog)
	ON_WM_CLOSE()
	ON_MESSAGE( WM_ACAD_KEEPFOCUS, OnAcadKeepFocus )  // Needed for modeless dialog.
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

INT_PTR AcadDialog::Run()
{
	if(m_bModal)
	{
		return this->DoModal();
	}
	else
	{
		this->Create(m_nIDTemplate, GetParent());
		this->SendMessage(WM_INITDIALOG);
		this->CenterWindow();
		return this->ShowWindow(SW_SHOW);
	}
}

// TestDlg 消息处理程序

BOOL AcadDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void AcadDialog::OnCancel()
{
	m_bTracking = FALSE;
	if(m_bModal)
	{
		CDialog::OnCancel();
	}
	else
	{
		DestroyWindow();
	}
}

void AcadDialog::OnOK()
{
	m_bTracking = FALSE;
	if(m_bModal)
	{
		CDialog::OnOK();
	}
	else
	{
		DestroyWindow();
	}
}

void AcadDialog::OnClose()
{
	m_bTracking = FALSE;
	AcadDialog::OnCancel();
}

void AcadDialog::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	if(!m_bModal)
	{
		//一般非模态的对话框都是new出来的,而模态则是局部变量
		delete this;
	}
}

void AcadDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bModal && !m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE | TME_HOVER;//要触发的消息
		tme.hwndTrack = this->m_hWnd;
		tme.dwHoverTime = 50;// 若不设此参数，则无法触发mouseHover

		if (::_TrackMouseEvent(&tme)) //MOUSELEAVE|MOUSEHOVER消息由此函数触发
		{
			m_bTracking = TRUE;   
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void AcadDialog::OnMouseHover(UINT nFlags, CPoint point)
{
	if(!m_bModal)
	{
		m_bTracking = FALSE;
		m_mouseInWindow = TRUE;
	}
	CDialog::OnMouseHover(nFlags, point);
}

void AcadDialog::OnMouseLeave()
{
	if(!m_bModal)
	{
		m_bTracking = FALSE;
		m_mouseInWindow = FALSE;
	}
	CDialog::OnMouseLeave();
}

//- Needed for modeless dialogs to keep focus.
//- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT AcadDialog::OnAcadKeepFocus(WPARAM wParam, LPARAM lParam)
{
	//非模态条件下,如果鼠标在窗口内(m_mouseInWindow=TRUE),则认为窗口应该获取焦点
	return m_bModal?TRUE:m_mouseInWindow;
}

