#include "duilib.h"


CPalletDialog::CPalletDialog(LPCTSTR pszXMLPath, CSkinPickerDialog *pSkinPickerWnd) :
	m_strXMLPath(pszXMLPath),
	m_pSkinPickerWnd(pSkinPickerWnd),
	m_pBtnCancel(NULL),
	m_pBtnOk(NULL),
	m_pPallet(NULL),
	m_SelectedColor(0)
{
}

CPalletDialog::~CPalletDialog()
{

}

LPCTSTR CPalletDialog::GetWindowClassName() const 
{ 
	return _T("UIPalletDialog");
}

UINT CPalletDialog::GetClassStyle() const 
{ 
	return CS_DBLCLKS;
}


void CPalletDialog::OnFinalMessage(HWND /*hWnd*/) 
{ 

	return;
}

void CPalletDialog::Init()
{
	 m_pBtnCancel = static_cast<CButtonUI*>(m_pm.FindControl(kPalletCancel));
	 m_pBtnOk = static_cast<CButtonUI*>(m_pm.FindControl(kPalletOk));
	 m_pPallet = static_cast<CColorPaletteUI*>(m_pm.FindControl(kPalletPallet));

	SetBkColor(m_pSkinPickerWnd->GetBkColor());
	SetBkImage(m_pSkinPickerWnd->GetBkImage());


	CWndShadow::Initialize(m_pm.GetInstance());
	m_WndShadow.Create(m_hWnd , true);
	m_WndShadow.SetSize(4);
	m_WndShadow.SetPosition(1, 1);
}

CControlUI* CPalletDialog::CreateControl(LPCTSTR pstrClass) 
{
	if (_tcsicmp(pstrClass, kPalletInterface) == 0)
		return	new CColorPaletteUI();

	return NULL;
}

DWORD CPalletDialog::GetBkColor() const
{
	CControlUI* background = m_pm.FindControl(kWndBackGround);
	if (background != NULL)
		return background->GetBkColor();
	return 0;
}

void CPalletDialog::SetBkColor(DWORD dwBackColor)
{
	CControlUI* background = m_pm.FindControl(kWndBackGround);
	if (background != NULL)
	{
		background->SetBkImage(_T(""));
		background->SetBkColor(dwBackColor);
		background->NeedUpdate();
	}
	if (m_pPallet)
	{
		m_pPallet->SetSelectColor(dwBackColor);
	}
}

void CPalletDialog::SetBkImage(CDuiString strBkImage)
{
	CControlUI* background = m_pm.FindControl(kWndBackGround);
	if (background != NULL)
		background->SetBkImage(strBkImage.GetData());
}

LPCTSTR CPalletDialog::GetBkImage()
{
	CControlUI* background = m_pm.FindControl(kWndBackGround);
	if (background != NULL)
		return background->GetBkImage();
	return _T("");
}

void CPalletDialog::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click") ) 
	{
		if( msg.pSender->GetName() == _T("closebtn")) 
		{
			Close(IDCANCEL);
			return; 
		}
		if( msg.pSender->GetName() == _T("okbtn")) 
		{
			if (m_pPallet)
			{
				m_SelectedColor = m_pPallet->GetSelectColor();
				Close(IDOK);
				return; 
			} 
			else
			{
				Close(IDCANCEL);
				return; 
			}
			
		}
	}

}

DWORD CPalletDialog::GetSelectColor()
{
	return m_SelectedColor;
}

LRESULT CPalletDialog::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLongPtr(*this, GWL_STYLE);
//	styleValue &= ~WS_CAPTION;
	::SetWindowLongPtr(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	CDialogBuilder builder;

	CControlUI *pRoot = builder.Create(m_strXMLPath.GetData(), (UINT)0, this, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();
	return 0;
}

LRESULT CPalletDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CPalletDialog::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CPalletDialog::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if( ::IsIconic(*this) ) bHandled = FALSE;
    return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CPalletDialog::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CPalletDialog::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CPalletDialog::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_pm.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
			if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
				_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CPalletDialog::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
	if( !::IsIconic(*this) /*&& (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)*/ ) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
#endif
	bHandled = FALSE;
    return 0;
}

LRESULT CPalletDialog::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT CPalletDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		bHandled = TRUE;
		SendMessage(WM_CLOSE);
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed ) {
	}
	return lRes;
}

LRESULT CPalletDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	default:
	bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

