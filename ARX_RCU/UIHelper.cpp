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
	//	pRcuDesiginDockBarDlg->Create ( pAcadFrame, _T("ʯ�Ž�ú���") ) ;
	//	pRcuDesiginDockBarDlg->EnableDocking ( CBRS_ALIGN_ANY ) ;
	//	pRcuDesiginDockBarDlg->RestoreControlBar () ;
	//}
	//pAcadFrame->ShowControlBar( pRcuDesiginDockBarDlg, TRUE, FALSE );

	////ˢ�½���
	//UIHelper::SendMessage(WM_RCU_UPDATE, 0);
	CAcModuleResourceOverride myResources;
	//��ģ̬�Ի���(������new,��������ڴ����)
	RcuDesignDlg* dlg = new RcuDesignDlg(acedGetAcadFrame(), FALSE);
	//�ر�ģ̬�Ի����,��ͨ����Ϣ��ɾ���ڴ�
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
	//ģ̬�Ի���
	TestDialog dlg(acedGetAcadFrame(), TRUE);
	dlg.Run();
}

void UIHelper::TestDlg2()
{
	CAcModuleResourceOverride myResources;
	//��ģ̬�Ի���(������new,��������ڴ����)
	TestDialog* dlg = new TestDialog(acedGetAcadFrame(), FALSE);
	//�ر�ģ̬�Ի����,��ͨ����Ϣ��ɾ���ڴ�
	dlg->Run();
}