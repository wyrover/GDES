// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include "SViewSwitch.h"

#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")

CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;
}

CMainDlg::~CMainDlg()
{
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 		MARGINS mar = {5,5,30,5};
	// 		DwmExtendFrameIntoClientArea ( m_hWnd, &mar );
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	return 0;
}


//TODO:消息映射
void CMainDlg::OnClose()
{
	PostMessage(WM_QUIT);
}

void CMainDlg::OnMaximize()
{
	//SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);

}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnDesktop()
{
	SViewSwitch *pViewSwitch = FindChildByName2<SViewSwitch>(L"vw_switch");
	if (pViewSwitch)
		pViewSwitch->SWitch(0);
}
void CMainDlg::OnUcenter()
{
	SViewSwitch *pViewSwitch = FindChildByName2<SViewSwitch>(L"vw_switch");
	if (pViewSwitch)
		pViewSwitch->SWitch(1);
}
void CMainDlg::OnGuard()
{
	SViewSwitch *pViewSwitch = FindChildByName2<SViewSwitch>(L"vw_switch");
	if (pViewSwitch)
		pViewSwitch->SWitch(2);
}
void CMainDlg::OnDiagnosescan()
{
	SViewSwitch *pViewSwitch = FindChildByName2<SViewSwitch>(L"vw_switch");
	if (pViewSwitch)
		pViewSwitch->SWitch(3);
}