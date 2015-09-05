#include "StdAfx.h"
#include "UIHelper.h"
#include "RcuDesiginDockBarDlg.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

RcuDesiginDockBarDlg* pRcuDesiginDockBarDlg = 0;

void UIHelper::ShowRcuDesignDockBar()
{
	//CAcModuleResourceOverride myResources;

	//CMDIFrameWnd* pAcadFrame = acedGetAcadFrame();

	//if( pRcuDesiginDockBarDlg == 0 )
	//{
	//	pRcuDesiginDockBarDlg = new RcuDesiginDockBarDlg();
	//	pRcuDesiginDockBarDlg->Create ( pAcadFrame, _T("石门揭煤设计") ) ;
	//	pRcuDesiginDockBarDlg->EnableDocking ( CBRS_ALIGN_ANY ) ;
	//	pRcuDesiginDockBarDlg->RestoreControlBar () ;
	//}
	//pAcadFrame->ShowControlBar( pRcuDesiginDockBarDlg, TRUE, FALSE );

	////刷新界面
	//UIHelper::SendMessage(WM_RCU_UPDATE, 0);
	CAcModuleResourceOverride myResources;
	//非模态对话框(必须用new,否则会有内存错误)
	RcuDesignDlg* dlg = new RcuDesignDlg(acedGetAcadFrame(), FALSE);
	//关闭模态对话框后,会通过消息来删除内存
	dlg->Run();

}

void UIHelper::DestroyRcuDesignDockBar()
{
	if( pRcuDesiginDockBarDlg != 0 )
	{
		pRcuDesiginDockBarDlg->DestroyWindow();
		delete pRcuDesiginDockBarDlg;
		pRcuDesiginDockBarDlg = 0;
	}
}

void UIHelper::SendMessage(unsigned int msgCode, ArxMsg* msgParam)
{
	if(pRcuDesiginDockBarDlg != 0)
	{
		pRcuDesiginDockBarDlg->mChildDlg.SendMessage(msgCode, (WPARAM)msgParam, 0);
	}
}

#include "TestDialog.h"

void UIHelper::TestDlg1()
{
	CAcModuleResourceOverride myResources;
	//模态对话框
	TestDialog dlg(acedGetAcadFrame(), TRUE);
	dlg.Run();
}

void UIHelper::TestDlg2()
{
	CAcModuleResourceOverride myResources;
	//非模态对话框(必须用new,否则会有内存错误)
	TestDialog* dlg = new TestDialog(acedGetAcadFrame(), FALSE);
	//关闭模态对话框后,会通过消息来删除内存
	dlg->Run();
}