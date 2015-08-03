#include "stdafx.h"
#include "DrawCmd.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

// ͨ��ͼԪ
#include "../DefGE/Tunnel.h"
#include "../DefGE/Fan.h"
#include "../DefGE/WindDirection.h"
#include "../DefGE/ArcTunnel.h"
#include "../DefGE/TTunnel.h"
#include "../DefGE/Joint.h"
#include "../DefGE/WindLibrary.h"
#include "../DefGE/FanTagGE.h"

#include "../DefGE/Chimney.h"
#include "../DefGE/QTagGE.h"
#include "../DefGE/EffectRanTag.h"
#include "../DefGE/GasTube.h"
#include "../DefGE/GasPumpGE.h"
#include "../DefGE/ValveGE.h"
#include "../DefGE/TailraceGE.h"
#include "../DefGE/FlowmeterGE.h"
#include "../DefGE/BackFireGE.h"
#include "../DefGE/DetermineHoleGE.h"
#include "../DefGE/GasFlowTagGE.h"
#include "../DefGE/DrillGE.h"
#include "../DefGE/TempeSensorGE.h"
#include "../DefGE/FlowSensorGE.h"
#include "../DefGE/DifferPressSensorGE.h"
#include "../DefGE/GasSensorGE.h"
#include "../DefGE/COSensorGE.h"
//#include "../vno/vno.h"
//#include "../GraphTool/DfsNetWork.h"

#include "PolyLineJig.h"

/* ȫ�ֺ���(ʵ����PromptTool.cpp) */
extern bool PromptSEPt( const CString& name, AcGePoint3d& startPt, AcGePoint3d& endPt, double& angle );
extern bool GetClosePtAndAngle( const AcDbObjectId& objId, AcGePoint3d& pt, double& angle );
extern bool PromptInsertPt( const AcDbObjectId& objId, AcGePoint3d& pt );
extern bool PromptArcPt( const CString& name, AcGePoint3d& startPt, AcGePoint3d& endPt, AcGePoint3d& thirdPt );
extern bool GetSEPt( const AcDbObjectId& objId, AcGePoint3d& spt, AcGePoint3d& ept );

// ���Ʒ�������ʱ���������С����
#define MIN_LENGTH 150

static bool CheackTubeLenth(AcDbObjectId& objId)
{
	objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ����˹��·:" ) );
	if( objId.isNull() ) return false;
	if( !ArxUtilHelper::IsEqualType( _T( "GasTube" ), objId ) ) return false;

	AcTransaction *pTrans = actrTransactionManager->startTransaction();
	if ( 0 == pTrans ) return false;
	AcDbObject *pObj;
	if (Acad::eOk != pTrans->getObject(pObj,objId,AcDb::kForRead)) return false;
	GasTube *pGasTube = GasTube::cast(pObj);
	if ( 0 == pGasTube)
	{
		actrTransactionManager->abortTransaction();
		return false;
	}
	AcGePoint3d spt,ept;
	pGasTube->getSEPoint(spt,ept);
	//double angle = pGasTube->getAngle();
	actrTransactionManager->endTransaction();

	AcGeVector3d v = ept - spt;
	double tubeLenth = v.length();
	return true;
}

static Acad::ErrorStatus GetLinetypeId(const CString& linetype, AcDbObjectId &linetypeId)
{
	AcDbLinetypeTable *pLtpTbl;
	acdbHostApplicationServices()->workingDatabase()->getLinetypeTable(pLtpTbl, AcDb::kForRead);
	if (!pLtpTbl->has(linetype))
	{
		pLtpTbl->close();
		return Acad::eBadLinetypeName;
	}
	pLtpTbl->getAt(linetype, linetypeId);
	pLtpTbl->close();
	return Acad::eOk;
}

static void AddLineType(CString lineTypeName)
{
	// �������ͣ����ַ�����
	Acad::ErrorStatus es;
	//es = acdbHostApplicationServices()->workingDatabase()->loadLineTypeFile(_T("CENTER"), _T("acadiso.lin"));
	es = acdbLoadLineTypeFile(lineTypeName, _T("acadiso.lin"),acdbHostApplicationServices()->workingDatabase());
	// �����µ�AcDbMlineStyle����
	AcDbMlineStyle *pMlStyle = new AcDbMlineStyle;
	pMlStyle->initMlineStyle();
	pMlStyle->setName(_T("NewStyle"));
	int index; // ������ʽ�е�Ԫ������
	//AcCmColor color; // ��ɫ
	AcDbObjectId linetypeId; // ���͵�ID
	// ��ӵ�һ��Ԫ�أ���ɫ�������ߣ�
	//color.setColorIndex(1); // ��ɫ
	GetLinetypeId(lineTypeName, linetypeId);
	//pMlStyle->addElement(index, 0, color, linetypeId);
	//// ��ӵڶ���Ԫ�أ���ɫ�����ߣ�
	//color.setColorIndex(5); // ��ɫ
	//GetLinetypeId("HIDDEN", linetypeId);
	//pMlStyle->addElement(index, 0.5, color, linetypeId);
	//// ��ӵ�����Ԫ�أ���ɫ�����ߣ�
	//pMlStyle->addElement(index, -0.5, color, linetypeId);
	//// ��������ʽ��ӵ�������ʽ�ֵ���
	AcDbDictionary *pDict;
	acdbHostApplicationServices()->workingDatabase()->getMLStyleDictionary(pDict, AcDb::kForWrite);
	AcDbObjectId mlStyleId;
	es = pDict->setAt(_T("NewStyle"), pMlStyle, mlStyleId);
	pDict->close();
	pMlStyle->close();
}

static void CheakLayerExit(CString layerName,int colorIndx, AcDb::LineWeight lineWeigt)
{
	//int colorIndx = 7;		//Ĭ�ϵ���ɫΪ��ɫ
	//AcDb::LineWeight lineWeigt = AcDb::kLnWt000;

	//lineWeigt = AcDb::kLnWt030;
	AcDbLayerTable* pLayerTbl = NULL;
	// ��ȡ��ǰ�����ݿ�
	AcDbDatabase*pDB=acdbHostApplicationServices()->workingDatabase();
	// ��ΪҪ�����µ�ͼ�㣬������Ҫ��д�ķ�ʽ��ȡͼ���
	pDB->getSymbolTable(pLayerTbl,AcDb::kForWrite);
	// ���ͼ���Ƿ����
	if (!pLayerTbl->has(layerName)) {
		// ��ʼ��һ���µĶ��󣬲���������������
		AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord;
		pLayerTblRcd->setName(layerName);
		pLayerTblRcd->setIsFrozen(0); // ͼ������ΪTHAWED���ⶳ�ģ�
		pLayerTblRcd->setIsOff(0); // ͼ������ΪON(���ŵ�)
		pLayerTblRcd->setIsLocked(0); // ͼ�� un-locked(������)
		AcCmColor color;
		color.setColorIndex(colorIndx); // ͼ�����ɫ����
		pLayerTblRcd->setColor(color);
		pLayerTblRcd->setLineWeight(lineWeigt);
		// ����һ���µ�ͼ�㵽����(��)��
		pLayerTbl->add(pLayerTblRcd);
		// ���½���ͼ��ر�(��Ҫɾ����)
		pLayerTblRcd->close();
		// �ر�����(��)
		pLayerTbl->close();
	}
	else {
		// ������ͼ���Ѿ����ڣ�������Ҫ�رձ��������
		AcDbLayerTableRecord *pLayerTblRcd;
		pLayerTbl->getAt(layerName, pLayerTblRcd, AcDb::kForWrite);
		AcCmColor color;
		color.setColorIndex(colorIndx); // ͼ�����ɫ����
		pLayerTblRcd->setColor(color);
		pLayerTblRcd->setLineWeight(lineWeigt);
		pLayerTblRcd->close();
		pLayerTbl->close();
		//acutPrintf(_T("\nMYLAYER already exists"));
	}

}

static void CreateDirection( const AcDbObjectId& host, const AcGePoint3d& pt, double angle, int colorIndx )
{
    WindDirection* pDir = new WindDirection( pt, angle );
    pDir->setRelatedGE( host ); // ����ͼԪ

	pDir->setColorIndex(colorIndx);
    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pDir ) ) delete pDir;
}

void DrawCmd::DrawArcTunnel( void )
{
	AcGePoint3d startPt, endPt, thirdPt;
	CString GEName = _T( "�������" );
	if( !PromptArcPt(GEName , startPt, endPt, thirdPt ) ) return;

	ArcTunnel* pArcTunnel = new ArcTunnel( startPt, endPt, thirdPt );  // �������
	
	CheakLayerExit(GEName,7,AcDb::kLnWt030);
	pArcTunnel->setLayer(GEName);
	if( !ArxUtilHelper::PostToModelSpace( pArcTunnel ) ) delete pArcTunnel;
}

void DrawCmd::DrawTunnel( void )
{
    AcGePoint3d startPt, endPt;
    double angle;
    if( !PromptSEPt( _T( "���" ), startPt, endPt, angle ) ) return;

    // 3������ͼԪ
    Tunnel* pTunnel = new Tunnel( startPt, endPt );  // ���

    // 4����ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pTunnel ) ) delete pTunnel;
	//bool ret = ArxUtilHelper::PostToModelSpace( pTunnel );
	//acutPrintf(_T("\n�ύ״̬��%s"),ret ? _T("�ɹ�"):_T("ʧ��"));
}


void DrawCmd::DrawLocalFan()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ�����:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;
	//if( !ArxUtilHelper::IsEqualType( _T( "Chimney" ), objId ) ) return;

    AcDbObjectIdArray objIds;
    DrawHelper::GetTagGEById2( objId, _T( "LocalFan" ), objIds );
    //if( !objIds.isEmpty() )
    //{
    //    AfxMessageBox( _T( "�þ���������������˾ֲ�ͨ���!" ) );
    //    return;
    //}

    AcGePoint3d pt,insertPt;

	double angle;
	//if( !GetClosePtAndAngle( objId, pt, angle ) ) return;
	
	if( !ArxUtilHelper::PromptPt( _T( "\n��ָ�����ȵĲ��������:" ), pt ) ) return;

	insertPt = pt;
	if( !GetClosePtAndAngle( objId, pt, angle ) ) return;
    Fan* pFan = new LocalFan( insertPt, angle ); 
    if( pFan == 0 ) return;

    pFan->setRelatedGE( objId ); // �������

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pFan ) ) delete pFan;
}

static void FanTagGEDrawed(AcDbObjectId fanId)
{
	AcTransaction *pTrans = actrTransactionManager->startTransaction();
	if ( 0 == pTrans ) return;
	AcDbObject *pObj;
	if (Acad::eOk != pTrans->getObject(pObj,fanId,AcDb::kForRead)) return;
	LocalFan *pFan = LocalFan::cast(pObj);
	if ( 0 == pFan)
	{
		actrTransactionManager->abortTransaction();
		return;
	}
	AcGePoint3d insertPt = 	pFan->getInsertPt();
	actrTransactionManager->endTransaction();

	AcDbObjectIdArray fanTags;
	DrawHelper::GetTagGEById2( fanId, _T( "FanTagGE" ), fanTags );
	if (!fanTags.isEmpty())
	{
		ArxEntityHelper::EraseObjects( fanTags, true );
	}
	
	CString name,qStr,hStr,way;
	DataHelper::GetPropertyData(fanId,_T("����ͺ�"),name);
	DataHelper::GetPropertyData(fanId,_T("��������(m3/s)"),qStr);
	DataHelper::GetPropertyData(fanId,_T("������ѹ(Pa)"),hStr);
	DataHelper::GetPropertyData(fanId,_T("������ʽ"),way);

	FanTagGE *pFanTag = new FanTagGE(insertPt,name,way,_tstof(qStr),_tstof(hStr));
	if( pFanTag == 0 ) return;

	//acutPrintf(_T("\ninsertPt:(%lf,%lf),angle:%lf"),insertPt.x,insertPt.y,angle);

	pFanTag->setRelatedGE( fanId ); // ��������

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pFanTag ) ) delete pFanTag;

}

void DrawCmd::DrawFanTag()
{
	AcDbObjectIdArray fanIds;
	DrawHelper::FindMineGEs(_T("LocalFan"),fanIds);
	int len = fanIds.length();
	for (int i = 0; i < len; i++)
	{
		FanTagGEDrawed(fanIds[i]);
	}
}

void DrawCmd::DrawDirection( void )
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ����Ҫ���������ͼԪ:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;
	if( ArxUtilHelper::IsEqualType( _T( "DrillGE" ), objId ) ) return;

	AcDbObject* pObj;
	acdbOpenObject( pObj, objId, AcDb::kForRead );
    
	AcGePoint3d pt;
    if( !PromptInsertPt( objId, pt ) ) return;

	//ArcTunnel *pArcTunnel = ArcTunnel::cast(pObj);
	LinkedGE* pEdge = LinkedGE::cast( pObj );
	//TTunnel* pTTunel = TTunnel::cast(pObj);

	pObj->close();
	
	int colorIndx = pEdge->colorIndex();
	//acutPrintf(_T("\n��ɫ����:%d"),colorIndx);
	//CheakLayerExit(_T("��������"),colorIndx);
	AcGePoint3d spt,ept;
	pEdge->getSEPoint( spt, ept );
	AcGeVector3d v = ept - spt;
	double angle = v.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis);

    if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	//bool isOppositeDir = false;
	//CString method;

	//CString name;
	//DataHelper::GetPropertyData( objId, _T( "����" ), name );

	//if (ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId ))
	//{
	//	DataHelper::GetPropertyData( objId, _T( "ͨ�緽��" ), method );
	//}
	//else
	//{
	//	AcDbObjectId tWorkId = GetRelatedTW(objId);
	//	DataHelper::GetPropertyData( tWorkId, _T( "ͨ�緽��" ), method );
	//}

	//if ( _T("������") != name && _T("�ط���") != name)
	//{
	//	if (_T("ѹ��ʽ") == method || _T("��ѹ�̳�") == method)
	//	{
	//		isOppositeDir = true;
	//	}
	//}

	//if (true == isOppositeDir)
	//{
	//	angle = PI + angle;
	//}

	angle = ControlDirByMethods(objId,angle);
    CreateDirection( objId, pt, angle, colorIndx );
}

static void CreatJoint( const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
	Joint* pStation = new Joint( pt, angle );
	// �����������
	pStation->setRelatedGE( host );

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pStation ) ) delete pStation;
}

void DrawCmd::DrawJoint()
{
	AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ�����������������:" ) );
	if( objId.isNull() ) return;
	if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;

	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return;
	}

	LinkedGE* pEdge = LinkedGE::cast( pObj );
	if( pEdge == 0 )
	{
		actrTransactionManager->abortTransaction();
		return;
	}
	actrTransactionManager->endTransaction();
	AcGePoint3d spt,ept;
	pEdge->getSEPoint(spt,ept);
	AcGePoint3d pt;
	if( !PromptInsertPt( objId, pt ) ) return;

	double angle;
	if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	SplitByPoint(objId,spt,ept,pt);
	CreatJoint( objId, pt, angle );
}

void DrawCmd::DrawChimney( void )
{
    //acutPrintf( _T( "\n���Ʒ�Ͳ����..." ) );

    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ���Ͳ����Ӧ�ľ���:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "LocalFan" ), objId ) ) return;

    //if( !objIds.isEmpty() )
    //{
    //    AfxMessageBox( _T( "�þ���������������˷�Ͳ!" ) );
    //    return;
    //}

    AcGePoint3dArray pts;
    PolyLineJig jig;
    if( !jig.doJig( pts ) ) return;

    int len = pts.length();
    //acutPrintf( _T( "\n�����:%d" ), len );
    if( len < 2 ) return;

    Chimney* pChimney = new Chimney();
    pChimney->setRelatedGE( objId );

    for( int i = 0; i < len; i++ ) pChimney->addControlPoint( pts[i] );

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pChimney ) ) delete pChimney;

	AcDbObjectIdArray objIds;
	DrawHelper::GetTagGEById2( objId, _T( "Chimney" ), objIds );

	//��ͬһ���������ķ�Ͳ�ϲ���һ��
	//int numChim = objIds.length();
	//for(int i = 0; i < numChim-1; i++)
	//{
	//	Merging(objIds[i],objIds[i+1]);
	//}
}

void DrawCmd::DrawWindLibrary( void )
{
	AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ�����:" ) );
	if( objId.isNull() ) return;
	if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;


	AcGePoint3d pt,insertPt;

	double angle;

	if( !ArxUtilHelper::PromptPt( _T( "\n��ָ�����Ĳ��������:" ), pt ) ) return;

	if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	AcGeVector3d v = AcGeVector3d(AcGeVector3d::kXAxis);
	v.rotateBy(angle - PI/2,AcGeVector3d::kZAxis);
	v.normalize();

	insertPt = pt + v * 60;

	WindLibrary* pWindLib = new WindLibrary( insertPt, angle ); 	
	if( pWindLib == 0 ) return;


	pWindLib->setRelatedGE( objId ); // �������

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pWindLib ) ) delete pWindLib;

}


static void QTagGEDrawed(const AcDbObjectId& chimId)
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
	AcGePoint3dArray ctlPts = 	pChim->getControlPoint();

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

void DrawCmd::DrawQTagGE()
{
	AcDbObjectIdArray chims;
	DrawHelper::FindMineGEs(_T("Chimney"),chims);
	int len = chims.length();
	//acutPrintf(_T("\n��Ͳ��:%d"),len);
	if(chims.isEmpty()) return;
	for (int i = 0; i < len; i++)
	{
		QTagGEDrawed(chims[i]);
	}

}

static void EffectRanDrawed(AcDbObjectId ttunel)
{
	AcTransaction *pTrans = actrTransactionManager->startTransaction();
	if ( 0 == pTrans ) return;
	AcDbObject *pObj;
	if (Acad::eOk != pTrans->getObject(pObj,ttunel,AcDb::kForRead)) return;
	TTunnel *pTTunnel = TTunnel::cast(pObj);
	if ( 0 == pTTunnel)
	{
		actrTransactionManager->abortTransaction();
		return;
	}
	AcGePoint3d spt,ept;
	pTTunnel->getSEPoint(spt,ept);
	double angle = pTTunnel->getAngle();
	actrTransactionManager->endTransaction();

	AcDbObjectIdArray eTags;
	DrawHelper::GetTagGEById2( ttunel, _T( "EffectRanTagGE" ), eTags );
	if (!eTags.isEmpty())
	{
		ArxEntityHelper::EraseObjects( eTags, true );
	}

	AcGeVector3d v = ept - spt;
	double diatance = v.length();
	CString area,way;
	if(!DataHelper::GetPropertyData(ttunel,_T("�������"),area)) return;
	if(!DataHelper::GetPropertyData(ttunel,_T("ͨ�緽��"),way)) return;

	double minDistan,maxDistan;
	if(way.IsEmpty()) return;
	if(area.IsEmpty()) return;

	if (_T("ѹ��ʽ") == way || _T("��ѹ�̳�") == way)
	{
		minDistan = 4*sqrtf(_tstof(area));
		maxDistan = 5*sqrtf(_tstof(area));
	}

	else
	{
		minDistan = 0;
		maxDistan = 1.5*sqrtf(_tstof(area));
	}
	EffectRanTagGE *pTag = new EffectRanTagGE(ept,angle,minDistan,maxDistan,diatance*0.1);
	if (0 == pTag) return;

	pTag->setRelatedGE(ttunel);
	if( !ArxUtilHelper::PostToModelSpace( pTag ) ) delete pTag;
}

void DrawCmd::DrawEffectRanGE()
{
	AcDbObjectIdArray ttunels;
	DrawHelper::FindMineGEs(_T("TTunnel"),ttunels);
	int len = ttunels.length();
	if(ttunels.isEmpty()) return;
	for (int i = 0; i < len; i++)
	{
		EffectRanDrawed(ttunels[i]);
	}

}

void DrawCmd::DrawGasTube( GASTUBETYPE GEName )
{
	int colorIndx;
	CString layerName,lineTypeName;

	switch(GEName)
	{
	case PD_GAS_TUBE:
		layerName = _T("���ó����˹��·");
		lineTypeName = _T("FENCELINE1");
		colorIndx = 1;	//��ɫ
		break;
	case MPE_GAS_TUBE:
		layerName = _T("�ƶ�������˹��·");
		lineTypeName = _T("ACAD_ISO03W100");
		colorIndx = 5;	//��ɫ
		break;
	case MPD_GAS_TUBE:
		layerName = _T("�ƶ��ó���˹��·");
		lineTypeName = _T("HOT_WATER_SUPPLY");
		colorIndx = 3;	//��ɫ
		break;
	default:
		colorIndx = 7;	//��ɫ
		break;
	}

	AcGePoint3d startPt, endPt;
	//CString GEName =  _T( "�������" );
	double angle;
	if( !PromptSEPt(layerName, startPt, endPt, angle ) ) return;
	// 3������ͼԪ
	GasTube* pGasTube = new GasTube( startPt, endPt );

	CheakLayerExit(layerName,colorIndx,AcDb::kLnWt030);
	pGasTube->setColorIndex(colorIndx);

	pGasTube->setLayer(layerName);
	
	AddLineType(lineTypeName);
	pGasTube->setLinetype(lineTypeName);
	// 4����ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pGasTube ) ) delete pGasTube;
	//bool ret = ArxUtilHelper::PostToModelSpace( pGasTube );
	//acutPrintf(_T("\n�ύ״̬��%s"),ret ? _T("�ɹ�"):_T("ʧ��"));
}

void DrawCmd::DrawGasPump( void )
{
	AcDbObjectId objId;
	if(!CheackTubeLenth(objId)) return;

	AcGePoint3d pt;

	double angle;

	if( !ArxUtilHelper::PromptPt( _T( "\n��ָ����˹�õĲ��������:" ), pt ) ) return;

	if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	AcGePoint3d spt,ept;
	GetSEPt(objId,spt,ept);
	//SplitByPoint(objId,spt,ept,pt);
	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( angle, AcGeVector3d::kZAxis );
	v.normalize();

	AcGePoint3d pt2 = pt + v*3;
	GasPumpGE* pGasPump = new GasPumpGE( pt, pt2 ); 	
	if( pGasPump == 0 ) return;
	//pGasPump->
	CString GEName = _T("��˹��");
	CheakLayerExit(GEName,7,AcDb::kLnWt025);
	pGasPump->setLayer(GEName);

	//pGasPump->setRelatedGE( objId ); 

	//// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pGasPump ) ) delete pGasPump;
}

void DrawCmd::DrawValve( void )
{
	AcDbObjectId objId;
	if(!CheackTubeLenth(objId)) return;

	AcGePoint3d pt,insertPt;
	double angle;

	if( !ArxUtilHelper::PromptPt( _T( "\n��ָ�����Ĳ��������:" ), pt ) ) return;

	if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	insertPt = pt;

	ValveGE* pValveGE = new ValveGE( insertPt, angle ); 	
	if( pValveGE == 0 ) return;

	CString GEName = _T("����");
	CheakLayerExit(GEName,7,AcDb::kLnWt025);
	pValveGE->setLayer(GEName);

	pValveGE->setRelatedGE( objId );

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pValveGE ) ) delete pValveGE;
}

void DrawCmd::DrawTailrace( void )
{
	AcDbObjectId objId;
	if(!CheackTubeLenth(objId)) return;

	AcGePoint3d pt,insertPt;

	double angle;

	if( !ArxUtilHelper::PromptPt( _T( "\n��ָ����ˮ���Ĳ��������:" ), pt ) ) return;

	if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	insertPt = pt;

	TailraceGE* pTailraceGE = new TailraceGE( insertPt, angle ); 	
	if( pTailraceGE == 0 ) return;

	CString GEName = _T("��ˮ��");
	CheakLayerExit(GEName,7,AcDb::kLnWt025);
	pTailraceGE->setLayer(GEName);

	pTailraceGE->setRelatedGE( objId );

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pTailraceGE ) ) delete pTailraceGE;
}

void DrawCmd::DrawFlowmeter( void )
{
	AcDbObjectId objId;
	if(!CheackTubeLenth(objId)) return;

	AcGePoint3d pt,insertPt;

	double angle;

	if( !ArxUtilHelper::PromptPt( _T( "\n��ָ�������ƵĲ��������:" ), pt ) ) return;

	if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	insertPt = pt;

	FlowmeterGE* pFlowmeterGE = new FlowmeterGE( insertPt, angle ); 	
	if( pFlowmeterGE == 0 ) return;

	CString GEName = _T("������");
	CheakLayerExit(GEName,7,AcDb::kLnWt025);
	pFlowmeterGE->setLayer(GEName);

	pFlowmeterGE->setRelatedGE( objId );

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pFlowmeterGE ) ) delete pFlowmeterGE;

}

void DrawCmd::DrawBackFire( void )
{
	AcDbObjectId objId;
	if(!CheackTubeLenth(objId)) return;

	AcGePoint3d pt,insertPt;

	double angle;

	if( !ArxUtilHelper::PromptPt( _T( "\n��ָ��������Ĳ��������:" ), pt ) ) return;

	if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	insertPt = pt;

	BackFireGE* pBackFireGE = new BackFireGE( insertPt, angle ); 	
	if( pBackFireGE == 0 ) return;

	CString GEName = _T("�����");
	CheakLayerExit(GEName,7,AcDb::kLnWt025);
	pBackFireGE->setLayer(GEName);

	pBackFireGE->setRelatedGE( objId );

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pBackFireGE ) ) delete pBackFireGE;

}

void DrawCmd::DrawDetermineHole( void )
{
	AcDbObjectId objId;
	if(!CheackTubeLenth(objId)) return;

	AcGePoint3d pt,insertPt;

	double angle;

	if( !ArxUtilHelper::PromptPt( _T( "\n��ָ���ⶨ�׵Ĳ��������:" ), pt ) ) return;

	if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	insertPt = pt;

	DetermineHoleGE* pDetermineHoleGE = new DetermineHoleGE( insertPt, angle ); 	
	if( pDetermineHoleGE == 0 ) return;

	CString GEName = _T("�ⶨ��");
	CheakLayerExit(GEName,7,AcDb::kLnWt025);
	pDetermineHoleGE->setLayer(GEName);

	pDetermineHoleGE->setRelatedGE( objId );

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pDetermineHoleGE ) ) delete pDetermineHoleGE;

}

static void CreateGasFlow( const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
	GasFlowTagGE* pGasFlowTagGE = new GasFlowTagGE( pt, angle );
	// �����������
	pGasFlowTagGE->setRelatedGE( host );

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pGasFlowTagGE ) ) delete pGasFlowTagGE;
	//acutPrintf(_T("\nobjId:%d"),pGasFlowTagGE->objectId());
	//CString vv;
	//DataHelper::GetPropertyData(pGasFlowTagGE->objectId(),_T("��˹����"),vv);
	//acutPrintf(_T("\n��˹����ֵ:%s"),vv);
}

void DrawCmd::DrawGasFlow()
{
	AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ����·:" ) );
	if( objId.isNull() ) return;
	if( !ArxUtilHelper::IsEqualType( _T( "GasTube" ), objId ) ) return;

	AcGePoint3d pt;
	if( !PromptInsertPt( objId, pt ) ) return;

	double angle;
	if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	CreateGasFlow( objId, pt, angle );
	//CString v;
	//DataHelper::GetPropertyData(objId,_T("��˹����"),v);
	//acutPrintf(_T("\nֵ:%s"),v);
}

void DrawCmd::DrawDrill( void )
{
	AcGePoint3d startPt, endPt;
	double angle;
	CString GEName = _T("���");
	if( !PromptSEPt( GEName, startPt, endPt, angle ) ) return;
	// 3������ͼԪ
	DrillGE* pDrillGE = new DrillGE( startPt, endPt );

	CheakLayerExit( GEName, 7,AcDb::kLnWt025);

	pDrillGE->setLayer(GEName);

	AddLineType(_T("DASHED2"));
	pDrillGE->setLinetype(_T("DASHED2"));
	pDrillGE->setLineWeight(AcDb::kLnWt025);
	// 4����ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pDrillGE ) ) delete pDrillGE;
}

static void DrawSensor(AcDbObjectId& objId,const CString& GEName,AcGePoint3d& pt,double& angle)
{
	objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ����·:" ) );
	if( objId.isNull() ) return;
	if( !ArxUtilHelper::IsEqualType( _T( "GasTube" ), objId ) ) return;

	//CString GEName = _T("�¶ȴ�����");

// 	AcGePoint3d pt;
	if( !PromptInsertPt( objId, pt ) ) return;
// 	double angle;
	if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( angle, AcGeVector3d::kZAxis );
	if( v.x < 0 ) v.negate();
	angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
	v.normalize();
	v.rotateBy( PI / 2, AcGeVector3d::kZAxis ); // ʼ�������ַ���

	pt = pt + 6 * v;
}

void DrawCmd::DrawTempeSensor()
{
	AcGePoint3d pt;
	double angle;
	AcDbObjectId objId;
	CString GEName = _T("�¶ȴ�����");
	DrawSensor(objId,GEName,pt,angle);
	TempeSensorGE* pTempeSensorGE = new TempeSensorGE( pt, angle );
	// ��������˹��·��
	pTempeSensorGE->setRelatedGE( objId );

	CheakLayerExit( GEName, 3, AcDb::kLnWt025);
	pTempeSensorGE->setLayer(GEName);
	pTempeSensorGE->setLineWeight(AcDb::kLnWt025);

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pTempeSensorGE ) ) delete pTempeSensorGE;
}

void DrawCmd::DrawFlowSensor()
{
	AcGePoint3d pt;
	double angle;
	AcDbObjectId objId;
	CString GEName = _T("����������");
	DrawSensor(objId,GEName,pt,angle);

	FlowSensorGE* pFlowSensorGE = new FlowSensorGE( pt, angle );
	// ��������˹��·��
	pFlowSensorGE->setRelatedGE( objId );

	CheakLayerExit( GEName, 3,AcDb::kLnWt025 );
	pFlowSensorGE->setLayer(GEName);
	pFlowSensorGE->setLineWeight(AcDb::kLnWt025);

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pFlowSensorGE ) ) delete pFlowSensorGE;
}



void DrawCmd::DrawCOSensor( void )
{
	AcGePoint3d pt;
	double angle;
	AcDbObjectId objId;
	CString GEName = _T("CO������");
	DrawSensor(objId,GEName,pt,angle);

	COSensorGE* pCOSensorGE = new COSensorGE( pt, angle );
	// ��������˹��·��
	pCOSensorGE->setRelatedGE( objId );

	CheakLayerExit( GEName, 3,AcDb::kLnWt025 );
	pCOSensorGE->setLayer(GEName);
	pCOSensorGE->setLineWeight(AcDb::kLnWt025);

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pCOSensorGE ) ) delete pCOSensorGE;
}
void DrawCmd::DrawDifferPressSensor()
{
	//AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ����·:" ) );
	//if( objId.isNull() ) return;
	//if( !ArxUtilHelper::IsEqualType( _T( "GasTube" ), objId ) ) return;

	//CString GEName = _T("ѹ�����");

	//AcGePoint3d pt;
	//if( !PromptInsertPt( objId, pt ) ) return;
	//double angle;
	//if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	//AcGeVector3d v( AcGeVector3d::kXAxis );
	//v.rotateBy( angle, AcGeVector3d::kZAxis );
	//if( v.x < 0 ) v.negate();
	//angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
	//v.normalize();
	//v.rotateBy( PI / 2, AcGeVector3d::kZAxis ); // ʼ�������ַ���

	//pt = pt + 6 * v;

	AcGePoint3d pt;
	double angle;
	AcDbObjectId objId;
	CString GEName = _T("ѹ�����");
	DrawSensor(objId,GEName,pt,angle);

	DifferPressSensorGE* pDifferPressSensorGE = new DifferPressSensorGE( pt, angle );
	// ��������˹��·��
	pDifferPressSensorGE->setRelatedGE( objId );

	CheakLayerExit( GEName, 3 ,AcDb::kLnWt025);
	pDifferPressSensorGE->setLayer(GEName);
	pDifferPressSensorGE->setLineWeight(AcDb::kLnWt025);

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pDifferPressSensorGE ) ) delete pDifferPressSensorGE;
}

void DrawCmd::DrawGasSensor()
{
	//AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ����·:" ) );
	//if( objId.isNull() ) return;
	//if( !ArxUtilHelper::IsEqualType( _T( "GasTube" ), objId ) ) return;

	//CString GEName = _T("��˹������");

	//AcGePoint3d pt;
	//if( !PromptInsertPt( objId, pt ) ) return;
	//double angle;
	//if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	//AcGeVector3d v( AcGeVector3d::kXAxis );
	//v.rotateBy( angle, AcGeVector3d::kZAxis );
	//if( v.x < 0 ) v.negate();
	//angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
	//v.normalize();
	//v.rotateBy( PI / 2, AcGeVector3d::kZAxis ); // ʼ�������ַ���

	//pt = pt + 6 * v;

	AcGePoint3d pt;
	double angle;
	AcDbObjectId objId;
	CString GEName = _T("��˹������");
	DrawSensor(objId,GEName,pt,angle);

	GasSensorGE* pGasSensorGE = new GasSensorGE( pt, angle );
	// ��������˹��·��
	pGasSensorGE->setRelatedGE( objId );

	CheakLayerExit( GEName, 3,AcDb::kLnWt025 );
	pGasSensorGE->setLayer(GEName);
	pGasSensorGE->setLineWeight(AcDb::kLnWt025);

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pGasSensorGE ) ) delete pGasSensorGE;
}
