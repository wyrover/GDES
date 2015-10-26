#include "stdafx.h"
#include "DataListDlg.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"
#include "../GraphTool/GraphAlgo.h"

struct ItemData
{
    int iItem;            // �ڼ��е�����
	AcDbObjectIdArray path; // ·��
};

// ����CtrlList��������
struct SortData
{
    int iSubItem;
    CListCtrl* pList;
};

static int nSortOrder = 1;      // ����ķ�ʽ,1����-1����

int CALLBACK SortFunc( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
    ItemData* pData1 = ( ItemData* )lParam1;
    ItemData* pData2 = ( ItemData* )lParam2;
    SortData* pSD = ( SortData* )lParamSort;

    // ������򵥵İ취�������Ǿ�������һ��������
    // �����ַ������򼴿�

    CString s1 = pSD->pList->GetItemText( pData1->iItem, pSD->iSubItem );
    CString s2 = pSD->pList->GetItemText( pData2->iItem, pSD->iSubItem );

    int nRetVal = 0;

    if( pSD->iSubItem == 0 )
    {
        // ��1��(���)
        int n1 = _ttoi( s1 );
        int n2 = _ttoi( s2 );
        nRetVal = ( ( n1 < n2 ) ? -1 : 1 );
    }
    else
    {
        nRetVal = s1.CompareNoCase( s2 );
    }

    return nRetVal * nSortOrder;
}

static void AdjustItemDataNum( CListCtrl& m_list )
{
    // ��Ϊ������ܵ���iItem��ʵ�ʵ���λ�ò�һ�£���Ҫ����
    int n = m_list.GetItemCount();
    for( int i = 0; i < n; i++ )
    {
        ItemData* pData = ( ItemData* )m_list.GetItemData( i );
        pData->iItem = i;
    }
}

static void GetAllRegTypes( AcStringArray& types )
{
    DrawHelper::GetAllRegGETypesForDraw( types );
}

static void ClearList( CListCtrl& m_list )
{
    //����ɾ��������
    m_list.DeleteAllItems();
    //�õ�����
    int n = m_list.GetHeaderCtrl()->GetItemCount();
    //ɾ��������
    for( int i = 0; i < n; i++ )
    {
        m_list.DeleteColumn( 0 );
    }
}

static void BuildHeadColumns( CListCtrl& m_list, const AcStringArray fields )
{
    int len = fields.length();

    CRect rect;
    m_list.GetClientRect( &rect );

    int n = len;
    if( n > 10 ) n = 10;
    double dw = rect.Width() / ( n + 1 );

    m_list.InsertColumn( 0, _T( "���" ), LVCFMT_LEFT, dw );
    for( int i = 0; i < len; i++ )
    {
        m_list.InsertColumn( i + 1, fields[i].kACharPtr(), LVCFMT_LEFT, dw );
    }
}

//����·������Ħ������
static double CaclPathF1(const AcDbObjectIdArray& path)
{
	double sumFrinction = 0;
	for( int i = 0; i < path.length(); i++ )
	{
		CString friction;
		DataHelper::GetPropertyData(path[i],_T("��·��Ħ������"),friction);
		sumFrinction += _tstof(friction);
		//acutPrintf(_T("\n��·[%d]:%.2lf"),i+1,sumFrinction);
	}
	return sumFrinction;
}

//����·�����ֲܾ�����
static double CaclPathF2(const AcDbObjectIdArray& path)
{
	double sumPartFrinction = 0;
	for( int i = 0; i < path.length(); i++ )
	{
		CString friction;
		DataHelper::GetPropertyData(path[i],_T("��·�ľֲ�����"),friction);
		sumPartFrinction += _tstof(friction);
		//acutPrintf(_T("\n��·[%d]:%.2lf"),i+1,sumPartFrinction);
	}
	return sumPartFrinction;
}

static void FillDatas( CListCtrl& m_list, const AcDbObjectId& sObjId, const AcDbObjectId& tObjId)
{
	AcDbIntArray pathNums;
	AcDbObjectIdArray allPaths;
	GraphAlgo::FindGasTubePaths(sObjId, tObjId, pathNums, allPaths);

	int len = pathNums.length();
	int start = 0;
	for( int i = 0; i < len; i++ )
	{
		//acutPrintf(_T("\n�����%d������"), i+1);
		m_list.InsertItem( i, _T( "xx" ) );

		//���ñ��(��1��)
		CString num;
		num.Format( _T( "%d" ), i + 1 );
		m_list.SetItemText( i, 0, num );

		//����·��(��2��)
		AcDbObjectIdArray path;
		CString pathStr;
		int end = start+pathNums[i];
		for(int j=start; j<end;j++)
		{
			//��¼·��
			path.append(allPaths[j]);

			//��Ҫ����·��������һ���ֶ�
			AcDbObjectId objId = allPaths[j];
			CString name;
			DataHelper::GetPropertyData(objId, _T("��·���"), name);
			if(name.IsEmpty())
			{
				name = _T("NULL");
			}
			pathStr.Append(name);
			if(j < end-1)
			{
				pathStr.Append(_T("->"));
			}
		}
		start = end;
		//���õ�2�е�·��
		m_list.SetItemText( i, 1, pathStr );

		//����·������Ħ������
		double F1 = CaclPathF1(path);
		//����·�����ֲܾ�����
		double F2 = CaclPathF2(path);

		//������Ħ������(��3��)
		CString value;
		value.Format( _T( "%.3f" ), F1 );
		m_list.SetItemText( i, 2, value );

		//������Ħ������(��4��)
		value.Format( _T( "%.3f" ), F2 );
		m_list.SetItemText( i, 3, value );

		//���õ�i�е�itemdata
		ItemData* pData = new ItemData();
		pData->iItem = i;
		//ItemDataҲ��Ҫ��¼·��,�������ʹ��
		pData->path.append(path);
		m_list.SetItemData( i, ( LPARAM )pData ); // ��������
	}
}

static void ClearItemData( CListCtrl& m_list )
{
    int n = m_list.GetItemCount();
    for( int i = 0; i < n; i++ )
    {
        ItemData* pData = ( ItemData* )m_list.GetItemData( i );
        delete pData;
    }
}

static void ZoomToEntity( const AcDbObjectId& objId )
{
    // �ڷ�ģ̬�Ի������޷�ʹ��
    //ads_name en;
    //if(Acad::eOk != acdbGetAdsName(en, objId)) return;
    //acedCommand(RTSTR, _T("ZOOM"), RTSTR, _T("O"), RTENAME, en, RTSTR, _T(""), 0);

    // ��ʱʹ��sendStringToExecute������Ŷ�λ����
    CString cmd;
    cmd.Format( _T( "ZOOM O \003" ) ); // ���ո����ѡ�����Ȼ��esc(��ֹ����Ŀո��ظ�ִ������)
    acDocManager->sendStringToExecute( curDoc(), cmd, true, false, false );
}

IMPLEMENT_DYNAMIC( DataListDlg, AcadDialog )


DataListDlg::DataListDlg( CWnd* pParent, BOOL bModal )
	: AcadDialog( DataListDlg::IDD, pParent, bModal )
{

}
DataListDlg::~DataListDlg()
{
}

void DataListDlg::DoDataExchange( CDataExchange* pDX )
{
    AcadDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_DATA_LIST, m_geList );
}

BEGIN_MESSAGE_MAP( DataListDlg, AcadDialog )
    //{{AFX_MSG_MAP(DataListDlg)
    //}}AFX_MSG_MAP

    ON_BN_CLICKED( IDC_DATA_LIST_UPDATE_BTN, &DataListDlg::OnBnClickedUpdateBtn )
    //ON_BN_CLICKED(IDC_GE_LIST_READ_TYPE_BTN, &DataListDlg::OnBnClickedGeListReadTypeBtn)
    ON_NOTIFY( LVN_ITEMCHANGED, IDC_DATA_LIST, &DataListDlg::OnLvnItemchangedGeList )
    ON_NOTIFY( LVN_COLUMNCLICK, IDC_DATA_LIST, &DataListDlg::OnLvnColumnclickGeList )
	ON_BN_CLICKED(IDC_EXIT_BTN, &DataListDlg::OnBnClickedExitBtn)
END_MESSAGE_MAP()

BOOL DataListDlg::OnInitDialog()
{
    AcadDialog::OnInitDialog();

    m_geList.SetExtendedStyle( m_geList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	UpdateData();
    return TRUE;
}


//void DataListDlg::OnBnClickedGeListReadTypeBtn()
//{
//	m_types.removeAll();
//	m_typeList.ResetContent();
//
//	FillTypeList(m_types, m_typeList);
//}

static void GetGasTunnelFields(AcStringArray& fields)
{
	fields.append(_T("·��"));
	fields.append(_T("��Ħ������(Pa)"));
	fields.append(_T("�ֲܾ�����(Pa)"));
}

void DataListDlg::UpdateData()
{
	// ɾ�����е�����(item data)
	ClearItemData( m_geList );

	// �������
	ClearList( m_geList );

	//CString type = _T("��˹��ɹ�·");

	acDocManager->lockDocument( curDoc() );

	AcStringArray fields;
	//������˹��··����column����
	GetGasTunnelFields(fields);
	// ��ȡ���е��ֶ�
	//FieldHelper::GetAllFields( type, fields );

	// ����Column
	BuildHeadColumns( m_geList, fields );

	m_initFinished = false;

	// �������
	FillDatas(m_geList, sObjId, tObjId);

	// ���ݳ�ʼ�����
	m_initFinished = true;

	acDocManager->unlockDocument( curDoc() );
}

void DataListDlg::OnBnClickedUpdateBtn()
{
	UpdateData();
}

//#include "../MineGE/LinkedGE.h"
//#include "../ArxHelper/HelperClass.h"
void DataListDlg::OnLvnItemchangedGeList( NMHDR* pNMHDR, LRESULT* pResult )
{
    if( !m_initFinished ) return;

    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );

    int row = pNMLV->iItem;
    if( row != LB_ERR )
    {
        ItemData* pData = ( ItemData* )m_geList.GetItemData( row );
        //acutPrintf(_T("\nid:%d"), pData->objId);

        acDocManager->lockDocument( curDoc() );

		//���Ų�ѡ������·��
		if(ArxEntityHelper::SelectEntities(pData->path))
		{
			ArxEntityHelper::ZoomToEntities(pData->path);
		}

        acDocManager->unlockDocument( curDoc() );
    }

    *pResult = 0;
}

//������Ϣ��Ӧ
void DataListDlg::OnLvnColumnclickGeList( NMHDR* pNMHDR, LRESULT* pResult )
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );

    static int ncurSortCol = -1;      // ���浱ǰ��������

    /*
     * һ��ʼ��ʾΪ-1����ʾ��δ���κ�������
     * ����ǰ��������������ͬ��������������������ʽ��
     * ��������뵱ǰ�����в�ͬ���У���ı������򣬲�������ʽ��Ϊ����
     */
    if( ncurSortCol == pNMLV->iSubItem )
    {
        nSortOrder *= -1;
    }
    else
    {
        ncurSortCol = pNMLV->iSubItem;
        nSortOrder = 1;
    }
    AdjustItemDataNum( m_geList );

    SortData sd = {pNMLV->iSubItem, &m_geList};
    m_geList.SortItems( SortFunc, ( LPARAM )&sd ); // ����

    *pResult = 0;
}

void DataListDlg::OnBnClickedExitBtn()
{
	AcadDialog::OnCancel();
}
