#include "StdAfx.h"
#include "resource.h"
#include "UIHelper.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"
#include "../ARX_ReportHelper/ReportHelper.h"

// ����ע���������
#ifndef GESES_CMD_SERVICE_NAME
#define GESES_CMD_SERVICE_NAME _T("GESESCMDS_SERVICE_NAME")
#endif

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CGESESCmdsApp : public AcRxArxApp {

public:
	CGESESCmdsApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		acrxRegisterService( GESES_CMD_SERVICE_NAME );
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		delete acrxServiceDictionary->remove( GESES_CMD_SERVICE_NAME );
		// TODO: Unload dependencies here

		UIHelper::DestroyDataListDockBar();
		return (retCode) ;
	}

	static void JL_GasReport( void )
	{
		//AfxGetMainWnd()->BeginWaitCursor();//���õȴ����
		UIHelper::CreatReport();
		//AfxGetMainWnd()->EndWaitCursor();//�����ȴ����
	}

	static void JL_ShowPathDlg( void )
	{
		AcDbObjectId sObjId = ArxUtilHelper::SelectObject( _T( "\n��ѡ��һ����˹��·��Ϊ�����ġ�Դ�㡿:" ) );
		if( sObjId.isNull() ) return;
		if( !ArxUtilHelper::IsEqualType( _T( "GasTube" ), sObjId ) ) return;

		AcDbObjectId tObjId = ArxUtilHelper::SelectObject( _T( "\n��ѡ��һ����˹��·��Ϊ�����ġ���㡿:" ) );
		if( tObjId.isNull() ) return;
		if( !ArxUtilHelper::IsEqualType( _T( "GasTube" ), tObjId ) ) return;

		//acutPrintf(_T("\nsid:%ld, tid:%ld"), sObjId.asOldId(), tObjId.asOldId());
		UIHelper::ShowDataListDockBar(sObjId, tObjId);
	}

	static void JL_SetGESize( void )
	{
		UIHelper::SetGESize();
	}

	virtual void RegisterServerComponents () {
	}

	static void JL_OpenRules()
	{
		ReportHelper::OpenCHMFile();
	}

	static void JL_OpenRulesOwn()
	{
		ReportHelper::OpenCHMFile2();
	}

	static void JL_OpenHelpCHM()
	{
		ReportHelper::OpenHelpCHMFile();
	}

	static void JL_CreatBaseReport()
	{
		ReportHelper::CreatBaseReport();
	}

	static void JL_CreatDrillReport()
	{
		ReportHelper::CreatDrillReport();
	}

	static void JL_GasSysDlg()
	{
		ReportHelper::ListShow();
	}

	static void JL_GasPlanDlg()
	{
		ReportHelper::ShowGasPlanDlg();
	}

	static void JL_EPCMFDDlg()
	{
		ReportHelper::ShowEPCMFDDlg();
	}

	static void JL_SelfManageDlg()
	{
		ReportHelper::ShowSelfManageDlg();
	}

	static void JL_InspectionDataDlg()
	{
		ReportHelper::ShowInspectionDataDlg();
	}

	static void JL_GasPumpDlg()
	{
		ReportHelper::ShowGasPumpDlg();
	}

	static void JL_MeasureDlg()
	{
		ReportHelper::ShowMeasureDlg();
	}

	static void JL_ReleatedInfoDlg()
	{
		ReportHelper::ShowReleatedInfoDlg();
	}

	static void JL_PumpSelect()
	{
		UIHelper::SelectPump();
	}

	static void JL_PumpsSelect()
	{
		UIHelper::SelectPumps();
	}

	static void JL_ShowDatabaseManagerDlg()
	{
		UIHelper::DatabaseManagerDlg();
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CGESESCmdsApp)
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _GasReport, GasReport, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _ShowPathDlg, ShowPathDlg, ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _SetGESize, SetGESize, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _OpenRules, OpenRules, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _OpenRulesOwn, OpenRulesOwn, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _OpenHelpCHM, OpenHelpCHM, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _CreatBaseReport, CreatBaseReport, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _CreatDrillReport, CreatDrillReport, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _GasSysDlg, GasSysDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _GasPlanDlg, GasPlanDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _EPCMFDDlg, EPCMFDDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _SelfManageDlg, SelfManageDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _InspectionDataDlg, InspectionDataDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _GasPumpDlg, GasPumpDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _MeasureDlg, MeasureDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _ReleatedInfoDlg, ReleatedInfoDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _PumpSelect, PumpSelect, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL, _PumpsSelect, PumpsSelect, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGESESCmdsApp, JL,_ShowDatabaseManagerDlg, ShowDatabaseManagerDlg, ACRX_CMD_TRANSPARENT, NULL )