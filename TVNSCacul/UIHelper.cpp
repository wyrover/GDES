#include "StdAfx.h"
#include "UIHelper.h"

#include "../DefGE/Fan.h"
#include "../DefGE/WindLibrary.h"
#include "../DefGE/Chimney.h"
#include "../DefGE/QTagGE.h"

#include "PropertyDockBarChildDlg.h"
#include "BaseparamDlg.h"
#include "CaculParamDlg.h"
//#include "LeakageStandByDlg.h"
#include "CheakFanDlg.h"
//#include "WindageChooseDlg.h"

#include "NetWorkData.h"
#include "RqAirflowCacul.h"
#include "WindageCacul.h"
#include "LeakageCacul.h"
#include "FanWPCacul.h"
#include "VentWaysConfig.h"

#include "DataInput_DockBar.h"
#include "BaseParam_DockBar.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"
#include "FanChooseDlg.h"
//#include "FindFanDlg.h"

DataInput_DockBar* pDataInput_DockBar = 0;
BaseParam_DockBar* pBaseParamDockBar = 0;

extern void InitDefVentData(AcDbObjectId objId);

static void SetVentMethod()
{
	AcDbObjectIdArray fanObjIds;
	DrawHelper::FindMineGEs(_T("LocalFan"),fanObjIds);
	int len = fanObjIds.length();
	for(int i = 0; i < len; i++)
	{
		AcDbObjectId ttunelId = GetLinkedTTunelId(fanObjIds[i]);
		CString ventMethod,ventWay;
		if(!DataHelper::GetPropertyData(ttunelId,_T("ͨ�緽ʽ"),ventMethod)) return;
		if(!DataHelper::GetPropertyData(ttunelId,_T("ͨ�緽��"),ventWay)) return;
		if(!DataHelper::SetPropertyData(fanObjIds[i],_T("ͨ�緽ʽ"),ventMethod)) return;
		if(!DataHelper::SetPropertyData(fanObjIds[i],_T("ͨ�緽��"),ventWay)) return;
	}
}

void UIHelper::ShowBaseParamDlg()
{
	// �л���Դ
	CAcModuleResourceOverride myResources;

	//AcStringArray fileds;
	//FieldHelper::GetAllFields( _("��������"), fileds );
	//int len = fileds.length();

	//AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "ѡ����Ҫ�������ݵĹ�����:" ) );
	//if (!ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId))
	//{
	//	//AfxMessageBox(_T("��ѡ�Ĳ��ǹ����棬�޷���������!"),MB_OK | MB_ICONSTOP);
	//	return;
	//}

	//BaseParamDlg *dlg = new BaseParamDlg(NULL,objId);
	//dlg->DoModal();
	//SetVentMethod();

	//AcDbObjectId baseParamObjId;
	//if(!SingleDataObjectHelper::GetObjectId(_T("����Ϣ"),baseParamObjId)) return;
	//CMDIFrameWnd* pAcadFrame = acedGetAcadFrame();

	//if( pBaseParamDockBar == 0 )
	//{
	//	pBaseParamDockBar = new BaseParam_DockBar();
	//	pBaseParamDockBar->Create ( pAcadFrame, _T( "�����������" ) ) ;
	//	pBaseParamDockBar->EnableDocking ( CBRS_ALIGN_ANY ) ;
	//	pBaseParamDockBar->RestoreControlBar () ;
	//}
	////pWindStationDockBar->RestoreControlBar();
	//pBaseParamDockBar->update();
	//pAcadFrame->ShowControlBar( pBaseParamDockBar, TRUE, FALSE );
	BaseParamDockDlg *dlg = new BaseParamDockDlg(acedGetAcadFrame(),TRUE);
	dlg->Run();
}

void UIHelper::DestroyBaseParamDockBar()
{
	if( pBaseParamDockBar != 0 )
	{
		pBaseParamDockBar->DestroyWindow();
		delete pBaseParamDockBar;
		pBaseParamDockBar = 0;
	}
}

void UIHelper::ShowDataInputDockBar()
{
	SetVentMethod();
	static int n = 0;

	CAcModuleResourceOverride myResources;

	CMDIFrameWnd* pAcadFrame = acedGetAcadFrame();

	if( pDataInput_DockBar == 0 )
	{
		pDataInput_DockBar = new DataInput_DockBar();
		pDataInput_DockBar->Create ( pAcadFrame, _T( "�������ݵ���ʾ" ) ) ;
		pDataInput_DockBar->EnableDocking ( CBRS_ALIGN_ANY ) ;
		pDataInput_DockBar->RestoreControlBar () ;
	}
	//pWindStationDockBar->RestoreControlBar();
	pAcadFrame->ShowControlBar( pDataInput_DockBar, TRUE, FALSE );
}

void UIHelper::DestroyDataInputDockBar()
{
	if( pDataInput_DockBar != 0 )
	{
		pDataInput_DockBar->DestroyWindow();
		delete pDataInput_DockBar;
		pDataInput_DockBar = 0;
	}
}

void UIHelper::CaculateRequirAirFlow()
{
	AcDbObjectIdArray objIds;
	DrawHelper::FindMineGEs(_T("TTunnel"), objIds);
	int TTunelNum = objIds.length();
	if (0 == TTunelNum)
	{
		AfxMessageBox(_T("ͼ����û�й�����"),MB_OK | MB_ICONSTOP );
		return;
	}
	for (int i = 0 ; i < TTunelNum; i++)
	{
		acutPrintf(_T("\n���������%d���������..."),i+1);
		VentCaculate *caculate = new VentCaculate(objIds[i]);
		caculate->ensureAir();
		if(caculate->m_rqAirflow <= 0) return;

		CString s;
		s.Format(_T("%lf"),caculate->m_rqAirflow);
		DataHelper::SetPropertyData( objIds[i], _T( "�����" ), s );


		DataHelper::SetPropertyData( objIds[i], _T( "������ʽ" ), caculate->m_dynamiteWay );
	}
	SetVentMethod();
}

void UIHelper::InputBaseParam()
{
	CAcModuleResourceOverride myResources;

	AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "ѡ����Ҫ����Ĺ�����:" ) );
	if (!ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId))
	{
		//AfxMessageBox(_T("��ѡ�Ĳ��ǹ����棬�޷�����!"));
		return;
	}

	InitDefVentData(objId);
	CaculParamDlg *dlg = new CaculParamDlg(NULL,objId);
	dlg->DoModal();

}

void UIHelper::CaculateChimwindage()
{
	//AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "ѡ����Ҫ����ķ�Ͳ:" ) );
	//if (!ArxUtilHelper::IsEqualType( _T( "Chimney" ), objId))
	//{
	//	//AfxMessageBox(_T("��ѡ�Ĳ��ǹ����棬�޷�����!"));
	//	return;
	//}

	//CAcModuleResourceOverride myResources;
	//AcDbObjectIdArray objIds;
	//DrawHelper::FindMineGEs(_T("Chimney"), objIds);
	//int ChimneyNum = objIds.length();
	//if (0 == ChimneyNum)
	//{
	//	AfxMessageBox(_T("ͼ����û�з�Ͳ"),MB_OK | MB_ICONSTOP );
	//	return;
	//}

	////WindageChooseDlg *dlg = new WindageChooseDlg;
	////dlg->DoModal();
	//int caculWay = dlg->m_windageWay;

	////CString msg;
	////msg.Format(_T("%d"),caculWay);
	////AfxMessageBox(msg);
	////return;
	//for(int i = 0; i < ChimneyNum; i++)
	//{
	//	acutPrintf(_T("\n��%d�η�Ͳ�������..."),i+1);
	//	WindageCacul *caculate = new WindageCacul(objIds[i]);
	//	double windageret;
	//	if(1 == caculWay)
	//	{
	//		windageret = caculate->windageCaculByHM();
	//	}
	//	else
	//	{
	//		windageret = caculate->windageCaculRet();
	//	}
	//	if(windageret <= 0) return;
	//	CString s;
	//	s.Format(_T("%lf"),windageret);
	//	DataHelper::SetPropertyData( objIds[i], _T( "��Ͳ����" ), s );
	//	//AfxMessageBox(_T("��Ͳ�������ɹ�"),MB_OK | MB_ICONINFORMATION );
	//}

}

void UIHelper::InputLeakageParam()
{
	CAcModuleResourceOverride myResources;

	AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "ѡ����Ҫ����ķ�Ͳ:" ) );
	if (!ArxUtilHelper::IsEqualType( _T( "Chimney" ), objId))
	{
		return;
	}

	CString chimType;
	DataHelper::GetPropertyData(objId,_T("����"),chimType);

	//LeakageStandByDlg *dlg = new LeakageStandByDlg(NULL,objId,chimType);
	//dlg->DoModal();

}

void UIHelper::LeakageCacul()
{
	CAcModuleResourceOverride myResources;

	AcDbObjectIdArray objIds;
	DrawHelper::FindMineGEs(_T("Chimney"), objIds);
	int ChimneyNum = objIds.length();
	if (0 == ChimneyNum)
	{
		AfxMessageBox(_T("ͼ����û�з�Ͳ"),MB_OK | MB_ICONSTOP );
		return;
	}

	for(int i = 0; i < ChimneyNum; i++)
	{
		LeakageCaculate * caculate = new LeakageCaculate(objIds[i]);
		double leakage = caculate->LeakageCaculRet();
		if (leakage <= 0) return;
		CString s;
		s.Format(_T("%lf"),leakage);
		if(!DataHelper::SetPropertyData( objIds[i], _T( "©�籸��ϵ��" ),  s)) return;
	}
}

static void CaculateQH(const AcDbObjectId& ttunelId)
{
	//FanWPCaculHelper::ReadData();
	AcDbObjectIdArray fanIds = GetLinkedFans(ttunelId);
	int fanNum = fanIds.length();
	//acutPrintf(_T("\nfanNum:%d"),fanNum);
	if (0 == fanNum)
	{
		AfxMessageBox(_T("�������û�з��"),MB_OK | MB_ICONSTOP );
		return;
	}

	CString method,reqQ,way;
	if(!DataHelper::GetPropertyData(ttunelId,_T("ͨ�緽ʽ"),method)) return;
	if(!DataHelper::GetPropertyData(ttunelId,_T("�����"),reqQ)) return;
	if(!DataHelper::GetPropertyData(ttunelId,_T("ͨ�緽��"),way)) return;


	if(method.IsEmpty())
	{
		AfxMessageBox(_T("û��ѡ��ͨ�緽ʽ"),MB_OK | MB_ICONSTOP);
		return;
	}

	FanChooseDlg *fanChDlg = new FanChooseDlg;
	fanChDlg->DoModal();
	chooseType chFanType = fanChDlg->m_chFanType;
	FanWPCaculHelper::FanWPCacul(fanIds,way,method,reqQ,chFanType);

}

//#include "../DrawVentCmd/DrawCmd.h"
void UIHelper::EnsureFan()
{
	CAcModuleResourceOverride myResources;

	AcDbObjectIdArray objIds;
	DrawHelper::FindMineGEs(_T("TTunnel"), objIds);
	int TTunelNum = objIds.length();
	if (0 == TTunelNum)
	{
		AfxMessageBox(_T("ͼ����û�й�����"),MB_OK | MB_ICONSTOP );
		return;
	}

	for (int i = 0 ; i < TTunelNum; i++)
	{
		CaculateQH(objIds[i]);
	}

	AcDbObjectIdArray fanTags;
	DrawHelper::FindMineGEs(_T("FanTagGE"), fanTags);
	if (!fanTags.isEmpty())
	{
		ArxEntityHelper::EraseObjects( fanTags, true );
		//DrawCmd::DrawFanTag();
	}


}

static void WriteChimneyQ(AcDbObjectId objId,double p)
{
	CString workQStr;
	DataHelper::GetPropertyData(objId,_T("��������(m3/s)"),workQStr);

	CString way;
	DataHelper::GetPropertyData(objId,_T("������ʽ"),way);
	AcDbObjectIdArray chims;
	DrawHelper::GetTagGEById2( objId, _T( "Chimney" ), chims );
	if (_T("ѹ��ʽ") == way)
	{
		for (int i = 0; i < chims.length(); i++) {
			DataHelper::SetPropertyData(chims[i],_T("��ڷ���(m3/s)"),workQStr);
			CString lekageStr;
			DataHelper::GetPropertyData(chims[i],_T("©�籸��ϵ��"),lekageStr);
			CString outQString;
			outQString.Format(_T("%lf"),(_tstof(workQStr) / _tstof(lekageStr)));
			DataHelper::SetPropertyData(chims[i],_T("���ڷ���(m3/s)"),outQString);
			workQStr = outQString;
		}
		acutPrintf(_T("\n��Ͳ���ڷ���:%.2lfm^3/s"),_tstof(workQStr));
	}

	if (_T("���ʽ") == way)
	{
		for (int i = 0; i < chims.length(); i++) {
			DataHelper::SetPropertyData(chims[i],_T("���ڷ���(m3/s)"),workQStr);
			CString lekageStr;
			DataHelper::GetPropertyData(chims[i],_T("©�籸��ϵ��"),lekageStr);
			CString inQString;
			inQString.Format(_T("%lf"),(_tstof(workQStr) /  _tstof(lekageStr)));
			DataHelper::SetPropertyData(chims[i],_T("��ڷ���(m3/s)"),inQString);
			workQStr = inQString;
		}
		acutPrintf(_T("\n��Ͳ��ڷ���:%.2lfm^3/s"),_tstof(workQStr));
	}

}

static void WriteWorkParam(AcDbObjectId objId)
{
	CString maxHStr,minHStr,minQStr,maxQStr;
	CString randQ,randH;
	DataHelper::GetPropertyData(objId,_T("������Χ"),randQ);
	DataHelper::GetPropertyData(objId,_T("��ѹ��Χ"),randH);

	//�ַ����ķָ�
	TCHAR *token;
	TCHAR *sep = _T("~");
	token = _tcstok((LPTSTR)(LPCTSTR)randQ,sep);
	minQStr = token;
	sep = _T("(");
	token = _tcstok(NULL,sep);
	maxQStr = token;

	sep = _T("~");
	token = _tcstok((LPTSTR)(LPCTSTR)randH,sep);
	minHStr = token;
	sep = _T("(");
	token = _tcstok(NULL,sep);
	maxHStr = token;

	//��ȡ��Ͳ�����ԣ���������Ͳ���ܷ����©��ϵ��
	AcDbObjectIdArray chimsObjIds = FanWPCaculHelper::getLinkedChim(objId);
	if( 0 == chimsObjIds.length()) return;

	int len = chimsObjIds.length();
	double p  = 1,r = 0;
	for(int i = len-1; i >= 0; i--)
	{
		CString leakStr,windageStr;
		if(!DataHelper::GetPropertyData(chimsObjIds[i],_T("©�籸��ϵ��"),leakStr)) return;
		if(!DataHelper::GetPropertyData(chimsObjIds[i],_T("��Ͳ����"),windageStr)) return;

		double p1 = _tstof(leakStr);
		double r1 = _tstof(windageStr);
		p = p * p1;
		r = r + r1;
	}

	double maxH = _tstof(maxHStr);
	double minH = _tstof(minHStr);
	double maxQ = _tstof(maxQStr)/60;
	double minQ = _tstof(minQStr)/60;
	double m = (maxH - minH) / (minQ - maxQ);
	double n = m * m + 4 * r * (minH*minQ - maxH*maxQ)/ (p *(minQ - maxQ));
	double Qf = (m + sqrtf(n)) * p * 0.5 / r;

	double workH = r * Qf * Qf / p;
	double spaceH = maxH - workH;

	if( Qf > 0 ) ;
	else return;
	CString workQStr,workHStr,spaceHStr;
	workQStr.Format(_T("%lf"),Qf);
	workHStr.Format(_T("%lf"),workH);
	spaceHStr.Format(_T("%lf"),spaceH);
	acutPrintf(_T("\n�����������:%.2lfm^3/s"),Qf);
	DataHelper::SetPropertyData(objId,_T("��������(m3/s)"),workQStr);
	DataHelper::SetPropertyData(objId,_T("������ѹ(Pa)"),workHStr);
	if( !(spaceH < 0) ){
		DataHelper::SetPropertyData(objId,_T("��ѹ���޿ռ�(Pa)"),spaceHStr);
	}

	WriteChimneyQ(objId,p);
}

static void QDisplay(AcDbObjectId chimId)
{
	AcTransaction *pTrans = actrTransactionManager->startTransaction();
	if ( 0 == pTrans ) return;
	AcDbObject *pObj;
	if (Acad::eOk != pTrans->getObject(pObj,chimId,AcDb::kForRead)) return;
	Chimney *pChim = Chimney::cast(pObj);
	if ( 0 == pChim)
	{
		actrTransactionManager->abortTransaction();
		return;
	}
	AcGePoint3dArray ctlPts = pChim->getControlPoint();

	AcDbObjectId fanId;
	if(!DrawHelper::GetHostGE(chimId,fanId)) return;

	if (Acad::eOk != pTrans->getObject(pObj,fanId,AcDb::kForRead)) return;
	LocalFan *pFan = LocalFan::cast(pObj);
	if ( 0 == pFan)
	{
		actrTransactionManager->abortTransaction();
		return;
	}
	AcGePoint3d insertPt = 	pFan->getInsertPt();
	actrTransactionManager->endTransaction();

	AcDbObjectIdArray QTags;
	DrawHelper::GetTagGEById2( chimId, _T( "QTagGE" ), QTags );
	if (!QTags.isEmpty())
	{
		ArxEntityHelper::EraseObjects( QTags, true );
	}

	CString inQStr,outQStr;
	DataHelper::GetPropertyData(chimId,_T("��ڷ���(m3/s)"),inQStr);
	DataHelper::GetPropertyData(chimId,_T("���ڷ���(m3/s)"),outQStr);

	CString way;
	DataHelper::GetPropertyData(fanId,_T("������ʽ"),way);
	if(way.IsEmpty() || inQStr.IsEmpty() || outQStr.IsEmpty()) return;

	QTagGE *pInQTag,*pOutQTag;
	if( _T("ѹ��ʽ") == way )
	{
		pInQTag = new QTagGE(ctlPts[0],_tstof(inQStr));
		if( pInQTag == 0 ) return;
		pOutQTag = new QTagGE(ctlPts[ctlPts.length()-1],_tstof(outQStr));
		if( pOutQTag == 0 ) return;
	}

	if( _T("���ʽ") == way )
	{
		pOutQTag = new QTagGE(ctlPts[0],_tstof(outQStr));
		if( pOutQTag == 0 ) return;
		pInQTag = new QTagGE(ctlPts[ctlPts.length()-1],_tstof(inQStr));
		if( pInQTag == 0 ) return;
	}
	pOutQTag->setRelatedGE( chimId ); // ������Ͳ
	pInQTag->setRelatedGE( chimId ); // ������Ͳ

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pOutQTag ) ) delete pOutQTag;
	if( !ArxUtilHelper::PostToModelSpace( pInQTag ) ) delete pInQTag;
}
void UIHelper::CheakFan()
{
	CAcModuleResourceOverride myResources;
	AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "ѡ����Ҫ��֤�ķ��:" ) );
	if (!ArxUtilHelper::IsEqualType( _T( "LocalFan" ), objId)) return;

	//�����������������С�����Լ�����ѹ����С��ѹ��
	//��д������������ֶζ�Ӧ��ֵ�У����򲻻�д��
	CheakFanDlg *dlg = new CheakFanDlg(NULL,objId);
	dlg->DoModal();
	//�����Ƿ�������ѹ��������͹�����ѹ���Լ����޿ռ����һ��
	WriteWorkParam(objId);
	AcDbObjectIdArray chimIds;
	DrawHelper::GetTagGEById2(objId,_T("Chimney"),chimIds);
	for (int i = 0 ; i < chimIds.length(); i++)
	{
		QDisplay(chimIds[i]);
	}
}

void UIHelper::InsertFan()
{
	//InserFanDlg *insertFanDlg = new InserFanDlg;
	//insertFanDlg->DoModal();
}

void UIHelper::FindFanByDB()
{
	//FindFanDlg findFanDlg;
	//findFanDlg.DoModal();
	////Sqlite sqlite;
	//AcStringArray retArray = sqlite.findFan(_T("MinQ"),_T("PullOut"),_T(" <=2300 "));
	//CString msg;

	//for (int i = 0; i < retArray.length(); i++)
	//{
	//	if (i%6 == 0)
	//	{
	//		msg.Append(_T("\n"));
	//	}
	//	msg.Append(retArray[i]);
	//}
	//AfxMessageBox(msg);
}

//AcDbObjectIdArray UIHelper::SelectFan()
//{
//	AcDbObjectIdArray objIds;
//	struct resbuf eb;
//	ACHAR sbuf[10];
//	eb.restype=0;//ʵ��������  
//	wcscpy(sbuf,_T("�ֲ��ȷ��")); 
//	eb.resval.rstring=sbuf;
//	eb.rbnext=NULL;  
//
//	ads_name ss;
//	int ret = acedSSGet( NULL, NULL, NULL, &eb, ss );
//	if( RTNORM != ret ) return NULL;
//
//	long n = 0;
//	acedSSLength ( ss, &n );
//	for( long i = 0; i < n; i++ )
//	{
//		ads_name ename;
//		if( RTNORM != acedSSName ( ss, i, ename ) ) continue;
//
//		AcDbObjectId objId;
//		if( Acad::eOk != acdbGetObjectId( objId, ename ) ) continue;
//
//		objIds.append( objId );
//	}
//	acedSSFree( ss );
//	return objIds;
//
//}

//AcDbObjectIdArray UIHelper::SelectLib()
//{
//	AcDbObjectIdArray objIds;
//	struct resbuf eb;
//	ACHAR sbuf[10];
//	eb.restype=0;//ʵ��������  
//	wcscpy(sbuf,_T("���")); 
//	eb.resval.rstring=sbuf;
//	eb.rbnext=NULL;  
//
//	ads_name ss;
//	int ret = acedSSGet( NULL, NULL, NULL, &eb, ss );
//	if( RTNORM != ret ) return NULL;
//
//	long n = 0;
//	acedSSLength ( ss, &n );
//	for( long i = 0; i < n; i++ )
//	{
//		ads_name ename;
//		if( RTNORM != acedSSName ( ss, i, ename ) ) continue;
//
//		AcDbObjectId objId;
//		if( Acad::eOk != acdbGetObjectId( objId, ename ) ) continue;
//
//		objIds.append( objId );
//	}
//	acedSSFree( ss );
//	return objIds;
//
//}


//void UIHelper::SetTunnelLenToDia( AcDbObjectId objId, double lenth )
//{
//	DataHelper::SetPropertyData( objId, _T( "����" ), lenth );
//}

