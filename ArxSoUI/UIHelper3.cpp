#include "StdAfx.h"
#include "UIHelper.h"

#include "SoUILoader.h"
#include "MainDlg.h"
#include "SListCtrl2.h"
//#include "SouiRealWndHandler.h"
//#include "MemFlash.h"
//#include "httpsvr/HTTPServer.h"
//#include "uianimation/UiAnimationWnd.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

//#include <controls.extend/propgrid/SPropertyGrid.h>
//#include <controls.extend/SFlyWnd.h>
//#include <controls.extend/SFadeFrame.h>
//#include <controls.extend/sradiobox2.h>
#include <controls.extend/SVscrollbar.h>
//#include <controls.extend/SChromeTabCtrl.h>
//#include <controls.extend/siectrl.h>
//#include <controls.extend/SCalendar2.h>
#include <controls.extend/SIPAddressCtrl.h>
#include <controls.extend/SChatEdit.h>
#include <controls.extend/SScrollText.h>
#include <controls.extend/SListCtrlEx.h>
#include <controls.extend/gif/SGifPlayer.h>
#include <controls.extend/gif/SSkinGif.h>

using namespace SOUI;

class MySoUiLoader3 : public SoUILoader
{
public:
	MySoUiLoader3(HINSTANCE hInstance) : SoUILoader(hInstance)
	{

	}

	~MySoUiLoader3() 
	{
		//�˳�GDI+����
		SSkinGif::Gdiplus_Shutdown();
	}

	virtual LPCTSTR getSkinDir() const
	{
		return _T("\\..\\..\\Skin\\demo3");
	}

	virtual bool initExtendControls()
	{
		//��SApplicationϵͳ��ע�����ⲿ��չ�Ŀؼ���SkinObj��
		//SWkeLoader wkeLoader;
		//if(wkeLoader.Init(_T("wke.dll")))        
		//{
		//	pSouiApp->RegisterWndFactory(TplSWindowFactory<SWkeWebkit>());//ע��WKE�����
		//}
		pSouiApp->RegisterWndFactory(TplSWindowFactory<SGifPlayer>());//ע��GIFPlayer
		pSouiApp->RegisterSkinFactory(TplSkinFactory<SSkinGif>());//ע��SkinGif
		//pSouiApp->RegisterSkinFactory(TplSkinFactory<SSkinAPNG>());//ע��SSkinAPNG
		pSouiApp->RegisterSkinFactory(TplSkinFactory<SSkinVScrollbar>());//ע�����������Ƥ��

		pSouiApp->RegisterWndFactory(TplSWindowFactory<SIPAddressCtrl>());//ע��IP�ؼ�
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SPropertyGrid>());//ע�����Ա�ؼ�
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SChromeTabCtrl>());//ע��ChromeTabCtrl
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SIECtrl>());//ע��IECtrl
		pSouiApp->RegisterWndFactory(TplSWindowFactory<SChatEdit>());//ע��ChatEdit
		pSouiApp->RegisterWndFactory(TplSWindowFactory<SScrollText>());//ע��SScrollText
		pSouiApp->RegisterWndFactory(TplSWindowFactory<SListCtrlEx>());//ע��SListCtrlEx
		pSouiApp->RegisterWndFactory(TplSWindowFactory<SListCtrl2>()); //ע��SListCtrl2
		//if(SUCCEEDED(CUiAnimation::Init()))
		//{
		//	pSouiApp->RegisterWndFactory(TplSWindowFactory<SUiAnimationWnd>());//ע�ᶯ���ؼ�
		//}
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SFlyWnd>());//ע����ж����ؼ�
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SFadeFrame>());//ע�ὥ���������ؼ�
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SRadioBox2>());//ע��RadioBox2
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SCalendar2>());//ע��SCalendar2

		//��ʼ��GDI+����
		SSkinGif::Gdiplus_Startup();
		return true;
	}

	virtual bool initRealWnd()
	{
		//�����洰�ڴ���ӿ�
		//CSouiRealWndHandler * pRealWndHandler = new CSouiRealWndHandler();
		//pSouiApp->SetRealWndHandler(pRealWndHandler);
		//pRealWndHandler->Release();

		return SoUILoader::initRealWnd();
	}

	virtual bool initGlobalStyle()
	{
		//����ȫ����Դ����XML
		//��ʼ��SOUIȫ����Դ
		return (TRUE == pSouiApp->Init(_T("xml_init"), _T("uidef"))); 
	}
};

void UIHelper::ShowSoUIDlg3()
{
	// �л���Դ
	CAcModuleResourceOverride myResources;

	//LOG_TRACE(_T("�����µ�soui����"));

	//����soui��ص��齨����Դ
	MySoUiLoader3 suLoader(_hdllInstance);
	if(!suLoader.init()) return;

	//��������ʾʹ��SOUI����Ӧ�ó��򴰿�
	CMainDlg dlgMain;
	//dlgMain.Create(acedGetAcadFrame()->GetSafeHwnd(),0,0,800,600);
	dlgMain.Create(GetActiveWindow(),0,0,800,600);
	dlgMain.GetNative()->SendMessage(WM_INITDIALOG);
	dlgMain.CenterWindow();
	dlgMain.ShowWindow(SW_SHOWNORMAL);
	suLoader.getApp()->Run(dlgMain.m_hWnd);	
}