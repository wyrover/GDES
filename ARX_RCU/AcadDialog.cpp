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

// TestDlg ��Ϣ�������

BOOL AcadDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
		//һ���ģ̬�ĶԻ�����new������,��ģ̬���Ǿֲ�����
		delete this;
	}
}

void AcadDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bModal && !m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE | TME_HOVER;//Ҫ��������Ϣ
		tme.hwndTrack = this->m_hWnd;
		tme.dwHoverTime = 50;// ������˲��������޷�����mouseHover

		if (::_TrackMouseEvent(&tme)) //MOUSELEAVE|MOUSEHOVER��Ϣ�ɴ˺�������
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
	//��ģ̬������,�������ڴ�����(m_mouseInWindow=TRUE),����Ϊ����Ӧ�û�ȡ����
	return m_bModal?TRUE:m_mouseInWindow;
}

