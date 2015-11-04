#include "StdAfx.h"
#include "GetAddFuncDatas.h"

static bool GetPumpDatas(AcStringArray& bookMks, AcStringArray& datas)
{
	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(MINE_GAS_CAPACITY,datasVector,1);
	if(datasVector.empty()) return false;
	for( int i = 0; i < datasVector.size(); i++ )
	{
		CString bookMarket;
		bookMarket.Format(_T("MP_BOOKMKS_%d"),i);
		bookMks.append(bookMarket);
		datas.append(datasVector[i][0].kACharPtr());
	}

	int list_col = 8;
	ReportDataHelper::ReadDatas(PUMP_CAPACITY_LIST,datasVector,list_col);
	//CString strQ,strCon,strP,strKP,strD,strV,strKS;
	if(datasVector.empty()) return false;
	for (int i = 1; i < list_col; i++)
	{
		CString tmp;
		for(int j = 0; j < datasVector.size(); j++)
		{
			tmp.Append(datasVector[j][i].kACharPtr());
			if(datasVector.size()-1 == j) break;
			tmp.Append(_T("、"));
		}

		CString bookMarket;
		bookMarket.Format(_T("MP_LIST_BOOKMKS_%d"),i);
		bookMks.append(bookMarket);
		datas.append(tmp);
	}
	return true;
}

static bool GetRealGasQData( AcStringArray& bookMks, AcStringArray& datas )
{
	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(YEAR_REAL_GAS,datasVector,1);

	if(datasVector.empty()) return false;
	for( int i = 0; i < datasVector.size(); i++ )
	{
		CString bookMarket;
		bookMarket.Format(_T("RGQ_BOOKMKS_%d"),i);
		bookMks.append(bookMarket);
		datas.append(datasVector[i][0].kACharPtr());
	}
	return true;
}

static bool GetPreGasDatas(AcStringArray& bookMks, AcStringArray& datas)
{
	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(PRE_GAS,datasVector,1);
	if(datasVector.empty()) return false;
	for( int i = 0; i < datasVector.size(); i++ )
	{
		CString bookMarket;
		bookMarket.Format(_T("PG_BOOKMKS_%d"),i);
		bookMks.append(bookMarket);
		datas.append(datasVector[i][0].kACharPtr());
	}

	int list_col = 6;
	ReportDataHelper::ReadDatas(PRE_GAS_LIST,datasVector,list_col);
	//CString strQ,strCon,strP,strKP,strD,strV,strKS;
	if(datasVector.empty()) return false;
	for (int i = 1; i < list_col; i++)
	{
		CString tmp;
		for(int j = 0; j < datasVector.size(); j++)
		{
			tmp.Append(datasVector[j][i].kACharPtr());
			if(datasVector.size()-1 == j) break;
			tmp.Append(_T("、"));
		}

		CString bookMarket;
		bookMarket.Format(_T("PG_LIST_BOOKMKS_%d"),i);
		bookMks.append(bookMarket);
		datas.append(tmp);
	}
	 return true;
}

static bool GetRateGasData( AcStringArray& bookMks, AcStringArray& datas )
{
	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(RATE_GAS,datasVector,1);

	if(datasVector.empty()) return false;
	for( int i = 0; i < datasVector.size(); i++ )
	{
		CString bookMarket;
		bookMarket.Format(_T("RG_BOOKMKS_%d"),i);
		bookMks.append(bookMarket);
		datas.append(datasVector[i][0].kACharPtr());
	}
	return true;
}

bool GetAddFuncDatas::getAllDatas( AcStringArray& bookMks, AcStringArray& datas )
{
	if(!GetPumpDatas(bookMks,datas))
	{
		AfxMessageBox(_T("瓦斯抽采系统核定未完成!"));
		return false;
	}
	if(!GetRealGasQData(bookMks,datas))
	{
		AfxMessageBox(_T("实际瓦斯抽采量核定未完成!"));
		return false;
	}

	if(!GetPreGasDatas(bookMks,datas))
	{
		AfxMessageBox(_T("防突要求核定未完成!"));
		return false;
	}

	if(!GetRateGasData(bookMks,datas))
	{
		AfxMessageBox(_T("瓦斯抽采率核定未完成!"));
		return false;
	}

	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(RET_GAS,datasVector,1);

	if(datasVector.empty())
	{
		AfxMessageBox(_T("必须先查看结果才能输出报告！"));
		return false;
	}
	bookMks.append(_T("RET_BOOKMKS_0"));
	datas.append(datasVector[0][0].kACharPtr());
	bookMks.append(_T("RET_BOOKMKS_1"));
	datas.append(datasVector[0][0].kACharPtr());
	return true;
}
