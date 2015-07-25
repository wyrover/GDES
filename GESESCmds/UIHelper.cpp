#include "StdAfx.h"

#include "UIHelper.h"

#include "PathBuild.h"

#include "../MineGE/DataListHelper.h"
#include "../MineGE/DataHelper.h"
#include "../MineGE/DrawHelper.h"
#include "../ArxHelper/ArxUtilHelper.h"
#include "../ARX_ReportHelper/ReportHelper.h"
#include "../MineGECmds/PropertyDataDlgHelper.h"

#include "DataList_DockBar.h"
#include "EditPumpDBDlg.h"

#define NONRET _T("**")

enum suportPatten
{
	FULLY_MECHANIZED_SUPORT	=	0,		//�۲�֧��ʽ
	FULLY_MECHANIZED_CORVE	=	1,		//�۲��ڻ�ʽ
	CONCENTIONAL_MONOMER	=	2		//�ղɵ���ʽ 

};

DataList_DockBar* pDataList_DockBar = 0;

static void GetGasLitmit(const CString& dayOutputStr ,CString& valueStr)
{
	if(dayOutputStr.IsEmpty()) return;
	int dayOutput = _ttoi(dayOutputStr);
	double value;
	if(dayOutput <= 1000 ) value = 8;
	if(dayOutput >= 1001 && dayOutput <= 2500) value = 7;
	if(dayOutput >= 2501 && dayOutput <= 4000) value = 6;
	if(dayOutput >= 4001 && dayOutput <= 6000) value = 5.5;
	if(dayOutput >= 6001 && dayOutput <= 8000) value = 5;
	if(dayOutput >= 8001 && dayOutput <= 10000) value = 4.5;
	if(dayOutput > 10000 ) value = 4;
	valueStr.Format(_T("%.1lf"),value);
}

static void GetMineDesc( CString& locaDesc ,CString& numDesc)
{
	CString dataDirName = _T( "Datas\\" );
	CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("locaDesc.txt") );

	AcIfstream inFile(fileName);
	if(!inFile) return;
	while( !inFile.eof() )
	{
		ACHAR /*charDesName[_MAX_PATH],*/ charLoca[_MAX_PATH];
		inFile /*>> charDesName*/ >> charLoca;
		CString chDesc;
		chDesc.Format(_T("%s"),charLoca);
		if(chDesc != _T("\n"))
		{
			locaDesc.Append(chDesc);
		}
	}
	inFile.close();

	fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("numDesc.txt") );

	//inFile.open(fileName);
	AcIfstream inFile2(fileName);
	if(!inFile2) return;
	while( !inFile2.eof() )
	{
		ACHAR charLoca[_MAX_PATH];
		inFile2 >> charLoca;
		CString chDesc;
		chDesc.Format(_T("%s"),charLoca);
		if(chDesc != _T("\n"))
		{
			numDesc.Append(chDesc);
		}
		//acutPrintf(_T("\n����:%s"),numDesc);
	}
	inFile2.close();

	if(locaDesc.IsEmpty()) locaDesc = NONRET;
	if(numDesc.IsEmpty()) numDesc = NONRET;
}

static void CaculFaceSV( const AcDbObjectId& objId, CString& faceAre, CString& faceSpeed)
{
	CString suportPattenIndx,thickless,minCtrl,airQuantity;
	DataHelper::GetPropertyData(objId,_T("֧����ʽ"),suportPattenIndx);
	DataHelper::GetPropertyData(objId,_T("�����濪�ɸ߶�"),thickless);
	DataHelper::GetPropertyData(objId,_T("��С�ض���"),minCtrl);
	DataHelper::GetPropertyData(objId,_T("�����������"),airQuantity);

	if(suportPattenIndx.IsEmpty())
	{
		if(faceAre.IsEmpty()) faceAre = NONRET;
		if (faceSpeed.IsEmpty()) faceSpeed = NONRET;
		return;
	}

	double fThickless = _tstof(thickless);
	double fMinCtrl = _tstof(minCtrl);
	double fQ	= _tstof(airQuantity);

	double fArea,fSpped;
	switch(_ttoi(suportPattenIndx))
	{
	case FULLY_MECHANIZED_SUPORT:
		fArea = 3.75 * (fThickless - 0.3 );
		break;
	case FULLY_MECHANIZED_CORVE:
		fArea = 3 * (fThickless - 0.3 );
		break;
	case CONCENTIONAL_MONOMER:
		fArea = fMinCtrl * fThickless;
		break;
	default:
		break;
	}

	fSpped = fQ / fArea / 60;

	faceAre.Format(_T("%lf"),fArea);
	faceSpeed.Format(_T("%lf"),fSpped);

	if(faceAre.IsEmpty()) faceAre = NONRET;
	if (faceSpeed.IsEmpty()) faceSpeed = NONRET;
}

static void CaculGasRate( const AcDbObjectId& objId, CString& gasRate )
{
	CString gasGussing,airQuantity;
	DataHelper::GetPropertyData(objId,_T("������˹��"),gasGussing);
	DataHelper::GetPropertyData(objId,_T("�����������"),airQuantity);

	if(gasGussing.IsEmpty() || airQuantity.IsEmpty()) return;
	double gG = _tstof(gasGussing);
	double aQ = _tstof(airQuantity);
	double gR = gG / (aQ);
	gR = gR * 100;
	gasRate.Format(_T("%lf"),gR);
	DataHelper::SetPropertyData(objId,_T("��������˹Ũ��"),gasRate);
}

static void DealSpPoints(CString& value)
{
	//С����ǰ���0��ȫ,���ҳ������Ҷ����0
	if(value.Find(_T('.')) == -1) return;
	CString strValue;
	strValue.Format(_T("%.2f"),_tstof(value));
	value = strValue;
	value.Replace(_T("0"),_T(" "));	//�滻0Ϊ�ո�
	value.Trim();	//�ü�
	value.Replace(_T(" "),_T("0"));
	if(value[0] == _T('.')) value.Insert(0,_T("0"));
	int lenth = value.GetLength();
	if(0 == lenth)
	{
		value = NONRET;
		return;
	}
	if(value[lenth-1] == _T('.'))
	{
		value.Replace(_T("."),_T(" "));
		value.Trim();	//�ü�
	}
}
static void DealWithSpicFields(const AcDbObjectId& objId, const CString& field, CString& value)
{
	//û���趨ֵ�Ĵ���
	if (value.IsEmpty() /*|| _T("") == value*//* || _tstof(value) <= 0*/)
	{
		//fieldValueStr = _T("");
		value = NONRET;
		//fieldValueStr.Append(fields[i].kACharPtr());
		//fieldValueStr.Append(_T("δֵ֪**"));
	}

	//�����Ĳ�ȫ
	if(_T("����") == field && -1 == value.Find(_T("��")))
	{
		value.Append(_T("��"));
	}

	//���������Ĳ�ȫ
	if(_T("��ú������") == field && -1 == value.Find(_T("������")))
	{
		value.Append(_T("������"));
	}

	if( _T("ú����ը��") == field )
	{
		switch(_ttoi(value))
		{
		case 0:
			value = _T("��");
			break;
		case 1:
			value = _T("��");
			break;
		default:
			value = NONRET;
			break;
		}
	}

	if(_T("��������˹��Դ") == field)
	{
		switch(_ttoi(value))
		{
		case 0:
			value = _T("��ú��");
			break;
		case 1:
			value = _T("�ڽ�ú��");
			break;
		default:
			value = NONRET;
			break;
		}
	}

	if ( _T("ʵ����������˹ѹ��") == field )
	{
		CString strPress;
		if( value.IsEmpty() || _tstof(value) <= 0 )
		{
			DataHelper::GetPropertyData(objId,_T("���������˹ѹ��"),strPress);
		
			if (strPress.IsEmpty() || _tstof(value) <= 0 )
			{
				value = _T("0");
				return;
			}
		}
	}
	if(_T("ú����ըָ��") != field)
	{
		DealSpPoints(value);
	}
}

static bool WriteDataToVartxt(AcDbObjectId objId,const AcStringArray& zhName, const AcStringArray& enName)
{
	CString dataDirName = _T( "Datas\\" );
	CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T( "vars.txt" ) );
	
	CString dayOutputStr;
	DataHelper::GetPropertyData(objId,_T("�ⶨ�ղ���"),dayOutputStr);
	if(_tstof(dayOutputStr) <= 0)
	{
		CString msg;
		msg.Format(_T("�ⶨ�ղ���û������"));
		AfxMessageBox(msg);
		return false;
	}
	AcOfstream outFile(fileName);
	if (!outFile) return false;
	//AcDbObjectId objId;
	//SingleDataObjectHelper::GetObjectId(_T("�������"),objId);

	int len = zhName.length();
	for ( int i = 0; i < len; i++ )
	{
		CString value;
		DataHelper::GetPropertyData(objId,zhName[i].kACharPtr(),value);
		DealWithSpicFields(objId,zhName[i].kACharPtr(),value);
		outFile << enName[i].kACharPtr() << _T("\t") << (LPWSTR)(LPCTSTR)value << _T("\n");
		//acutPrintf(_T("\n�ֶ�:%s\tֵ:%s\n"),zhName[i].kACharPtr(),enName[i].kACharPtr());
	}

	//���۷�Χ�ڵĿɽ�����˹�����ɽ�����˹����
	CString gasLimStr,actualRGStr,actualAGStr,wCCStr;
	GetGasLitmit(dayOutputStr,gasLimStr);
	if(gasLimStr.IsEmpty() ||  _T("NULL") == gasLimStr) gasLimStr = NONRET;
	DealSpPoints(gasLimStr);
	outFile  << _T("{{Analysis_Gas_Limit}}")  << _T("\t") << (LPWSTR)(LPCTSTR)gasLimStr << _T("\n");
	//���۷�Χ�ڵĿɽ�����˹���� 
	CString strAnalysis;
	DataHelper::GetPropertyData(objId,_T("�ɽ�����˹����"),strAnalysis);
	if (strAnalysis.IsEmpty())
	{
		CString msg;
		msg.Format(_T("�ɽ�����˹����û������"));
		AfxMessageBox(msg);
		return false;
	}

	if ( _tstof(gasLimStr) - _tstof(strAnalysis) < 0.00001)
	{
		CString msg;
		msg.Format(_T("���۷�Χ�ڵĿɽ�����˹������%.2fm3/t�����ڿɽ�����˹������%.2fm3/t��\n��������˹���Ҫ��!"), _tstof(strAnalysis),_tstof(gasLimStr));
		AfxMessageBox(msg);
		return false;
	}

	DataHelper::GetPropertyData(objId,_T("ʵ�������˹����"),actualRGStr);
	//��׼����ѹ���µĲд���˹����
	DataHelper::GetPropertyData(objId,_T("��׼����ѹ���µĲд���˹����"),wCCStr);

	double actualAG = _tstof(actualRGStr) - _tstof(wCCStr);
	actualAGStr.Format(_T("%lf"),actualAG);
	if( actualAG <= 0 ) actualAGStr = NONRET;
	DealSpPoints(actualAGStr);
	outFile  << _T("{{Actual_Analysis_Gas}}")  << _T("\t") << (LPWSTR)(LPCTSTR)actualAGStr << _T("\n") ;
	//acutPrintf(_T("\n�ֶ�:%s\tֵ:%s\n"),_T("{{Analysis_Gas_Limit}}"),gasLimStr);
	CString locaDesc,numDesc;
	GetMineDesc(locaDesc,numDesc);
	outFile  << _T("{{Location_Desc}}")  << _T("\t") << (LPWSTR)(LPCTSTR)locaDesc << _T("\n") ;
	outFile  << _T("{{Num_Desc}}")  << _T("\t") << (LPWSTR)(LPCTSTR)numDesc << _T("\n") ;

	CString gasRate;
	CaculGasRate(objId,gasRate);
	if (_tstof(gasRate) >= 1 || _tstof(gasRate) <= 0)
	{
		outFile.close();
		CString msg;
		msg.Format(_T("��������˹Ũ��Ϊ:%.2lf%s>1%s,��������˹���Ҫ��!"), _tstof(gasRate),_T("%"),_T("%"));
		AfxMessageBox(msg);
		return false;
	}

	DealSpPoints(gasRate);
	outFile  << _T("{{Face_Gas_Density}}")  << _T("\t") << (LPWSTR)(LPCTSTR)gasRate << _T("\n") ;

	CString areS,speedV;
	CaculFaceSV(objId,areS,speedV);
	if(_tstof(speedV) <= 0 || _tstof(speedV) > 4)
	{
		outFile.close();
		CString msg;
		msg.Format(_T("����Ϊ:%.2lfm/s,��������˹���Ҫ��!"), _tstof(speedV));
		AfxMessageBox(msg);
		return false;
	}

	DealSpPoints(areS);
	DealSpPoints(speedV);
	outFile  << _T("{{Face_Equivalent_Area}}")  << _T("\t") << (LPWSTR)(LPCTSTR)areS << _T("\n") ;
	outFile  << _T("{{Face_Wind_Speed}}")  << _T("\t") << (LPWSTR)(LPCTSTR)speedV << _T("\n") ;
	
	outFile.close();

	return true;
}

static bool GetFieldsValue()
{
	AcDbObjectId objId;
	SingleDataObjectHelper::GetObjectId(_T("�������"),objId);
	
	CString gasFromStr;
	DataHelper::GetPropertyData(objId,_T("��������˹��Դ"),gasFromStr);

	CString fvName;
	if (0 == _ttoi(gasFromStr))
	{
		fvName = _T("fvOwn.txt");
	}
	else
	{
		fvName = _T("fvNearBy.txt");
	}

	CString dataDirName = _T( "Datas\\" );
	CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),fvName );

	AcIfstream inFile(fileName);
	AcStringArray zhNames,enNames;
	//acutPrintf(_T("\n%s"),fileName);
	if(!inFile) return false;
	while( !inFile.eof() )
	{
		ACHAR zhName[_MAX_PATH], enName[_MAX_PATH];
		inFile >> zhName >> enName;
		if(inFile.fail()) break;
		CString strZhName,strEnName;
		strZhName.Format(_T("%s"),zhName);
		strEnName.Format(_T("%s"),enName);
		zhNames.append(zhName);
		enNames.append(enName);
	}
	inFile.close();
	return WriteDataToVartxt(objId,zhNames,enNames);
}

void UIHelper::CreatReport()
{
	if(!GetFieldsValue()) return;
	ReportHelper::CreatReportHelper();
	//acedCommand(RTSTR,_T("ARXWORD"),0);
}

void UIHelper::ShowDataListDockBar(const AcDbObjectId& sObjId, const AcDbObjectId& tObjId)
{
	CAcModuleResourceOverride myResources;

	//CMDIFrameWnd* pAcadFrame = acedGetAcadFrame();

	//if( pDataList_DockBar == 0 )
	//{
	//	pDataList_DockBar = new DataList_DockBar();
	//	pDataList_DockBar->Create ( pAcadFrame, _T( "·����ʾ" ) ) ;
	//	pDataList_DockBar->EnableDocking ( CBRS_ALIGN_ANY ) ;
	//	pDataList_DockBar->RestoreControlBar () ;

	//}
	////����·�����ҵ�ʼĩ�����ڷ�֧(����Ƚ϶���)
	//pDataList_DockBar->mChildDlg.sObjId = sObjId;
	//pDataList_DockBar->mChildDlg.tObjId = tObjId;
	//pAcadFrame->ShowControlBar( pDataList_DockBar, TRUE, FALSE );
	DataListDlg *dlg = new DataListDlg(acedGetAcadFrame(),FALSE);
	dlg->sObjId = sObjId;
	dlg->tObjId = tObjId;
	dlg->Run();
}

void UIHelper::DestroyDataListDockBar()
{
	if( pDataList_DockBar != 0 )
	{
		pDataList_DockBar->DestroyWindow();
		delete pDataList_DockBar;
		pDataList_DockBar = 0;
	}
}

void UIHelper::SetGESize()
{

	AcDbObjectId objId = ArxUtilHelper::SelectObject(_T("ѡ��һ����ʩ(��˹�á���ˮ����)"));
	if (!ArxUtilHelper::IsEqualType( _T( "FacilityGE" ), objId))
	{
		if(!ArxUtilHelper::IsEqualType( _T( "GasPumpGE" ), objId))
		{
			AfxMessageBox(_T("��ѡ������ʩ!"));
			return;
		}
	}

	ACHAR tt[100];
	acedGetString( 0, _T("�����С:"), tt );
	CString lenthStr;
	lenthStr.Format(_T("%s"),tt);
	DataHelper::SetPropertyData(objId,_T("��С"),lenthStr );

}

static void PrintPropertyTable( const PropertyTable& pt)
{
	//acutPrintf(_T("\n***************************"));
	acutPrintf(_T("\n**********���Ա�**********"));
	acutPrintf(_T("\nID:%d,ת��:%d,�������:%.1f,�������:%.2f,����ѹ��:%d,�����ѹ:%d"),
		pt.id,pt.speed,pt.power,pt.maxQ,pt.maxP,pt.absP);
	acutPrintf(_T("\n"));
}

static void PrintTypeTable( const TypeTable& tt)
{
	//acutPrintf(_T("\n***************************"));
	acutPrintf(_T("\n**********���ͱ�**********"));
	acutPrintf(_T("\nID:%d,����%s,��������ѹ:%d,����:%d,��:%d,��:%d,��:%d,����:%s"),
		tt.id,tt.type,tt.absP,tt.weight,tt.length,tt.weidth,tt.heigth,tt.factName);
	acutPrintf(_T("\n"));
}

//����ѡ���������������;�ѹ��ӽ�
static void SelectPumpByQP(const PropertyTableVector& propertyTableVector,double fpumpQ,double fpumpH,PropertyTableVector& retPropertyTableVector)
{
	double deltaQ = 1e100;
	double deltaP = 1e100;
	double minQ,minP;

	//��ѡ��������ӽ����Ǹ�
	for(int i = 0; i < propertyTableVector.size(); i++)
	{
		PropertyTable pt = propertyTableVector[i];
		double difQ = pt.maxQ - fpumpQ;
		if(deltaQ >= difQ && difQ >= 0) 
		{
			minQ = pt.maxQ;
			deltaQ = difQ;
		}
	}

	//Ȼ��ѡ����ѹ��ӽ����Ǹ�
	for(int i = 0; i < propertyTableVector.size(); i++)
	{
		PropertyTable pt = propertyTableVector[i];
		double difP = pt.absP - fpumpH;
		if((deltaP >= difP && difP >= 0) && ( abs(minQ - pt.maxQ) < 1e-5)) 
		{
			minP = pt.absP;
			deltaP = difP;
		}
	}

	//��¼��ѡ��������
	for(int i = 0; i < propertyTableVector.size(); i++)
	{
		PropertyTable pt = propertyTableVector[i];

		if( (abs(minP - pt.absP) < 1e-5) && ( abs(minQ - pt.maxQ) < 1e-5) ) 
		{
			retPropertyTableVector.push_back(pt);
			//PrintPropertyTable(pt);
		}
	}

}

static bool SelectPumpType( const CString& strID ,TypeTable& selTT)
{
	CString dataDirName = _T( "Datas\\" );
	CString szDbPath = BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("pump.db") );

	CString sql = _T("select * from TypeTable where catagory_id = ") + strID;
	TypeTableVector typeTableVector;
	if(!GetPumpTypeTable(sql,szDbPath,typeTableVector))
	{
		AfxMessageBox(_T("���ݿ��ʧ��!"));
		return false;
	}

	//��¼��ѡ��������
	//TypeTable selTT;
	for(int i = 0; i < typeTableVector.size(); i++)
	{
		TypeTable tt = typeTableVector[i];
		//PrintTypeTable(tt);
		selTT = tt;
	}
	return true;
}

static void SelectMinPower( const PropertyTableVector& retPropertyTableVector,PropertyTable& selPT )
{
	int vLenth = retPropertyTableVector.size();
	double minPower = 1e100;
	for(int i = 0; i < vLenth; i++)
	{
		PropertyTable pt = retPropertyTableVector[i];
		if(minPower >= pt.power)
		{
			minPower = pt.power;
		}
	}

	for(int i = 0; i < vLenth; i++)
	{
		PropertyTable pt = retPropertyTableVector[i];
		if(abs(minPower - pt.power) < 1e-5)
		{
			selPT = pt;
		}
	}
}

static void WritePumpDatas( const AcDbObjectId& objId, const PropertyTable& selPT,const TypeTable& selTT )
{
	CString strTemp;
	strTemp.Format(_T("%d"),selPT.absP);
	DataHelper::SetPropertyData(objId,_T("�����ѹ"),strTemp);
	strTemp.Format(_T("%d"),selPT.speed);
	DataHelper::SetPropertyData(objId,_T("ת��"),strTemp);
	strTemp.Format(_T("%.1lf"),selPT.power);
	DataHelper::SetPropertyData(objId,_T("�������"),strTemp);
	strTemp.Format(_T("%.2lf"),selPT.maxQ);
	DataHelper::SetPropertyData(objId,_T("�������"),strTemp);
	strTemp.Format(_T("%d"),selPT.maxP);
	DataHelper::SetPropertyData(objId,_T("����ѹ��"),strTemp);

	DataHelper::SetPropertyData(objId,_T("�ͺ�"),selTT.type);
	strTemp.Format(_T("%d"),selTT.absP);
	DataHelper::SetPropertyData(objId,_T("��������ѹ"),strTemp);
	strTemp.Format(_T("%d"),selTT.weight);
	DataHelper::SetPropertyData(objId,_T("����"),strTemp);
	strTemp.Format(_T("%d"),selTT.length);
	DataHelper::SetPropertyData(objId,_T("��"),strTemp);
	strTemp.Format(_T("%d"),selTT.weidth);
	DataHelper::SetPropertyData(objId,_T("��"),strTemp);
	strTemp.Format(_T("%d"),selTT.heigth);
	DataHelper::SetPropertyData(objId,_T("��"),strTemp);
	DataHelper::SetPropertyData(objId,_T("����"),selTT.factName);
}

static bool PumpSelecting( const AcDbObjectId& objId )
{
	CString pumpQ,pumpH;
	DataHelper::GetPropertyData(objId,_T("��˹�õĶ����"),pumpQ);
	DataHelper::GetPropertyData(objId,_T("��˹�õ�ѹ��"),pumpH);
	double fpumpH = _tstof(pumpH) * 0.01;
	double fpumpQ = _tstof(pumpQ);
	pumpH.Format(_T("%lf"),fpumpH);

	CString dataDirName = _T( "Datas\\" );
	CString szDbPath = BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("pump.db") );

	CString sql = _T("select * from PropertyTable where maxQ >= ") + pumpQ + (_T(" and absP >= ")) + pumpH;
	PropertyTableVector propertyTableVector;
	if(!GetPumpPropertyTable(sql,szDbPath,propertyTableVector))
	{
		AfxMessageBox(_T("���ݿ��ʧ��!"));
		return false;
	}

	PropertyTableVector retPropertyTableVector;
	SelectPumpByQP(propertyTableVector,fpumpQ,fpumpH,retPropertyTableVector);

	PropertyTable selPT;
	int vLenth = retPropertyTableVector.size();
	if(vLenth < 1) 
	{
		AfxMessageBox(_T("û�к��ʵ���˹���ͺ�!"));
		//�ú�ɫ��ʾ����
		DrawHelper::HighLightShowGE(objId,240);
		return false;
	}
	if(vLenth > 1)
	{
		SelectMinPower(retPropertyTableVector,selPT);
	}
	else
	{
		selPT = retPropertyTableVector[0];
	}
	CString strID;
	strID.Format(_T("%d"),selPT.id);
	TypeTable selTT;
	if( !SelectPumpType(strID,selTT) ) return false;
	//acutPrintf(_T("\n**********���ս��**********"));
	//PrintPropertyTable(selPT);
	//PrintTypeTable(selTT);
	WritePumpDatas(objId,selPT,selTT);
	return true;
}

void UIHelper::SelectPump()
{
	CAcModuleResourceOverride myResources;
	AcDbObjectId objId = ArxUtilHelper::SelectObject(_T("ѡ����Ҫѡ�͵���˹��"));
	if (!ArxUtilHelper::IsEqualType( _T( "GasPumpGE" ), objId)) return;
	if(!PumpSelecting(objId)) return;

	//�û�ɫ��ʾ����
	DrawHelper::HighLightShowGE(objId,2);

	PropertyDataDlgHelper::DisplayPropertyByFunName( objId,_T("��˹��ѡ�Ͳο�") );
}

void UIHelper::SelectPumps()
{
	CAcModuleResourceOverride myResources;
	AcDbObjectIdArray objIds;
	DrawHelper::FindMineGEs(_T("GasPumpGE"), objIds);
	for (int i = 0; i < objIds.length(); i++)
	{
		if(!PumpSelecting(objIds[i])) return;
		//�û�ɫ��ʾ����
		DrawHelper::HighLightShowGE(objIds[i],2);

		PropertyDataDlgHelper::DisplayPropertyByFunName( objIds[i],_T("��˹��ѡ�Ͳο�") );

	}
	//AfxMessageBox(_T("������˹��ѡ�ͳɹ�!"));
}

void UIHelper::DatabaseManagerDlg()
{
	CAcModuleResourceOverride myResources;
	EditPumpDBDlg dlg;
	dlg.DoModal();
	return;
}