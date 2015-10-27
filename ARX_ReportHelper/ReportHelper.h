#pragma once
#include "dlimexp.h"

class ARX_REPORTHELPER_DLLIMPEXP ReportHelper
{
public:
	static void CreatReportHelper();
	static void CreatBaseReport();
	static void CreatDrillReport();
	static void OpenCHMFile();
	static void OpenCHMFile2();
	static void OpenHelpCHMFile();
};

class ARX_REPORTHELPER_DLLIMPEXP BaseReportHelper
{
public:
	static void ListShow();
	static void ShowGasPlanDlg();
	static void ShowEPCMFDDlg();
	static void ShowSelfManageDlg();
	static void ShowInspectionDataDlg();
	static void ShowGasPumpDlg();
	static void ShowMeasureDlg();	//������ضԻ���
	static void ShowReleatedInfoDlg();	//�����������

};
