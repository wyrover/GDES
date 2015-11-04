#include "StdAfx.h"
#include "ReportHelper.h"
#include "CreatReport.h"
#include "GasSysDlg.h"
#include "GasPlanDlg.h"
#include "EPCMFDDlg.h"
#include "SelfManageDlg.h"
#include "InspectionDataDlg.h"
#include "GasPumpDlg.h"
#include "MEREDlg.h"
#include "config.h"
#include "../MineGE/HelperClass.h"
#include "../ArxHelper/ArxUtilHelper.h"

static CString GetAppPathDir()
{
	TCHAR szModulePath[_MAX_PATH];
	GetModuleFileName( _hdllInstance, szModulePath, _MAX_PATH );

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	_tsplitpath( szModulePath, drive, dir, NULL, NULL );

	TCHAR szPath[_MAX_PATH] = {0};
	_tmakepath( szPath, drive, dir, NULL, NULL );

	return CString( szPath );
}

static CString BuildPath( const CString& dir, const CString& fileName )
{
	CString path;
	path.Format( _T( "%s%s" ), dir, fileName );
	return path;
}

static void GetDocPath( CString& defaultPath )
{
	TCHAR pPath[MAX_PATH]={0};
	SHGetSpecialFolderPath(NULL,pPath,CSIDL_PERSONAL,0);

	defaultPath.Format(_T("%s"),pPath);
}

static BOOL SaveAndOpenReport(CString tplName,CString outName,CString mineName = _T(""))
{
	TCHAR szFileFilter[] = _T("doc�ĵ�(*.doc)|*.doc||");
	TCHAR szFileExt[] = _T("doc");

	CString defaultPath;
	GetDocPath(defaultPath);

	CFileDialog dlg(FALSE,szFileExt,defaultPath,OFN_READONLY,szFileFilter);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���

	TCHAR* pFileBuf = new TCHAR[MAX_PATH * 1024];
	if (NULL == pFileBuf)
	{
		return FALSE;
	}
	_tcscpy(pFileBuf,outName);

	dlg.m_ofn.lpstrFile = pFileBuf;
	//dlg.m_ofn.lpstrFile[0] = NULL;
	dlg.m_ofn.nMaxFile = MAX_PATH * 1024;
	//dlg.m_ofn.lpstrInitialDir = (LPWSTR)(LPCTSTR)defaultPath;
	CString selectedPath;
	if(IDOK == dlg.DoModal())
	{
		selectedPath = dlg.GetPathName();
	}

	else
	{
		return FALSE;
	}

	delete pFileBuf;
	pFileBuf = NULL;

	acutPrintf( _T( "\n����������...\n" ) );
	//��ʼ��com
	if(initword())
	{
		if(!CreatReport(tplName,selectedPath,mineName)) 
		{
			//ж��com
			uninitword();
			acutPrintf( _T( "\n��������ʧ��!\n" ) );
			return FALSE;
		}
		//ж��com
		uninitword();
	}
	acutPrintf( _T( "\n���汣�浽:%s" ),selectedPath);
	if(IDYES == AfxMessageBox(_T("�������ɳɹ�!�Ƿ����ڴ�?"),MB_YESNO))
	{
		if(initword())
		{
			OpenWordDocument(selectedPath);
			//ж��com
			uninitword();
		}
	}
	return TRUE;
}

static BOOL SaveAndOpenReport(const CString& outName, AcDbObjectId& objId)
{
	TCHAR szFileFilter[] = _T("doc�ĵ�(*.doc)|*.doc||");
	TCHAR szFileExt[] = _T("doc");

	CString defaultPath;
	GetDocPath(defaultPath);

	CFileDialog dlg(FALSE,szFileExt,defaultPath,OFN_READONLY,szFileFilter);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���

	TCHAR* pFileBuf = new TCHAR[MAX_PATH * 1024];
	if (NULL == pFileBuf)
	{
		return FALSE;
	}
	_tcscpy(pFileBuf,outName);

	dlg.m_ofn.lpstrFile = pFileBuf;
	//dlg.m_ofn.lpstrFile[0] = NULL;
	dlg.m_ofn.nMaxFile = MAX_PATH * 1024;
	//dlg.m_ofn.lpstrInitialDir = (LPWSTR)(LPCTSTR)defaultPath;
	CString selectedPath;
	if(IDOK == dlg.DoModal())
	{
		selectedPath = dlg.GetPathName();
	}

	else
	{
		return FALSE;
	}

	delete pFileBuf;
	pFileBuf = NULL;

	acutPrintf( _T( "\n����������...\n" ) );
	//��ʼ��com
	if(initword())
	{
		if(!CreatReport(selectedPath,objId)) 
		{
			//ж��com
			uninitword();
			acutPrintf( _T( "\n��������ʧ��!\n" ) );
			return FALSE;
		}
		//ж��com
		uninitword();
	}
	acutPrintf( _T( "\n���汣�浽:%s" ),selectedPath);
	if(IDYES == AfxMessageBox(_T("�������ɳɹ�!�Ƿ����ڴ�?"),MB_YESNO))
	{
		if(initword())
		{
			OpenWordDocument(selectedPath);
			//ж��com
			uninitword();
		}
	}
	return TRUE;
}

//���������������
static void DealReportName(const AcDbObjectId& objId, CString& fileName)
{
	if(objId.isNull())
	{
		fileName = _T("����") + fileName;
		return;
	}
	CString name;
	DataHelper::GetPropertyData(objId,_T("�������"),name);
	if(name.IsEmpty())
	{
		fileName = _T("(������)") + fileName;
	}
	else
	{
		fileName =name + _T("#") + fileName;
	}
}

static bool CheackDrillGE(CString& outName)
{
	AcDbObjectIdArray objIds;
	DrawHelper::FindMineGEs( _T( "DrillGE" ), objIds );
	if( objIds.isEmpty() ) 
	{
		AfxMessageBox(_T("ϵͳ��δ������ף�"));
		return false;
	}
	outName =_T("��λ��ײ���������");
	return true;
}

void ReportHelper::CreatReportHelper()
{
	CAcModuleResourceOverride myResources;

	AcDbObjectId objId;
	SingleDataObjectHelper::GetObjectId(_T("�������"),objId);
	CString gasFromStr;
	DataHelper::GetPropertyData(objId,_T("��������˹��Դ"),gasFromStr);

	CString fileName,tplName,outName;
	if (0 == _ttoi(gasFromStr))
	{
		fileName = _T("Datas\\JL\\tpl\\tplOwn.doc");
		outName = _T("��˹���(��ú��)���ⱨ��");
	}
	else
	{
		fileName = _T("Datas\\JL\\tpl\\tplNear.doc");
		outName = _T("��˹���(�ڽ�ú��)���ⱨ��");
	}
	tplName = BuildPath( GetAppPathDir(), fileName );
	if(!SaveAndOpenReport(tplName,outName)) return;
}

void ReportHelper::OpenCHMFile()
{
	CString filePath = _T("help\\��˹��ɴ�����й涨.pdf");
	OpenWordDocument(filePath,false);
}

//�����Լ�������
void ReportHelper::OpenCHMFile2()
{
	CString filePath = _T("help\\��˹��ɴ�����۹�����ϵ(����).pdf");
	OpenWordDocument(filePath,false);
}

//�����ĵ���˵����
void ReportHelper::OpenHelpCHMFile()
{
	CString filePath = _T("help\\doc.pdf");
	OpenWordDocument(filePath,false);
}

void ReportHelper::CreatBaseReport()
{
	CAcModuleResourceOverride myResources;

	TCHAR szFileFilter[] = _T("doc�ĵ�(*.doc)|*.doc||");
	TCHAR szFileExt[] = _T("doc");
	CString defaultPath;
	GetDocPath(defaultPath);
	AcDbObjectId objId;
	SingleDataObjectHelper::GetObjectId(_T("�������"),objId);
	CString mineName;
	DataHelper::GetPropertyData(objId,_T("����"),mineName);

	CString fileName =  _T("Datas\\JL\\tpl\\tplBase.doc");
	CString outName = mineName + _T("��˹��ɻ��������������");
	CString tplName = BuildPath( GetAppPathDir(), fileName );

	tplName = BuildPath( GetAppPathDir(), fileName );
	if(!SaveAndOpenReport(tplName,outName,mineName)) return;
}

void ReportHelper::CreatDrillReport()
{
	CAcModuleResourceOverride myResources;

	CString outName;
	if(!CheackDrillGE(outName)) return;
	AcDbObjectId objId = AcDbObjectId::kNull;
	DealReportName(objId,outName);
	if(!SaveAndOpenReport(outName,objId)) return;
}

void ReportHelper::CreatDrillReportOnlyOne()
{
	CAcModuleResourceOverride myResources;

	CString outName;
	if(!CheackDrillGE(outName)) return;
	AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ������Ҫ��������������:" ) );
	if( objId.isNull() ) return;
	if( !ArxUtilHelper::IsEqualType( _T( "DrillGE" ), objId )) return;

	DealReportName(objId,outName);
	if(!SaveAndOpenReport(outName,objId)) return;

}

void ReportHelper::CreatAddFuncReport()
{
	CAcModuleResourceOverride myResources;

	TCHAR szFileFilter[] = _T("doc�ĵ�(*.doc)|*.doc||");
	TCHAR szFileExt[] = _T("doc");
	CString defaultPath;
	GetDocPath(defaultPath);
	AcDbObjectId objId;
	SingleDataObjectHelper::GetObjectId(_T("�������"),objId);
	CString mineName;
	DataHelper::GetPropertyData(objId,_T("����"),mineName);

	CString fileName =  _T("Datas\\JL\\tpl\\tplAddF.doc");
	CString outName = mineName + _T("��˹��ɴ�����������˶�");
	CString tplName = BuildPath( GetAppPathDir(), fileName );

	tplName = BuildPath( GetAppPathDir(), fileName );
	if(!SaveAndOpenReport(tplName,outName,mineName)) return;
}

void BaseReportHelper::ListShow()
{
	CAcModuleResourceOverride myResources;
	GasSysDlg dlg;
	dlg.DoModal();
}

void BaseReportHelper::ShowGasPlanDlg()
{
	CAcModuleResourceOverride myResources;
	GasPlanDlg dlg;
	dlg.DoModal();
}

void BaseReportHelper::ShowEPCMFDDlg()
{
	CAcModuleResourceOverride myResources;
	EPCMFDDlg dlg;
	dlg.DoModal();
}

void BaseReportHelper::ShowSelfManageDlg()
{
	CAcModuleResourceOverride myResources;
	SelfManageDlg dlg;
	dlg.DoModal();
}

void BaseReportHelper::ShowInspectionDataDlg()
{
	CAcModuleResourceOverride myResources;
	InspectionDataDlg dlg;
	dlg.DoModal();
}

void BaseReportHelper::ShowGasPumpDlg()
{
	CAcModuleResourceOverride myResources;
	GasPumpDlg dlg;
	dlg.DoModal();
}

void BaseReportHelper::ShowMeasureDlg()
{
	CAcModuleResourceOverride myResources;
	MEREDlg dlg;
	dlg.setObjectName(RET_MERSURE_OBJCT_NAME);
	dlg.setDesText(_T("��ɼ�������Ƿ���㡢���������Ƿ���ϼ�����׼Ҫ��"));
	AcStringArray bookMks;
	bookMks.append(_T("MESURE_Ret"));
	bookMks.append(_T("MESURE_Method"));
	bookMks.append(_T("MESURE_Conclusion"));
	bookMks.append(_T("MESURE_YesOrNo"));
	dlg.setBookMarks(bookMks);
	dlg.DoModal();
}

void BaseReportHelper::ShowReleatedInfoDlg()
{
	CAcModuleResourceOverride myResources;
	MEREDlg dlg;
	dlg.setObjectName(RET_REALATE_OBJCT_NAME);
	dlg.setDesText(_T("���Ч����������ز��������Ƿ���ϱ�׼Ҫ��"));
	AcStringArray bookMks;
	bookMks.append(_T("ReleatInfo_Ret"));
	bookMks.append(_T("ReleatInfo_Method"));
	bookMks.append(_T("ReleatInfo_Conclusion"));
	bookMks.append(_T("ReleatInfo_YesOrNo"));
	dlg.setBookMarks(bookMks);
	dlg.DoModal();
}