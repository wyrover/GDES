#include "StdAfx.h"
#include "PropertyDataDlgHelper.h"

#include "PropertyDataDlg.h"
#include "PropertyDataDlgHelper.h"
#include "WcyPropertyDlg.h"

#include "GasWidthCaculDlg.h"
//#include "NearByGasDlg.h"
//#include "ValueHelper.h"

#include "../MineGE/FieldHelper.h"
#include "../MineGE/DataHelper.h"
#include "../MineGE/FuncFieldHelper.h"

static void setFieldInfo(CString type,CString field, FieldInfo fieldInfo)
{

	// �������ֶ�
	int index = FieldHelper::FindField(type,field);
	if( index < 1 || !FieldInfoHelper::FindFieldInfo( type, field ) )
	{
		FieldInfoHelper::AddFieldInfo( type, field, fieldInfo );
	}
	else
	{
		// �������е��ֶ���Ϣ
		FieldInfoHelper::RemoveFieldInfo( type, field );
		FieldInfoHelper::AddFieldInfo( type, field, fieldInfo );
	}

}

static bool AddNearByFields( const AcDbObjectId& objId, const CString& func)
{
	CString type = _T("�������");
	CString numMine;
	DataHelper::GetPropertyData(objId,_T("�ڽ���ú�����"),numMine);
	int dNumMine = _ttoi(numMine);
	if(dNumMine <= 0)
	{
		AfxMessageBox(_T("û���ڽ���"));
		return false;
	}

	for (int i = 0; i < dNumMine; i++)
	{
		CString strmi,strX0i,strXi,strEntai;
		strmi.Format(_T("��%d�ڽ����ú���"),i+1);
		strX0i.Format(_T("��%d�ڽ����ԭʼ��˹����"),i+1);
		strXi.Format(_T("��%d�ڽ���Ĳ�����˹����"),i+1);
		strEntai.Format(_T("��%d�ڽ������˹ӿ����"),i+1);

		FieldInfo fieldInfo;
		fieldInfo.m_dt = DT_NUMERIC;
		fieldInfo.m_minValue = 0;
		fieldInfo.m_maxValue = 999999;
		fieldInfo.m_enable = true;
		fieldInfo.m_showInTooltip = true;
		fieldInfo.m_descr = _T("��λ:m");

		FuncFieldHelper::AddField(func,type,strmi);
		FieldHelper::AddField(type,strmi);
		setFieldInfo(type,strmi,fieldInfo);

		FuncFieldHelper::AddField(func,type,strX0i);
		FieldHelper::AddField(type,strX0i);
		fieldInfo.m_descr = _T("��λ:m3/t");
		setFieldInfo(type,strX0i,fieldInfo);

		FuncFieldHelper::AddField(func,type,strXi);
		FieldHelper::AddField(type,strXi);
		fieldInfo.m_descr = _T("��λ:m3/t");
		setFieldInfo(type,strXi,fieldInfo);

		FuncFieldHelper::AddField(func,type,strEntai);
		FieldHelper::AddField(type,strEntai);
		fieldInfo.m_descr = _T("��λ:%");
		fieldInfo.m_enable = false;
		setFieldInfo(type,strEntai,fieldInfo);
	}

	while(dNumMine > 0)
	{
		CString strmi,strX0i,strXi,strEntai;
		strmi.Format(_T("��%d�ڽ����ú���"),dNumMine+1);
		strX0i.Format(_T("��%d�ڽ����ԭʼ��˹����"),dNumMine+1);
		strXi.Format(_T("��%d�ڽ���Ĳ�����˹����"),dNumMine+1);
		strEntai.Format(_T("��%d�ڽ������˹ӿ����"),dNumMine+1);
		if(FuncFieldHelper::RemoveField(func,type,strmi) 
			&& FuncFieldHelper::RemoveField(func,type,strX0i) 
			&& FuncFieldHelper::RemoveField(func,type,strXi)
			&& FuncFieldHelper::RemoveField(func,type,strEntai))
		{
			dNumMine += 1;
		}
		else
		{
			dNumMine = 0;
		}
	}

	return true;

}
void PropertyDataDlgHelper::DisplayAllPropertyDataDlg( const AcDbObjectId& objId )
{
    if( objId.isNull() ) return;

    // �л���Դ
    CAcModuleResourceOverride myResources;

    PropertyDataDlg pdd;
    pdd.showAllData( true );
    pdd.setMineGE( objId );
    pdd.DoModal();
}


void PropertyDataDlgHelper::DisplayPartialPropertyDataDlg( const AcDbObjectId& objId, const AcStringArray& fields )
{
    if( objId.isNull() ) return;
    if( fields.isEmpty() ) return;

    // �л���Դ
    CAcModuleResourceOverride myResources;

    PropertyDataDlg pdd;
    pdd.showAllData( true );
    pdd.setMineGE( objId );
    int len = fields.length();
    for( int i = 0; i < len; i++ )
    {
        pdd.addField( fields[i].kACharPtr() );
		//acutPrintf(_T("\n�ֶ�:%s"),fields[i].kACharPtr());
    }
    pdd.DoModal();
}

bool PropertyDataDlgHelper::DisplayPropertyDataDlg( const AcDbObjectId& objId )
{
    if( objId.isNull() ) return false;

    AcDbObjectId dObjId;
    if( !DataHelper::GetDataObject( objId, dObjId ) ) return false;

    CString type;
    if( !DataHelper::GetTypeName( objId, type ) ) return false;

    AcStringArray fields;
    FieldHelper::GetAllFields( type, fields );
	//acutPrintf(_T("\n���ͣ�%s"),type);
    if( fields.isEmpty() )
    {
        CString msg;
        //msg.Format(_T("����:%s���ֶθ���Ϊ0��û�����ݿ�����ʾ..."), type);
        msg.Format( _T( "û�����ݿ�����ʾ..." ) );
        AfxMessageBox( msg );
        return false;
    }
    else
    {
		DisplayPartialPropertyDataDlg( dObjId, fields );
        return true;
    }
}

void PropertyDataDlgHelper::InputPropertyDataDlg( const AcDbObjectId& objId, const AcStringArray& fields, const CString& func )
{
	if( objId.isNull() ) return;
	if( fields.isEmpty() ) return;

	// �л���Դ
	CAcModuleResourceOverride myResources;
	if (_T("�������۵�ú��") == func)
	{
		GasWidthCaculDlg gaswidthdlg;
		gaswidthdlg.setMineGE(objId);
		int ret = gaswidthdlg.DoModal();
		if (IDCANCEL == ret) return;
	}

	if (_T("�ڽ�����˹ӿ����") == func)
	{
		if(!AddNearByFields(objId,func)) return;
	}

	if(_T("��������˹�����") == func)
	{
		CString strGD,strVD,strGDold,strVDold;
		DataHelper::GetPropertyData(objId,_T("��˹����ӿ����"),strGD);
		DataHelper::GetPropertyData(objId,_T("������˹��"),strVD);
		DataHelper::GetPropertyData(objId,_T("���¹�������ƽ����˹�����"),strGDold);
		DataHelper::GetPropertyData(objId,_T("���¹����������˹��"),strVDold);
		double ret = _tstof(strGD) - _tstof(strVD);
		CString strRet;
		strRet.Format(_T("%.2lf"),ret);
		if(_tstof(strGDold) <= 0 && ret >= 0)
			DataHelper::SetPropertyData(objId,_T("���¹�������ƽ����˹�����"),strRet);
		if(_tstof(strVDold) <= 0)
			DataHelper::SetPropertyData(objId,_T("���¹����������˹��"),strVD);
	}

	//if (_T("��·����") == func)
	//{
	//	ValueHelper::setGasAirValue(objId);
	//}

	//if(_T("ͨ����˹����ʼ���") == func || _T("ͨ��������˹ѹ������") == func)
	//{
	//	WcyPropertyDlg pdd(NULL,func);
	//	pdd.showAllData( true );
	//	pdd.setMineGE( objId );
	//	int len = fields.length();
	//	for( int i = 0; i < len; i++ )
	//	{
	//		pdd.addField( fields[i].kACharPtr() );
	//		//acutPrintf(_T("\n�ֶ�:%s"),fields[i].kACharPtr());
	//	}
	//	pdd.DoModal();
	//}
	//else
	//{
		PropertyDataDlg pdd(NULL,func);
		pdd.showAllData( true );
		pdd.setMineGE( objId );
		int len = fields.length();
		for( int i = 0; i < len; i++ )
		{
			pdd.addField( fields[i].kACharPtr() );
			//acutPrintf(_T("\n�ֶ�:%s"),fields[i].kACharPtr());
		}
		pdd.DoModal();
	//}
}

bool PropertyDataDlgHelper::DisplayPropertyByFunName( const AcDbObjectId& objId, const CString funName )
{
	if( objId.isNull() ) return false;

	AcDbObjectId dObjId;
	if( !DataHelper::GetDataObject( objId, dObjId ) ) return false;

	CString type;
	if( !DataHelper::GetTypeName( objId, type ) ) return false;

	AcStringArray fields;
	FuncFieldHelper::GetFields(funName,type,fields);
	//for (int i = 0; i < fields.length(); i++)
	//{
	//	acutPrintf(_T("\n����:%s\t�ֶ�:%s"),funName,fields[i]);
	//}
	//FieldHelper::GetAllFields( type, fields );
	if( fields.isEmpty() )
	{
		CString msg;
		//msg.Format(_T("����:%s���ֶθ���Ϊ0��û�����ݿ�����ʾ..."), type);
		msg.Format( _T( "û�����ݿ�����ʾ..." ) );
		AfxMessageBox( msg );
		return false;
	}
	InputPropertyDataDlg( dObjId, fields,funName );
	return true;
}