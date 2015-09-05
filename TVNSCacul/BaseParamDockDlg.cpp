// BaseParamDockDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BaseParamDockDlg.h"
//#include "commCtrl.h"
#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"
#include "../MineGE/PropertyDataUpdater.h"
#include "../MineGE/FuncFieldHelper.h"
#include "../MineGE/DataHelper.h"
#include "../MineGE/FieldHelper.h"

IMPLEMENT_DYNAMIC(BaseParamDockDlg, AcadDialog)
BaseParamDockDlg::BaseParamDockDlg( CWnd* pParent /*= NULL*/, BOOL bModal /*= FALSE*/ )
: AcadDialog(BaseParamDockDlg::IDD, pParent,bModal)
{

}
BaseParamDockDlg::~BaseParamDockDlg()
{
}

void BaseParamDockDlg::DoDataExchange(CDataExchange* pDX)
{
	AcadDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BaseParamDockDlg, AcadDialog)
	ON_REGISTERED_MESSAGE( AFX_WM_PROPERTY_CHANGED, OnPropertyChanged )
END_MESSAGE_MAP()

void BaseParamDockDlg::readPropertyData()
{
	// ��ȡ��������
	CString type;
	DataHelper::GetTypeName( m_objId, type );
	//acutPrintf(_T("\n����:%s"),type);

	AcStringArray funcs,funcFieldsInfo;
	FuncFieldHelper::GetFunctions(funcs);
	int funcNum = funcs.length();
	for (int i = 0; i < funcNum; i++)
	{
		AcStringArray fields;
		if(!FuncFieldHelper::GetFields(funcs[i].kACharPtr(),type,fields)) return;
		if(fields.isEmpty()) continue;
		//acutPrintf(_T("\n����:%s"),funcs[i].kACharPtr());
		funcFieldsInfo.append(_T("$"));
		funcFieldsInfo.append(funcs[i]);
		for(int j = 0; j < fields.length(); j++)
		{
			funcFieldsInfo.append(fields[j]);
			//acutPrintf(_T("\n�ֶ�����:%s"),fields[j].kACharPtr());
		}
	}

	// �����������ݿؼ�
	PropertyDataUpdater::BuildPropGridCtrl( &m_propertyDataList, type,funcFieldsInfo );

	// �����������
	bool ret = PropertyDataUpdater::ReadDataFromGE( &m_propertyDataList, m_objId );
	
	 //�������ʧ��
	if( !ret )
	{
		//acutPrintf(_T("\nʧ��~~~~~~~~~"));
		acutPrintf(_T("\n�������ʧ��(ReadDataFromGE)"));
		m_propertyDataList.EnableWindow( FALSE );
		m_propertyDataList.ShowWindow( SW_HIDE );
	}
	//UpdateData(FALSE);
	m_propertyDataList.ExpandAll(FALSE);

}

void BaseParamDockDlg::writePropertyData()
{
	bool ret = PropertyDataUpdater::WriteDataToGE( &m_propertyDataList, m_objId );
	int pPropCount = m_propertyDataList.GetPropertyCount();
	//acutPrintf(_T("\n���ݱ���%s"),ret?_T("�ɹ�"):_T("ʧ��"));
}

BOOL BaseParamDockDlg::OnInitDialog()
{
	AcadDialog::OnInitDialog();
	if(!SingleDataObjectHelper::GetObjectId(_T("�������"),m_objId)) return FALSE;
	//����, ��λ, ��ʾCMFCPropertyGridCtrl
	CRect rect;
	GetDlgItem( IDC_PROPERTY_GRID )->GetWindowRect( &rect );
	ScreenToClient( &rect );
	m_propertyDataList.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rect, this, ( UINT ) - 1 );
	//��boolֵ��ɡ���/��
	//m_propertyDataList.SetBoolLabels(_T("��"),_T("��"));
	updateProperty(false);
	return TRUE;
}

LRESULT BaseParamDockDlg::OnPropertyChanged( WPARAM,LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lParam;
	//int i = (int)pProp->GetData();
	CString filedName = pProp->GetName();
	COleVariant changedValue = pProp->GetValue();
	//t?=?pProp->GetOriginalValue();??//�ı�֮ǰ��ֵ??
	int groupCount = m_propertyDataList.GetPropertyCount();
	for( int i = 0; i < groupCount; i++)
	{
		CMFCPropertyGridProperty* pGroup = m_propertyDataList.GetProperty(i);
		int propCount = pGroup->GetSubItemsCount();
		for( int j = 0; j < propCount; j++ )
		{
			CMFCPropertyGridProperty* pPro = pGroup->GetSubItem(j);
			if (pPro->GetName() == filedName && pPro != pProp)
			{
				pPro->SetValue(changedValue);
			}
		}
	}
	updateProperty(true);
	return 0;
}

void BaseParamDockDlg::onClosing()
{
	updateProperty(true);
}

void BaseParamDockDlg::updateProperty(bool writeOrRead)
{
	acDocManager->lockDocument( curDoc() );
	if(writeOrRead)
	{
		UpdateData(TRUE);
		// ��������
		writePropertyData(  );
	}
	else
	{
		// ��ȡ����
		readPropertyData(  );
		UpdateData(FALSE);
	}
	acDocManager->unlockDocument( curDoc() );
}
