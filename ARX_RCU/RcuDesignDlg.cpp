#include "stdafx.h"
#include "RcuDesignDlg.h"
#include "RcuEditDrillSiteDlg.h"

#include "Rcu.h"
#include "RcuHelper.h"
#include "RcuDataLink.h"
#include "ListCtrlHelper.h"
#include "SwitchHelper.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

#include "../DefGE/RcuGE.h"
#include "../DefGE/Pore.h"
#include "../DefGE/DrillSite.h"
//#include "../DefGE/RockGate.h"
#include "../DefGE/CoalSurface.h"

//�Զ�����Ϣ
#include "MyMsg.h"

//΢��list1�ı�ͷ�п��
static void CrackHeadColumnWidth(CListCtrl& m_list)
{
	m_list.SetColumnWidth(1, int(m_list.GetColumnWidth(1)*1.2));
	for(int i=5;i<m_list.GetHeaderCtrl()->GetItemCount()+1;i++)
	{
		m_list.SetColumnWidth(i, int(m_list.GetColumnWidth(i)*1.5));
	}
	m_list.SetColumnWidth(7, int(m_list.GetColumnWidth(7)*1.5));
}

//�޸��곡�б��n�е�����
static void ModifyDrillSiteToListCtrl(CListCtrl& m_list, int n, DrillSiteLink& ds_link)
{
	{
		m_list.SetItemText( n, 1, ds_link.m_name );
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_width, value);
		m_list.SetItemText( n, 2, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_height, value);
		m_list.SetItemText( n, 3, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_dist, value);
		m_list.SetItemText( n, 4, value);
	}
	{
		m_list.SetItemText( n, 5, ArxUtilHelper::Point3dToString(ds_link.m_pt));
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_left, value);
		m_list.SetItemText( n, 6, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_right, value);
		m_list.SetItemText( n, 7, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_top, value);
		m_list.SetItemText( n, 8, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_bottom, value);
		m_list.SetItemText( n, 9, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_pore_size, value);
		m_list.SetItemText( n, 10, value);
	}
}

//���곡�б�������һ��
static void InsertDrillSiteToListCtrl(CListCtrl& m_list, const AcDbObjectId& objId, DrillSiteLink& ds_link)
{
	int row = ListCtrlHelper::SearchRowOfList(m_list, objId);
	if(row == -1)
	{
		//����һ��
		row = ListCtrlHelper::InsertRowToListCtrl(m_list, objId);
	}
	//�����곡�б�ĸ�������
	ModifyDrillSiteToListCtrl(m_list, row, ds_link);
}


static bool UpdateDrillSiteDataFromDlg(const AcDbObjectId& drill_site, DrillSiteLink& ds_link, CoalSurfaceLink& cs_link)
{
	CAcModuleResourceOverride resourceOverride;

	//��ʼ���곡�Ի���
	RcuEditDrillSiteDlg dlg;
	dlg.m_drill_site = drill_site;
	//����곡id��Ϊ��,���ȡ���ݲ����µ��Ի���
	dlg.readFromDataLink(ds_link, cs_link);

	if(IDOK != dlg.DoModal()) return false;

	//�ӶԻ�������ȡ����
	dlg.writeToDataLink(ds_link, cs_link);
	

	//�����ݸ��µ�ͼԪ��
	ds_link.updateData(true);
	cs_link.updateData(true);

	return true;
}


//��ʽ���곡�����ƶԻ���ı���
static CString FormatPoreDlgTitle(const CString& ds_name)
{
	CString name = ds_name;
	name.Trim();
	if(name.IsEmpty())
	{
		name = _T("***");
	}

	CString title;
	title.Format(_T("%s�곡-������"), name);
	return title;
}


//static bool UpdateOpenPoresFromDlg(const AcDbObjectId& drill_site,  DrillSiteLink& ds_link)
//{
//	CAcModuleResourceOverride resourceOverride;
//	
//	//RcuEditOpenPoreDlg dlg;
//	////��ʽ�������ƶԻ���ı���
//	//dlg.m_title = FormatPoreDlgTitle(ds_link.m_name);
//	////������װ뾶
//	//dlg.m_pore_size = ds_link.m_pore_size;
//	////��ȡ��׵ĸ���
//	//AcDbObjectIdArray pores;
//	//RcuHelper::GetRelatedOpenPores(drill_site, pores);
//	//dlg.m_pore_num = pores.length();
//	////��ȡ��׵ļ��
//	////if(!pores.isEmpty())
//	////{
//	////	dlg.m_pore_gap = ds_link.m_pore_gap;
//	////}
//	////else
//	////{
//	////	dlg.m_pore_gap = 0;
//	////}
//
//	//if(IDOK != dlg.DoModal()) return false;
//
//	////�ӶԻ����ж�ȡ����
//	////��׼��
//	////ds_link.m_pore_gap = dlg.m_pore_gap;
//	////�������ݵ����
//	//ds_link.updateData(true);
//
//	////ɾ���������������
//	//RcuHelper::ClearRelatedOpenPores(drill_site);
//
//	//�����µ����ͼԪ
//	return RcuHelper::CreateOpenPores(drill_site, ds_link);
//}

// RcuDesignDlg �Ի���

IMPLEMENT_DYNAMIC(RcuDesignDlg, AcadDialog)

RcuDesignDlg::RcuDesignDlg( CWnd* pParent, BOOL bModal )
	: AcadDialog(RcuDesignDlg::IDD, pParent, bModal)
	, m_allPut(FALSE)
{

}
RcuDesignDlg::~RcuDesignDlg()
{
	//ClearListCtrlItemDatas(m_list);
}

void RcuDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	AcadDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Check(pDX, IDC_ALL_PUTOUT_CHECK, m_allPut);
}

BEGIN_MESSAGE_MAP(RcuDesignDlg, AcadDialog)
	ON_BN_CLICKED(IDC_BUTTON4, &RcuDesignDlg::OnBnClickedExport)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &RcuDesignDlg::OnNMDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &RcuDesignDlg::OnNMRclickList1)
	ON_COMMAND(ID_RCU_LIST_HILIGHT, &RcuDesignDlg::OnHilightDrillSiteCommand)
	ON_COMMAND(ID_RCU_LIST_MODIFY, &RcuDesignDlg::OnModifyDrillSiteCommand)
	ON_COMMAND(ID_RCU_LIST_DELETE, &RcuDesignDlg::OnDeleteDrillSiteCommand)
	ON_COMMAND(ID_RCU_LIST_ADD, &RcuDesignDlg::OnAddDrillSiteCommand)
	ON_COMMAND(ID_RCU_LIST_UPDATE, &RcuDesignDlg::OnUpdateDrillSiteListCommand)
	ON_COMMAND(ID_RCU_COAL_HILIGHT, &RcuDesignDlg::OnHilightCoalSurfCommand)
	ON_COMMAND(ID_RCU_COAL_SURF, &RcuDesignDlg::OnDisplayCoalSurfCommand)
	ON_COMMAND(ID_COAL_SURF_PORE_DESIGN, &RcuDesignDlg::OnDesiginClosePoresCommand)

	ON_COMMAND(IDR_DRILL_SITE_HILIGHT, &RcuDesignDlg::OnHilightDrillSiteCommand)
	//ON_COMMAND(IDR_DRILL_SITE_PORE_DESIGN, &RcuDesignDlg::OnDesiginOpenPoresCommand)
	//ON_COMMAND(IDR_DRILL_SITE_PORE_CLEAR, &RcuDesignDlg::OnClearOpenPoresCommand)

	ON_MESSAGE(WM_RCU_ADD, &RcuDesignDlg::OnRcuAddMessage)
	ON_MESSAGE(WM_RCU_DEL, &RcuDesignDlg::OnRcuDelMessage)
	ON_MESSAGE(WM_RCU_MODIFY, &RcuDesignDlg::OnRcuModifyMessage)
	ON_MESSAGE(WM_RCU_UPDATE, &RcuDesignDlg::OnRcuUpdateMessage)

END_MESSAGE_MAP()

BOOL RcuDesignDlg::OnInitDialog()
{
	AcadDialog::OnInitDialog();

	m_list.SetExtendedStyle( m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	AcStringArray drill_fields;
	FieldHelper::GetAllFields(_T("DrillSite"), drill_fields);
	//������ͷ�б�
	ListCtrlHelper::BuildListCtrlHeadColumns( m_list, drill_fields );
	CrackHeadColumnWidth(m_list);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void RcuDesignDlg::OnClosing()
{
	//ɾ���б���ÿһ�и���������(��Щ���ݶ���new������)
	//ClearListCtrlItemDatas(m_list);
	// �������
	//ClearList(m_list);
}

// RcuDesignDlg ��Ϣ�������

/**
http://blog.csdn.net/carl2380/article/details/4816392
http://blog.csdn.net/sjls2659/article/details/7580660
http://blog.csdn.net/oldmtn/article/details/9298577
����ListCtrlʱ��LVN_ITEMCHANGED��Ӧ3�ε���Ī������ı���
�������:
	�������´���
		if(LVIF_STATE == pNMLV->uChanged && (pNMLV->uNewState & LVIS_SELECTED))
		{
			int row = pNMLV->iItem;
		}
*/

void RcuDesignDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//�ڼ���
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	int row1 = pNMItemActivate->iItem;
	if( row1 != LB_ERR )
	{
		ItemData* pData1 = (ItemData*)m_list.GetItemData(row1);

		//�ĵ����л�
		DocumentLockSwitch lock_switch;
		//����ѡ��ͼԪ
		ArxEntityHelper::SelectEntity(pData1->objId);
		//��λ��ͼԪ
		ArxEntityHelper::ZoomToEntityForModeless(pData1->objId);
		//����pResultΪ1,���θ�������Ӧ����Ϣ
		*pResult = 1;
	}
}

void RcuDesignDlg::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//�ڼ���
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	int row = pNMItemActivate->iItem;
	//��ֹ�ڿհ�����������˵�
	//if (m_list.GetSelectedCount() > 0)
	{
		//�������δ���, ��������Ӧ��ListCtrl
		CMenu menu, *pPopup;
		menu.LoadMenu(IDR_RCU_LIST_MENU);
		pPopup = menu.GetSubMenu(0);
		CPoint myPoint;
		ClientToScreen(&myPoint);
		GetCursorPos(&myPoint); //���λ��
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, myPoint.x, myPoint.y,this);  
	}
	//����pResultΪ1,���θ�������Ӧ����Ϣ
	*pResult = 1;
}

void RcuDesignDlg::OnAddDrillSiteCommand()
{
	//�л�controlbar����ʾ
	//ControlBarShowSwitch cb_switch(this);

	//�л��ĵ���
	DocumentLockSwitch lock_switch;

	//ѡ���곡�Ĳ��������
	AcGePoint3d pt;
	if(!ArxUtilHelper::PromptPt(_T("\n��ѡ���곡�Ĳ�������꣺"), pt)) return;

	//���öԻ����ȡ�곡��ú������
	DrillSiteLink ds_link;
	CoalSurfaceLink cs_link;
	if(!UpdateDrillSiteDataFromDlg(AcDbObjectId::kNull, ds_link, cs_link)) return;

	//�����곡��ú��ͼԪ
	if(RcuHelper::CreateDrillSite(pt, ds_link, cs_link))
	{
		if(!RcuHelper::ModifyDrillSiteRelatedGEs(ds_link.getDataSource(), cs_link, ds_link)) return;

		//��list1�в���һ���곡����
		InsertDrillSiteToListCtrl(m_list, ds_link.getDataSource(), ds_link);

		//cad���ڻ�ȡ����
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnDeleteDrillSiteCommand()
{
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

		//�ĵ����л�
		DocumentLockSwitch lock_switch;

		//ɾ��ͼԪ
		//ע:EraseObject2ʹ��open/closeɾ������Ч�곡�͹�����ͼԪ
		//   ��EraseObject����ʹ�õ�������,��һ������Чɾ��!!!
		ArxEntityHelper::EraseObject2(pData1->objId, true);

		//ɾ��ѡ�����
		ListCtrlHelper::DeleteListCtrlItem(m_list, row1);

		//cad���ڻ�ȡ����
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnModifyDrillSiteCommand()
{
	//MessageBox(_T("RcuDesignDlg::OnLocateCommand"));
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 == LB_ERR)
	{
		MessageBox( _T( "��ȷ����ǰ��һ�б�ѡ��!" ) );
		return;
	}
	ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

	//�ĵ��л���
	DocumentLockSwitch lock_switch;
	
	//��ȡ�곡��ú������
	DrillSiteLink ds_link;
	CoalSurfaceLink cs_link;
	if(!RcuHelper::GetDrillSiteAndCoalSurfDatas(pData1->objId, ds_link, cs_link))
	{
		MessageBox(_T("��ȡ�곡��ú��ʧ��!!!"));
		return;
	}

	//�л�controlbar����ʾ
	//ControlBarShowSwitch cb_switch(this);

	//�ӶԻ����и�������
	if(UpdateDrillSiteDataFromDlg(pData1->objId, ds_link, cs_link))
	{
		if(!RcuHelper::ModifyCoalSurfParams(pData1->objId)) return;
		if(!RcuHelper::ModifyDrillSiteRelatedGEs(pData1->objId, cs_link, ds_link)) return;

		//�޸ĵ�ǰѡ���곡������
		ModifyDrillSiteToListCtrl(m_list, row1, ds_link);
		//acutPrintf(_T("\n����֮ǰ->���:%.4lf\t�߶�:%.4lf\n"),cs_link.m_width,cs_link.m_height);
		
		//cad���ڻ�ȡ����
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnHilightDrillSiteCommand()
{
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

		//�ĵ����л�
		DocumentLockSwitch lock_switch;
		//����ѡ���곡ͼԪ
		ArxEntityHelper::SelectEntity(pData1->objId);

		//cad���ڻ�ȡ����
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnUpdateDrillSiteListCommand()
{
	//���½���
	updateUI();
}

void RcuDesignDlg::OnHilightCoalSurfCommand()
{
	//MessageBox(_T("RcuDesignDlg::OnHilightCoalSurfCommand"));
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

		//�ĵ����л�
		DocumentLockSwitch lock_switch;

		AcDbObjectId coal_surf;
		RcuHelper::GetRelatedCoalSurface(pData1->objId, coal_surf);

		//��������λú��ͼԪ
		ArxEntityHelper::SelectEntity(coal_surf);
		ArxEntityHelper::ZoomToEntityForModeless(coal_surf);

		//cad���ڻ�ȡ����
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnDisplayCoalSurfCommand()
{
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

		//�ĵ����л�
		DocumentLockSwitch lock_switch;

		AcDbObjectId coal_surf;
		RcuHelper::GetRelatedCoalSurface(pData1->objId, coal_surf);
		
		CoalSurfaceLink cs_link;
		if(RcuHelper::ReadCoalSurfaceData(coal_surf, cs_link))
		{
			CString msg;
			msg.Format(_T("���:%.3f\t���:%.3f\n"), cs_link.m_thick, cs_link.m_angle);
			msg.AppendFormat(_T("��׳�ɰ뾶:%.3f\n"), cs_link.m_gas_radius);
			msg.AppendFormat(_T("��ɷ�Χ���:%.3f\t�߶�:%.3f\n"), cs_link.m_width, cs_link.m_height);
			msg.AppendFormat(_T("��ɷ�Χ���ĵ�����:(%.3f, %.3f, %.3f)\n"), cs_link.m_pt.x, cs_link.m_pt.y, cs_link.m_pt.z);
			msg.AppendFormat(_T("ƽ�淨����:(%.3f, %.3f, %.3f)\n"), cs_link.m_normV.x, cs_link.m_normV.y, cs_link.m_normV.z);
			msg.AppendFormat(_T("ú����������:(%.3f, %.3f, %.3f)\n"), cs_link.m_headV.x, cs_link.m_headV.y, cs_link.m_headV.z);
			msg.AppendFormat(_T("ú����������:(%.3f, %.3f, %.3f)"), cs_link.m_dipV.x, cs_link.m_dipV.y, cs_link.m_dipV.z);
			MessageBox(msg);
		}
	}
}

void RcuDesignDlg::OnDesiginClosePoresCommand()
{
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

		//�ĵ����л�
		DocumentLockSwitch lock_switch;

		AcDbObjectId coal_surf;
		RcuHelper::GetRelatedCoalSurface(pData1->objId, coal_surf);

		//ɾ�����е�ú�����
		RcuHelper::ClearRelatedClosePores(coal_surf);
		
		//cad���ڻ�ȡ����
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnBnClickedExport()
{
	UpdateData(TRUE);
	//�ĵ����л�
	DocumentLockSwitch lock_switch;
	if(m_allPut)
	{
		RcuHelper::CreatReportHelper(NULL);
		return;
	}
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 == LB_ERR)
	{
		MessageBox( _T( "��ȷ����ǰ��һ�б�ѡ��!" ) );
		return;
	}
	ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );
	
	RcuHelper::CreatReportHelper(pData1->objId);
}

void RcuDesignDlg::updateDrillSiteListCtrl(unsigned int op, const AcDbObjectId& drill_site)
{
	if(drill_site.isNull()) return;

	int row1 = ListCtrlHelper::SearchRowOfList(m_list, drill_site);
	if(op == 0)
	{
		if(row1 == LB_ERR)
		{
			//�����곡
			addDrillSite(drill_site);
		}
		else
		{
			//�޸��곡
			modifyDrillSite(row1);
		}
	}
	else if(op == 1 && row1 != LB_ERR)
	{
		//ɾ���곡
		delDrillSite(row1);
	}
	else if(op == 2 && row1 != LB_ERR)
	{
		//�޸��곡
		modifyDrillSite(row1);
	}
}

LRESULT RcuDesignDlg::OnRcuAddMessage(WPARAM wParam, LPARAM lParam)
{
	ArxMsg* pMsg = (ArxMsg*)wParam;
	if(pMsg->clsName.CompareNoCase(_T("DrillSite")) == 0)
	{
		updateDrillSiteListCtrl(0, pMsg->objId);
	}
	return 0;
} 

LRESULT RcuDesignDlg::OnRcuDelMessage(WPARAM wParam, LPARAM lParam)
{
	ArxMsg* pMsg = (ArxMsg*)wParam;
	if(pMsg->clsName.CompareNoCase(_T("DrillSite")) == 0)
	{
		updateDrillSiteListCtrl(1, pMsg->objId);
	}
	return 0;
}

LRESULT RcuDesignDlg::OnRcuModifyMessage(WPARAM wParam, LPARAM lParam)
{
	ArxMsg* pMsg = (ArxMsg*)wParam;
	if(pMsg->clsName.CompareNoCase(_T("DrillSite")) == 0)
	{
		updateDrillSiteListCtrl(2, pMsg->objId);
	}
	return 0;
}

LRESULT RcuDesignDlg::OnRcuUpdateMessage(WPARAM wParam, LPARAM lParam)
{
	//���½���
	updateUI();
	return 0;
}

void RcuDesignDlg::addDrillSite(const AcDbObjectId& drill_site)
{
	//�л��ĵ���
	DocumentLockSwitch lock_switch;

	//��ȡ�곡����
	DrillSiteLink ds_link;
	if(!RcuHelper::ReadDrillSiteData(drill_site, ds_link)) return;

	//��list1�в���һ���곡����
	InsertDrillSiteToListCtrl(m_list, drill_site, ds_link);
}

void RcuDesignDlg::delDrillSite(int row1)
{
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

		//�ĵ����л�
		DocumentLockSwitch lock_switch;

		//ɾ��ͼԪ
		//ע:EraseObject2ʹ��open/closeɾ������Ч�곡�͹�����ͼԪ
		//   ��EraseObject����ʹ�õ�������,��һ������Чɾ��!!!
		ArxEntityHelper::EraseObject2(pData1->objId, true);

		//ɾ��ѡ�����
		ListCtrlHelper::DeleteListCtrlItem(m_list, row1);
	}
}

void RcuDesignDlg::modifyDrillSite(int row1)
{
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData(row1);

		//�ĵ����л�
		DocumentLockSwitch lock_switch;

		DrillSiteLink ds_link;
		//��ȡ�곡�������������ݲ���䵽�Ի�����
		if(!RcuHelper::ReadDrillSiteData(pData1->objId, ds_link)) return;

		//�޸ĵ�ǰѡ���곡������
		ModifyDrillSiteToListCtrl(m_list, row1, ds_link);
	}
}

void RcuDesignDlg::updateUI()
{
	//����곡�б���곡�б�
	ListCtrlHelper::ClearListCtrl(m_list);

	//�ĵ����л�
	DocumentLockSwitch lock_switch;

	AcStringArray fields;
	FieldHelper::GetAllFields(_T("DrillSite"), fields);
	if(fields.isEmpty()) return;

	AcDbObjectIdArray objIds;
	RcuHelper::FindAllDrillSite(objIds);
	if(objIds.isEmpty()) return;

	ListCtrlHelper::InsertDatasToListCtrl(m_list, fields, objIds);

	//ѡ�е�1��,����LVN_ITEMCHANGED��Ϣ
	m_list.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_list.EnsureVisible(0, FALSE);
	m_list.SetFocus();
}
