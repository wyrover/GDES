#include "StdAfx.h"
#include "resource.h"

// �����ļ�
#include "../LoadConfig.h"

// ϵͳ����������
// ���ڵ����ꡢ����������ж�
#include "TolSetter.h"

#include "../MineGEDraw/MineGEDrawSystem.h"
#include "../ArxHelper/HelperClass.h"

// ��ȡ��ǰģ���·��
static CString GetAppPathDir()
{
	TCHAR szMoudlePath[_MAX_PATH];
	GetModuleFileName( _hdllInstance, szMoudlePath, _MAX_PATH );

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	_tsplitpath( szMoudlePath, drive, dir, NULL, NULL );

	TCHAR path[_MAX_PATH];
	_tmakepath( path, drive, dir, NULL, NULL );

	return CString( path );
}

// ����·��
static CString BuildPath( const CString& dir, const CString& fileName )
{
	CString path;
	path.Format( _T( "%s%s" ), dir, fileName );
	return path;
}

// ϵͳ����������ȫ�ֱ���
TolSetter* pTS = 0;

static CString BuildArxFileName( const CString& arxModuleName )
{
	CString str;
	str.Format( _T( "%s.arx" ), arxModuleName );
	return str;
}

static CString BuildServiceName( const CString& arxModuleName )
{
	CString str;
	str.Format( _T( "%s_SERVICE_NAME" ), arxModuleName );
	return str.MakeUpper();
}

static void SetWindowTitle()
{
	//CWnd *pp= AfxGetMainWnd();
	CMDIFrameWnd *pp = acedGetAcadFrame();
	//CString appName = pp->GetWindowTitle();
	//acutPrintf(_T("\n������:%s"),appName);
	//acedGetAcadFrame()->ModifyStyle(0,WS_SYSMENU|WS_MAXIMIZEBOX|WS_MINIMIZEBOX); 
	//AfxGetMainWnd()->SetWindowText(_T("��˹��ɴ������ϵͳ(AutoCAD 2010��)"));
	pp->SetWindowText (_T("��˹��ɴ������ϵͳ"));
	HICON hIcon =AfxGetApp()->LoadIcon(IDI_ICON1);
	pp->SetIcon(hIcon,TRUE);
	pp->SetIcon(hIcon,FALSE);
	pp->UpdateWindow ();
}

class CVVLoaderApp : public AcRxArxApp
{

public:
	CVVLoaderApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
	{
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

		//��ʼ��log4cplus��־ϵͳ
		//Ϊ�˱�֤��־��������ʹ�ã��ڼ�������ģ��֮ǰ��ʼ����־ϵͳ
		//log_init(_T(".\\Datas\\log4cplus.properties"));

		// �޸�cadϵͳȫ�־���
		pTS = new TolSetter( GLOBAL_TOLERANCE );

		// ��ʼ�����ӻ�ϵͳ
		if( !initDrawSystem() ) return AcRx::kRetError;

		// ����ͼԪģ��
		if( !loadGEModule() ) return AcRx::kRetError;

		// ��������ģ��
		if( !loadCmdModule() ) return AcRx::kRetError;

		// ����רҵ����ģ��
		if( !loadFunctionModule() ) return AcRx::kRetError;

		acutPrintf( _T( "\nVVLoader::On_kInitAppMsg\n" ) );

		SetWindowTitle();

		return ( retCode ) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
	{

		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

		// �ָ�cadϵͳ��ȫ�־���
		delete pTS;
		pTS = 0;

		// ж��רҵ����ģ��
		unloadFunctionModule();

		// ж������ģ��
		unloadCmdModule();

		// ж��ͼԪģ��
		unloadGEModule();

		// �˳����ӻ�ϵͳ
		unInitDrawSystem();

		acutPrintf( _T( "\nVVLoader::On_kUnloadAppMsg\n" ) );

		//�ر�log4cplus��־ϵͳ
		//log_uinit();

		return ( retCode ) ;
	}

	virtual void RegisterServerComponents ()
	{
	}

	bool loadArxModule( const CString& arxModuleName )
	{
		CString arxName = BuildArxFileName( arxModuleName );
		CString serviceName = BuildServiceName( arxModuleName );

		AcRxObject* pSvc = acrxServiceDictionary->at ( serviceName );
		if( pSvc == NULL )
		{
			if( !acrxDynamicLinker->loadModule( arxName, false ) )
			{
				acutPrintf ( _T( "\n����%sģ��ʧ��...\n" ), arxName ) ;
				return false;
			}
			acutPrintf ( _T( "\n�ɹ�����%sģ��...\n" ), arxName ) ;
		}
		else
		{
			acutPrintf ( _T( "\n%sģ���Ѿ����ع���...\n" ), arxName ) ;
		}
		return true;
	}

	void unloadArxModule( const CString& arxModuleName )
	{
		acrxDynamicLinker->unloadModule( BuildArxFileName( arxModuleName ), 0 );
		acutPrintf( _T( "\nж��ģ��:%s\n" ), arxModuleName );
	}

	bool initDrawSystem()
	{
		if( !loadArxModule( _T( "MineGEDraw" ) ) ) return false;
		//if( !loadArxModule( _T( "ContextMenuHelper" ) ) ) return false;
		// ��ʼ��MineGEDrawSystem
		MineGEDrawSystem::CreateInstance();
		MineGEDrawSystem* pDrawSystem = MineGEDrawSystem::GetInstance();

#if INCLUDE_VENT_MODULE
		pDrawSystem->loadPlugin( BuildPath( GetAppPathDir(), _T( "DefGEPlugin.arx" ) ) );
#endif

#if INCLUDE_GAS_MODULE
		pDrawSystem->loadPlugin( BuildPath( GetAppPathDir(), _T( "GasGEPlugin.arx" ) ) );
#endif

#if INCLUDE_FFC_MODULE
		pDrawSystem->loadPlugin( BuildPath( GetAppPathDir(), _T( "FFC_GEPlugin.arx" ) ) );
#endif

		return true;
	}

	void unInitDrawSystem()
	{
		// �˳�������MineGEServiceϵͳ
		MineGEDrawSystem::DestroyInstance();

		unloadArxModule( _T( "MineGEDraw" ) );
		//unloadArxModule( _T("ContextMenuHelper") );
	}

	// ����ͼԪģ��
	bool loadGEModule()
	{
		acutPrintf( _T( "\n-------- ����ͼԪģ�� ------------\n" ) );

		if( !loadArxModule( _T( "MineGE" ) ) ) return false;

#if INCLUDE_VENT_MODULE
		if( !loadArxModule( _T( "DefGE" ) ) ) return false;
#endif

#if INCLUDE_GAS_MODULE
		if( !loadArxModule( _T( "GasGE" ) ) ) return false;
#endif

#if INCLUDE_FFC_MODULE
		if( !loadArxModule( _T( "FFC_GE" ) ) ) return false;
#endif

		return true;
	}

	void unloadGEModule()
	{
		acutPrintf( _T( "\n-------- ж��ͼԪģ�� ------------" ) );

#if INCLUDE_FFC_MODULE
		unloadArxModule( _T( "FFC_GE" ) );
#endif

#if INCLUDE_GAS_MODULE
		unloadArxModule( _T( "GasGE" ) );
#endif

#if INCLUDE_VENT_MODULE
		unloadArxModule( _T( "DefGE" ) );
#endif

		unloadArxModule( _T( "MineGE" ) );
	}

	bool loadCmdModule()
	{
		acutPrintf( _T( "\n-------- ��������ģ�� ------------" ) );

		if( !loadArxModule( _T( "MineGECmds" ) ) ) return false;

#if INCLUDE_VENT_MODULE
		if( !loadArxModule( _T( "DrawVentCmd" ) ) ) return false;
#endif

#if INCLUDE_TVNS_MODULE
		if( !loadArxModule( _T( "ARX_ReportHelper" ) ) ) return false;
		if( !loadArxModule( _T( "TVNSCacul" ) ) ) return false;
#endif

#if INCLUDE_GESES_MODULE
		if( !loadArxModule( _T( "GraphTool" ) ) ) return false;
		if( !loadArxModule( _T( "ARX_ReportHelper" ) ) ) return false;
		if( !loadArxModule( _T( "GESESCmds" ) ) ) return false;
		if( !loadArxModule( _T( "GDESAddFunc" ) ) ) return false;
		if( !loadArxModule( _T( "ARX_RCU" ) ) ) return false;
#endif

#if INCLUDE_GAS_MODULE
		if( !loadArxModule( _T( "DrawGasCmd" ) ) ) return false;
#endif

#if INCLUDE_FFC_MODULE
		if( !loadArxModule( _T( "DrawFFCCmd" ) ) ) return false;
#endif
		return true;
	}

	void unloadCmdModule()
	{
		acutPrintf( _T( "\n-------- ж������ģ�� ------------" ) );

		unloadArxModule( _T( "MineGECmds" ) );

#if INCLUDE_VENT_MODULE
		unloadArxModule( _T( "DrawVentCmd" ) );
#endif

#if INCLUDE_TVNS_MODULE
		unloadArxModule( _T( "ARX_ReportHelper" ) );
		unloadArxModule( _T( "TVNSCacul" ) );
#endif

#if INCLUDE_GESES_MODULE
		unloadArxModule( _T( "ARX_RCU" ) );
		unloadArxModule( _T( "GDESAddFunc" ) );
		unloadArxModule( _T( "ARX_ReportHelper" ) );
		unloadArxModule( _T( "GESESCmds" ) ) ;
		unloadArxModule( _T( "GraphTool" ) ) ;
#endif

#if INCLUDE_GAS_MODULE
		unloadArxModule( _T( "DrawGasCmd" ) );
#endif

#if INCLUDE_FFC_MODULE
		unloadArxModule( _T( "DrawFFCCmd" ) );
#endif

	}

	bool loadFunctionModule()
	{
		acutPrintf( _T( "\n-------- ���ع���ģ�� ------------" ) );

		if( !loadArxModule( _T( "DataInit" ) ) ) return false;

#if INCLUDE_EVAL_MODULE
		if( !loadArxModule( _T( "VentNetworkAlgo" ) ) ) return false;
		if( !loadArxModule( _T( "VentEvaluate" ) ) ) return false;
#endif

#if INCLUDE_VNC_MODULE
		if( !loadArxModule( _T( "ARX_VNC" ) ) ) return false;
#endif

#if INCLUDE_MVSS_MODULE
		if( !loadArxModule( _T( "ARX_MVSS" ) ) ) return false;
#endif

#if INCLUDE_GAS_MODULE
		if( !loadArxModule( _T( "ARX_Contour" ) ) ) return false;
#endif

#if INCLUDE_VNG_GEN_MODULE
		if( !loadArxModule( _T( "ARX_VNGGen" ) ) ) return false;
#endif

#if INCLUDE_FFC_MODULE
		if( !loadArxModule( _T( "ARX_FFC" ) ) ) return false;
#endif

#if INCLUDE_TEST_MODULE
		if( !loadArxModule( _T( "AllTests" ) ) ) return false;
#endif

		if( !loadArxModule( _T( "ArxSoUI" ) ) ) return false;

		return true;
	}

	void unloadFunctionModule()
	{
		acutPrintf( _T( "\n-------- ж�ع���ģ�� ------------" ) );

		unloadArxModule( _T( "DataInit" ) );

#if INCLUDE_EVAL_MODULE
		unloadArxModule( _T( "VentNetworkAlgo" ) );
		unloadArxModule( _T( "VentEvaluate" ) );
#endif

#if INCLUDE_VNC_MODULE
		unloadArxModule( _T( "ARX_VNC" ) );
#endif

#if INCLUDE_MVSS_MODULE
		unloadArxModule( _T( "ARX_MVSS" ) );
#endif

#if INCLUDE_GAS_MODULE
		unloadArxModule( _T( "ARX_Contour" ) );
#endif

#if INCLUDE_VNG_GEN_MODULE
		unloadArxModule( _T( "ARX_VNGGen" ) );
#endif

#if INCLUDE_FFC_MODULE
		unloadArxModule( _T( "ARX_FFC" ) );
#endif

#if INCLUDE_TEST_MODULE
		unloadArxModule( _T( "AllTests" ) );
#endif

		unloadArxModule( _T( "ArxSoUI" ) );
	}

	static void VVTest_testVector()
	{
		const double PI = 3.1415926535897932384626433832795;
		AcGeVector3d v(AcGeVector3d::kXAxis);
		v.rotateBy(PI/6, AcGeVector3d::kZAxis);

		acutPrintf(_T("\n%.3f, %.3f, %.3f"), v.x, v.y, v.z);

		AcGeVector3d vv(-1,1,0);
		double ang = vv.angleTo(AcGeVector3d::kYAxis, AcGeVector3d::kZAxis);
		acutPrintf(_T("\n�Ƕ�:%.3f"), ang);
/*
	    AcGeVector3d v1( 100, 20, 0 );
	    AcGeVector3d v2( 10, 50, 1 );
	    AcGeVector3d v3 = v1.crossProduct( v2 );
	    AcGeVector3d v4 = v2.crossProduct( v1 );

	    acutPrintf( _T( "\n���z1=%.3f, z2:%.3f" ), v3.z, v4.z );

	    AcGePoint3d p1, p2( 1, 1, 0 ), p3( 2, 0, 0 ), p4( 1, -1, 0 );
	    AcGeCircArc3d arc( p1, p2, p3 );
	    AcGeCircArc3d arc2( p3, p2, p1 );
	    AcGeCircArc3d arc3( p2, p4, p3 );
	    AcGeCircArc3d arc4( p1, p3, p4 );

	    acutPrintf( _T( "\n����z1=%.3f, z2:%.3f" ), arc.normal().z, arc2.normal().z );
	    acutPrintf( _T( "\n[1]start angle:%.3f, end angle:%.3f" ), arc.startAng(), arc.endAng() );
	    acutPrintf( _T( "\n[2]start angle:%.3f, end angle:%.3f" ), arc2.startAng(), arc2.endAng() );
	    acutPrintf( _T( "\n[3]start angle:%.3f, end angle:%.3f" ), arc3.startAng(), arc3.endAng() );
	    acutPrintf( _T( "\n[4]start angle:%.3f, end angle:%.3f" ), arc4.startAng(), arc4.endAng() );

	    AcGeVector3d v5( 0, -1, 0 ), v6( 0, 1, 0 );
	    double a1 = v5.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
	    double a2 = v6.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

	    acutPrintf( _T( "\na1:%.3f, a2:%.3f" ), a1, a2 );

	    ads_point pt1, pt2, pt3, pt4;
	    ads_name ssname;
	    pt1[X] = pt1[Y] = pt1[Z] = 0.0;
	    pt2[X] = pt2[Y] = 5.0;
	    pt2[Z] = 0.0;

	    // Get the current PICKFIRST set, if there is one;
	    // otherwise, ask the user for a general entity selection.
	    acedSSGet( _T( ":S" ), NULL, NULL, NULL, ssname );

	    acutPrintf( _T( "\nxxx\n" ) );
	    //acedSSGet(NULL, NULL, NULL, NULL, ssname);

		AcGeVector3d v(-1, 1, 0);
		acutPrintf(_T("\n...........\n"));
		acutPrintf(_T("\nͶӰǰ:(%.3f, %.3f, %.3f)"), v.x, v.y, v.z);

		v = v.dotProduct( AcGeVector3d::kXAxis ) * AcGeVector3d::kXAxis;
		acutPrintf(_T("\nͶӰ��:(%.3f, %.3f, %.3f)"), v.x, v.y, v.z);
*/
	}

} ;

IMPLEMENT_ARX_ENTRYPOINT( CVVLoaderApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CVVLoaderApp, VVTest, _testVector, tvec, ACRX_CMD_TRANSPARENT, NULL )