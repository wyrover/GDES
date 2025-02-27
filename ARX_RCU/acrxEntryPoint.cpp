#include "StdAfx.h"
#include "resource.h"
#include "ReactorHelper.h"
#include "UIHelper.h"
#include "RcuHelper.h"

#ifndef ARX_RCU_SERVICE_NAME
#define ARX_RCU_SERVICE_NAME _T("ARX_RCU_SERVICE_NAME")
#endif

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CARX_RCUApp : public AcRxArxApp {

public:
	CARX_RCUApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {

		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;

		//注册服务
		acrxRegisterService( ARX_RCU_SERVICE_NAME );
		//创建反应器
		//ReactorHelper::CreateDocumentReactorMap();
		acutPrintf( _T( "\nARX_RCU::On_kLoadAppMsg\n" ) );

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {

		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		//销毁反应器
		//ReactorHelper::RemoveDocumentReactorMap();
		//销毁停靠对话框
		UIHelper::DestroyRcuDesignDockBar();
		//卸载服务
		delete acrxServiceDictionary->remove( ARX_RCU_SERVICE_NAME );
		acutPrintf( _T( "\nARX_RCU::On_kUnloadAppMsg\n" ) );

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
	{
		AcRx::AppRetCode retCode = AcRxArxApp::On_kLoadDwgMsg ( pkt ) ;

		//注册文档相关的反应器
		//ReactorHelper::AddDocumentReactor( curDoc() );
		acutPrintf( _T( "\nARX_RCU::On_kLoadDwgMsg\n" ) );

		return retCode;
	}

	virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
	{
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadDwgMsg ( pkt ) ;

		//卸载文档反应器
		//ReactorHelper::RemoveDocumentReactor( curDoc() );
		acutPrintf( _T( "\nARX_RCU::On_kUnloadDwgMsg\n" ) );

		return retCode;
	}

	virtual void RegisterServerComponents () {
	}

	static void JL_RCUDlg()
	{
		UIHelper::ShowRcuDesignDockBar();
	}

	static void JL_RCUReport()
	{
		//RcuHelper::CreatReportHelper();
		//ReportHelper::CreatRCUReport();
	}

	static void JL_TestV()
	{
		AcGeVector3d v(1,1,1);
		AcGeVector3d normalV(AcGeVector3d::kYAxis);
		AcGeVector3d v2 = v.orthoProject(normalV);
		acutPrintf(_T("\n投影之后的坐标:(%.2f,%.2f,%.2f)"),v2.x,v2.y,v2.z);
		//RcuHelper::VectorToAngleTest();
	}

	static void JL_TestDlg1()
	{
		UIHelper::TestDlg1();
	}
	static void JL_TestDlg2()
	{
		UIHelper::TestDlg2();
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CARX_RCUApp)

ACED_ARXCOMMAND_ENTRY_AUTO( CARX_RCUApp, JL, _RCUDlg, RCUDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CARX_RCUApp, JL, _RCUReport, RCUReport, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CARX_RCUApp, JL, _TestV, TestV, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CARX_RCUApp, JL, _TestDlg1, TestDlg1, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CARX_RCUApp, JL, _TestDlg2, TestDlg2, ACRX_CMD_TRANSPARENT, NULL )

