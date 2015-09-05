#include "stdafx.h"
#include "EditPumpDBDlg.h"
#include "PathBuild.h"


IMPLEMENT_DYNAMIC(EditPumpDBDlg, CDialog)

EditPumpDBDlg::EditPumpDBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(EditPumpDBDlg::IDD, pParent)
	, m_btype(FALSE)
	, m_bweight(FALSE)
	, m_bspeed(FALSE)
	, m_bmaxp(FALSE)
	, m_bfactory(FALSE)
	, m_blength(FALSE)
	, m_bheight(FALSE)
	, m_bweidth(FALSE)
	, m_bminabsp(FALSE)
	, m_babsp(FALSE)
	, m_bpower(FALSE)
	, m_bmaxq(FALSE)
	, m_ball(TRUE)
	, m_type(_T(""))
	, m_weight(_T(""))
	, m_speed(_T(""))
	, m_maxp(_T(""))
	, m_factory(_T(""))
	, m_length(_T(""))
	, m_height(_T(""))
	, m_weidth(_T(""))
	, m_absp(_T(""))
	, m_power(_T(""))
	, m_maxq(_T(""))
	, m_minabsp(_T(""))
	, m_itemsNum(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_GASSYS_ICON);
}

EditPumpDBDlg::~EditPumpDBDlg()
{
}

static BOOL IsNum(CString &str)
{
	int n=str.GetLength();
	for(int i=0;i<n;i++)
		if ((str[i]<'0'||str[i]>'9') && str[i] != '.') 
			return FALSE;
	return TRUE;
}

static BOOL IsInt(CString &str)
{
	int n=str.GetLength();
	for(int i=0;i<n;i++)
		if (str[i]<'0'||str[i]>'9') 
			return FALSE;
	return TRUE;
}


// �����õıȽϺ���

static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CString &lp1 = *((CString *)lParam1);
	CString &lp2 = *((CString *)lParam2);
	int &sort = *(int *)lParamSort;
	if (sort == 0)
	{
		if(!IsNum(lp1) && !IsNum(lp2) && !IsInt(lp1) && !IsInt(lp2)) return lp1.CompareNoCase(lp2);
		if(_tstof(lp1) < _tstof(lp2)) return -1;
		if(_tstof(lp1) > _tstof(lp2)) return 1;
		else return 0;
		//return lp1.CompareNoCase(lp2);
	}
	else
	{
		if(!IsNum(lp1) && !IsNum(lp2) && !IsInt(lp1) && !IsInt(lp2)) return lp2.CompareNoCase(lp1);
		if(_tstof(lp2) < _tstof(lp1)) return -1;
		if(_tstof(lp2) > _tstof(lp1)) return 1;
		else return 0;
	}
}

void EditPumpDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_PUMP_NAME_CHECK, m_btype);
	DDX_Check(pDX, IDC_PUMP_WEIGHT_CHECK, m_bweight);
	DDX_Check(pDX, IDC_PUMP_SPEED_CHECK, m_bspeed);
	DDX_Check(pDX, IDC_PUMP_MAXP_CHECK, m_bmaxp);
	DDX_Check(pDX, IDC_PUMP_FACTORY_CHECK, m_bfactory);
	DDX_Check(pDX, IDC_PUMP_LENGTH_CHECK, m_blength);
	DDX_Check(pDX, IDC_PUMP_HEIGHT_CHECK, m_bheight);
	DDX_Check(pDX, IDC_PUMP_WEIDTH_CHECK, m_bweidth);
	DDX_Check(pDX, IDC_PUMP_MINABSP_CHECK, m_bminabsp);
	DDX_Check(pDX, IDC_PUMP_ABSP_CHECK, m_babsp);
	DDX_Check(pDX, IDC_PUMP_POWER_CHECK, m_bpower);
	DDX_Check(pDX, IDC_PUMP_MAXQ_CHECK, m_bmaxq);
	DDX_Check(pDX, IDC_ALL_PUMP_CHECK, m_ball);
	DDX_Text(pDX, IDC_FIND_PUMP_NAME_EDIT, m_type);
	DDX_Text(pDX, IDC_FIND_PUMP_WEIGHT_EDIT, m_weight);
	DDX_Text(pDX, IDC_FIND_PUMP_SPEED_EDIT, m_speed);
	DDX_Text(pDX, IDC_FIND_PUMP_MAXP_EDIT, m_maxp);
	DDX_Text(pDX, IDC_FIND_PUMP_FACTORY_EDIT, m_factory);
	DDX_Text(pDX, IDC_FIND_PUMP_LENGTH_EDIT, m_length);
	DDX_Text(pDX, IDC_FIND_PUMP_HEIGHT_EDIT, m_height);
	DDX_Text(pDX, IDC_FIND_PUMP_WEIDTH_EDIT, m_weidth);
	DDX_Text(pDX, IDC_FIND_PUMP_ABSP_EDIT, m_absp);
	DDX_Text(pDX, IDC_FIND_PUMP_POWER_EDIT, m_power);
	DDX_Text(pDX, IDC_FIND_PUMP_MAXQ_EDIT, m_maxq);
	DDX_Text(pDX, IDC_FIND_PUMP_MINABSP_EDIT, m_minabsp);
	DDX_Text(pDX, IDC_PUMP_ITEMSNUM_EDIT, m_itemsNum);
	DDX_Control(pDX, IDC_FIND_PUMP_RET_LIST, m_listCtrl);
	DDX_Control(pDX, IDC_PUMP_POWER_CHECK, m_powerBtn);
	DDX_Control(pDX, IDC_PUMP_ABSP_CHECK, m_abspBtn);
	DDX_Control(pDX, IDC_PUMP_MAXQ_CHECK, m_maxqBtn);
	DDX_Control(pDX, IDC_PUMP_NAME_CHECK, m_typeBtn);
	DDX_Control(pDX, IDC_PUMP_WEIGHT_CHECK, m_weightBtn);
	DDX_Control(pDX, IDC_PUMP_SPEED_CHECK, m_speedBtn);
	DDX_Control(pDX, IDC_PUMP_FACTORY_CHECK, m_factoryBtn);
	DDX_Control(pDX, IDC_PUMP_LENGTH_CHECK, m_lengthBtn);
	DDX_Control(pDX, IDC_PUMP_HEIGHT_CHECK, m_heightBtn);
	DDX_Control(pDX, IDC_PUMP_WEIDTH_CHECK, m_weidthBtn);
	DDX_Control(pDX, IDC_PUMP_MINABSP_CHECK, m_minabspBtn);
	DDX_Control(pDX, IDC_PUMP_MAXP_CHECK, m_maxpBtn);
	DDX_Control(pDX, IDC_ALL_PUMP_CHECK, m_allBtn);
}


BEGIN_MESSAGE_MAP(EditPumpDBDlg, CDialog)
	ON_BN_CLICKED(IDC_ALL_PUMP_CHECK, &EditPumpDBDlg::OnBnClickedAllPumpCheck)
	ON_BN_CLICKED(IDC_PUMP_NAME_CHECK, &EditPumpDBDlg::OnBnClickedPumpNameCheck)
	ON_BN_CLICKED(IDC_PUMP_LENGTH_CHECK, &EditPumpDBDlg::OnBnClickedPumpLengthCheck)
	ON_BN_CLICKED(IDC_PUMP_ABSP_CHECK, &EditPumpDBDlg::OnBnClickedPumpAbspCheck)
	ON_BN_CLICKED(IDC_PUMP_WEIGHT_CHECK, &EditPumpDBDlg::OnBnClickedPumpWeightCheck)
	ON_BN_CLICKED(IDC_PUMP_HEIGHT_CHECK, &EditPumpDBDlg::OnBnClickedPumpHeightCheck)
	ON_BN_CLICKED(IDC_PUMP_POWER_CHECK, &EditPumpDBDlg::OnBnClickedPumpPowerCheck)
	ON_BN_CLICKED(IDC_PUMP_SPEED_CHECK, &EditPumpDBDlg::OnBnClickedPumpSpeedCheck)
	ON_BN_CLICKED(IDC_PUMP_WEIDTH_CHECK, &EditPumpDBDlg::OnBnClickedPumpWeidthCheck)
	ON_BN_CLICKED(IDC_PUMP_MAXQ_CHECK, &EditPumpDBDlg::OnBnClickedPumpMaxqCheck)
	ON_BN_CLICKED(IDC_PUMP_MAXP_CHECK, &EditPumpDBDlg::OnBnClickedPumpMaxpCheck)
	ON_BN_CLICKED(IDC_PUMP_MINABSP_CHECK, &EditPumpDBDlg::OnBnClickedPumpMinabspCheck)
	ON_BN_CLICKED(IDC_PUMP_FACTORY_CHECK, &EditPumpDBDlg::OnBnClickedPumpFactoryCheck)
	ON_BN_CLICKED(IDC_EXIT_BUTTON, &EditPumpDBDlg::OnBnClickedExitButton)
	ON_BN_CLICKED(IDC_UPDATE_PUMPDB_BUTTON, &EditPumpDBDlg::OnBnClickedUpdatePumpdbButton)
	ON_BN_CLICKED(ID_FIND_PUMP, &EditPumpDBDlg::OnBnClickedFindPump)
	ON_NOTIFY(NM_RCLICK, IDC_FIND_PUMP_RET_LIST, &EditPumpDBDlg::OnNMRClickFindPumpRetList)
	ON_COMMAND(ID_DELETE_ITEM, &EditPumpDBDlg::OnDeleteItem)
	ON_NOTIFY(NM_DBLCLK, IDC_FIND_PUMP_RET_LIST, &EditPumpDBDlg::OnNMDblclkFindPumpRetList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_FIND_PUMP_RET_LIST, &EditPumpDBDlg::OnLvnColumnclickFindPumpRetList)
	ON_WM_CTLCOLOR() 
END_MESSAGE_MAP()

static void SetSpecialBtn(CButtonST& btn)
{
	btn.SetIcon(IDI_ON_ICON, IDI_OFF_ICON);
	btn.DrawTransparent();
	btn.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 128, 128));
	btn.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(240, 0, 0));
	btn.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(240, 0, 0));
}

static void SetNormalBtn(CButtonST& btn)
{
	btn.SetIcon(IDI_ON_ICON, IDI_OFF_ICON);
	btn.DrawTransparent();
	btn.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(100, 100, 100));
	btn.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(31, 31, 31));
	btn.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(31, 31, 31));
}

void EditPumpDBDlg::SetCheckBoxIcon()
{
	SetSpecialBtn(m_powerBtn);
	SetSpecialBtn(m_abspBtn);
	SetSpecialBtn(m_maxqBtn);
	
	SetNormalBtn( m_typeBtn );
	SetNormalBtn( m_weightBtn );
	SetNormalBtn( m_speedBtn );
	SetNormalBtn( m_factoryBtn );
	SetNormalBtn( m_lengthBtn );
	SetNormalBtn( m_heightBtn );
	SetNormalBtn( m_weidthBtn );
	SetNormalBtn( m_minabspBtn );
	SetNormalBtn( m_maxpBtn );
	SetNormalBtn( m_allBtn );
}

BOOL EditPumpDBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	OnInitListCtrl();
	SetCheckBoxIcon();
	UpdateData(FALSE);
	return TRUE;
}


void EditPumpDBDlg::OnBnClickedAllPumpCheck()
{
	UpdateData(TRUE);
	m_btype = m_bweight = m_bspeed = m_bmaxp
		= m_bfactory = m_blength =m_bheight
		= m_bweidth = m_bminabsp = m_babsp
		= m_bpower = m_bmaxq = !m_ball;
	UpdateData(FALSE);
}

void EditPumpDBDlg::SetAllCheckBox()
{
	UpdateData(TRUE);
	m_ball = !(m_btype || m_bweight || m_bspeed || m_bmaxp
				|| m_bfactory || m_blength || m_bheight
				|| m_bweidth || m_bminabsp || m_babsp
				|| m_bpower || m_bmaxq);
	UpdateData(FALSE);
}

void EditPumpDBDlg::OnBnClickedPumpNameCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpLengthCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpAbspCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpWeightCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpHeightCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpPowerCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpSpeedCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpWeidthCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpMaxqCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpMaxpCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpMinabspCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpFactoryCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnInitListCtrl()
{
	//����ߺ���ѡ��
	m_listCtrl.SetExtendedStyle(m_listCtrl.GetExtendedStyle() |
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	//��ͷ
	m_listCtrl.InsertColumn(1,_T("����"),LVCFMT_CENTER);  
	m_listCtrl.InsertColumn(2,_T("����"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(3,_T("�ó�"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(4,_T("�ÿ�"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(5,_T("�ø�"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(6,_T("ת��"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(7,_T("�����ѹ"),LVCFMT_CENTER);  
	m_listCtrl.InsertColumn(8,_T("����ѹ��"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(9,_T("�������"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(10,_T("�������"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(11,_T("��������ѹ"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(12,_T("��������"),LVCFMT_CENTER);

	CRect rect;  
	m_listCtrl.GetClientRect(rect); //��õ�ǰ�ͻ�����Ϣ  
	m_listCtrl.SetColumnWidth(0, 2.5*rect.Width() / 23); //�����еĿ�ȡ�  
	m_listCtrl.SetColumnWidth(1, 1.2*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(2, 1.2*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(3, 1.2*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(4, 1.2*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(5, 1.2*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(6, 1.5*rect.Width() / 23);   
	m_listCtrl.SetColumnWidth(7, 1.5*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(8, 1.5*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(9, 1.5*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(10,2.3*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(11, 6*rect.Width() / 23);  

	CFont *cFont = new CFont;  

	cFont->CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,_T("����")); 
	m_listCtrl.SetFont(cFont,true);//���������С
	//m_listCtrl.SetTextBkColor(RGB(185,193,181));//���ñ�����ɫ

	//m_listCtrl.SetTextColor(RGB(80,80,80));//����������ɫ
	//COLORREF color = RGB(185,193,181); 
	//m_listCtrl.SetBkColor(color); 
}
void EditPumpDBDlg::OnBnClickedExitButton()
{
	CDialog::OnCancel();
}

bool EditPumpDBDlg::EditsHasEmpty()
{
	UpdateData(TRUE);
	CString msg;
	if(m_type.IsEmpty())
	{
		msg = _T("�ͺ�Ϊ��!");
		AfxMessageBox(msg);
		return true;
	}

	if(m_weight.IsEmpty())
	{
		msg = _T("����Ϊ��!");
		AfxMessageBox(msg);
		return true;
	}

	if(m_speed.IsEmpty())
	{
		msg = _T("ת��Ϊ��!");
		AfxMessageBox(msg);
		return true;
	}

	if(m_maxp.IsEmpty())
	{
		msg = _T("����ѹ��Ϊ��!");
		AfxMessageBox(msg);
		return true;
	}

	if(m_factory.IsEmpty())
	{
		msg = _T("��������Ϊ��!");
		AfxMessageBox(msg);
		return true;
	}

	if(m_length.IsEmpty())
	{
		msg = _T("�ó�Ϊ��!");
		AfxMessageBox(msg);
		return true;
	}

	if(m_height.IsEmpty())
	{
		msg = _T("�ø�Ϊ��!");
		AfxMessageBox(msg);
		return true;
	}

	if(m_weidth.IsEmpty())
	{
		msg = _T("�ÿ�Ϊ��!");
		AfxMessageBox(msg);
		return true;
	}

	if(m_absp.IsEmpty())
	{
		msg = _T("�����ѹΪ��!");
		AfxMessageBox(msg);
		return true;
	}

	if(m_power.IsEmpty())
	{
		msg = _T("�������Ϊ��!");
		AfxMessageBox(msg);
		return true;
	}

	if(m_maxq.IsEmpty())
	{
		msg = _T("�������Ϊ��!");
		AfxMessageBox(msg);
		return true;
	}

	if(m_minabsp.IsEmpty())
	{
		msg = _T("��������ѹΪ��!");
		AfxMessageBox(msg);
		return true;
	}

	if(!IsInt(m_weight))
	{
		msg = _T("���طǷ�[����������]!");
		AfxMessageBox(msg);
		return true;
	}

	if(!IsInt(m_speed))
	{
		msg = _T("ת�ٷǷ�[����������]!");
		AfxMessageBox(msg);
		return true;
	}

	if(!IsInt(m_maxp))
	{
		msg = _T("����ѹ���Ƿ�[����������]!");
		AfxMessageBox(msg);
		return true;
	}

	if(!IsInt(m_length))
	{
		msg = _T("�ó��Ƿ�[����������]!");
		AfxMessageBox(msg);
		return true;
	}

	if(!IsInt(m_height))
	{
		msg = _T("�ø߷Ƿ�[����������]!");
		AfxMessageBox(msg);
		return true;
	}

	if(!IsInt(m_weidth))
	{
		msg = _T("�ÿ�Ƿ�[����������]!");
		AfxMessageBox(msg);
		return true;
	}

	if(!IsInt(m_absp))
	{
		msg = _T("�����ѹ�Ƿ�[����������]!");
		AfxMessageBox(msg);
		return true;
	}

	if(!IsNum(m_power))
	{
		msg = _T("������ʷǷ�[��������������С��]!");
		AfxMessageBox(msg);
		return true;
	}

	if(!IsNum(m_maxq))
	{
		msg = _T("��������Ƿ�[��������������С��]!");
		AfxMessageBox(msg);
		return true;
	}

	if(!IsInt(m_minabsp))
	{
		msg = _T("��������ѹ�Ƿ�[����������]!");
		AfxMessageBox(msg);
		return true;
	}

	return false;
}

bool EditPumpDBDlg::GetEditContents(TypeTable& tt,PropertyTable& pt)
{
	if(EditsHasEmpty()) return false;

	tt.absP = _ttoi(m_minabsp);
	tt.factName = m_factory;
	tt.heigth = _ttoi(m_height);
	tt.length = _ttoi(m_length);
	tt.type = m_type;
	tt.weidth = _ttoi(m_weidth);
	tt.weight = _ttoi(m_weight);

	pt.speed = _ttoi(m_speed);
	pt.power = _tstof(m_power);
	pt.absP = _ttoi(m_absp);
	pt.maxP = _ttoi(m_maxp);
	pt.maxQ = _tstof(m_maxq);
	return true;

}

void EditPumpDBDlg::OnBnClickedUpdatePumpdbButton()
{
	CString dataDirName = _T( "Datas\\" );
	CString szDbPath = BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("pump.db") );

	TypeTable tt;
	PropertyTable pt;
	if(!GetEditContents(tt,pt)) return;
	int newId;
	InsertPumpToTypeTable(szDbPath,tt,newId);
	pt.id = newId;

	if(!InsertPumpToPropertyTable(szDbPath,pt))
	{
		AfxMessageBox(_T("���ݲ���ʧ��!"),MB_OK|MB_ICONSTOP);
		return;
	}
	AfxMessageBox(_T("���ݲ���ɹ�!"));
	//�����б�ؼ�
	m_btype = m_bweight = m_bspeed = m_bmaxp
		= m_bfactory = m_blength =m_bheight
		= m_bweidth = m_bminabsp = m_babsp
		= m_bpower = m_bmaxq = TRUE;
	m_ball = FALSE;
	UpdateData(FALSE);
	DBDatasVector datasV;
	if(!FindPumpsByCondition(datasV)) datasV.clear();
	UpdateList(datasV);
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

static void GetTypeTableDatas(const CString& sql,TypeTableVector& ttV)
{
	CString dataDirName = _T( "Datas\\" );
	CString szDbPath = BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("pump.db") );
	GetPumpTypeTable(sql,szDbPath,ttV);
}

static void GetPropertyTableDatas(const CString& sql,PropertyTableVector& ptV)
{
	CString dataDirName = _T( "Datas\\" );
	CString szDbPath = BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("pump.db") );
	GetPumpPropertyTable(sql,szDbPath,ptV);
}

static void FindTypeFirst(const CString& ttsql,DBDatasVector& datasV)
{
	TypeTableVector ttV;
	GetTypeTableDatas(ttsql,ttV);
	for(int i = 0; i < ttV.size(); i++)
	{
		TypeTable tt = ttV[i];
		DBDatas datas;
		datas.CopyFromType(tt);
		CString strID;
		strID.Format(_T("%d"),tt.id);
		CString ptsql = _T("select * from PropertyTable where catagory_id = ") + strID;
		PropertyTableVector ptV;
		GetPropertyTableDatas(ptsql,ptV);
		for(int j = 0; j < ptV.size(); j++)
		{
			datas.CopyFromProperty(ptV[j]);
			datasV.push_back(datas);
		}
	}
}

static void FindPropertyFirst(const CString& ptsql,DBDatasVector& datasV)
{
	PropertyTableVector ptV;
	GetPropertyTableDatas(ptsql,ptV);
	for(int i = 0; i < ptV.size(); i++)
	{
		PropertyTable pt = ptV[i];
		DBDatas datas;
		datas.CopyFromProperty(pt);
		CString strID;
		strID.Format(_T("%d"),pt.id);
		CString ttsql = _T("select * from TypeTable where catagory_id = ") + strID;
		TypeTableVector ttV;
		GetTypeTableDatas(ttsql,ttV);
		for(int j = 0; j < ttV.size(); j++)
		{
			datas.CopyFromType(ttV[j]);
			datasV.push_back(datas);
		}
	}
}

static void FindTypeProperty(const CString& ttsql,const CString& ptsql,DBDatasVector& datasV)
{
	PropertyTableVector ptV;
	GetPropertyTableDatas(ptsql,ptV);
	TypeTableVector ttV;
	GetTypeTableDatas(ttsql,ttV);
	for(int i = 0; i < ttV.size(); i++)
	{
		DBDatas datas;
		datas.CopyFromType(ttV[i]);
		for(int j = 0; j < ptV.size(); j++)
		{
			PropertyTable pt = ptV[j];
			if(pt.id != ttV[i].id) continue;
			datas.CopyFromProperty(pt);
			datasV.push_back(datas);
		}
	}
}

static void FindAllPumps(DBDatasVector& datasV)
{
	CString ttsql;
	ttsql = _T("select * from TypeTable");
	FindTypeFirst(ttsql,datasV);
}


void EditPumpDBDlg::setCountPumps()
{
	int itemNum = m_listCtrl.GetItemCount();
	CString itemNumStr;
	BOOL show = FALSE;
	itemNumStr.Format(_T("%d"),itemNum);
	if( 0 >= itemNum ) 
	{
		itemNumStr = _T("");
		show = FALSE;
	}
	else
	{
		show = TRUE;
	}
	//GetDlgItem(IDC_PUMP_ITEMSNUM_EDIT)->SetWindowText(itemNumStr);
	m_itemsNum = itemNumStr;
	GetDlgItem(IDC_NUM_STATIC)->ShowWindow(show);
	GetDlgItem(IDC_GROUP_STATIC)->ShowWindow(show);
	UpdateData(FALSE);
}

void EditPumpDBDlg::UpdateList( const DBDatasVector& datasV )
{
	m_listCtrl.DeleteAllItems();

	//����ˢ�����ݵ�ʱ����˸
	m_listCtrl.SetRedraw(FALSE);
	for (int i = 0; i < datasV.size(); i++ )
	{
		DBDatas datas = datasV[i];
		m_listCtrl.InsertItem(i, datas.type);  
		m_listCtrl.SetItemText(i, 1, datas.weight);  
		m_listCtrl.SetItemText(i, 2, datas.length);  
		m_listCtrl.SetItemText(i, 3, datas.weidth);  
		m_listCtrl.SetItemText(i, 4, datas.height);
		m_listCtrl.SetItemText(i, 5, datas.speed);  
		m_listCtrl.SetItemText(i, 6, datas.absp);  
		m_listCtrl.SetItemText(i, 7, datas.maxp);  
		m_listCtrl.SetItemText(i, 8, datas.maxq);  
		m_listCtrl.SetItemText(i, 9, datas.power);  
		m_listCtrl.SetItemText(i, 10, datas.minabsp);  
		m_listCtrl.SetItemText(i, 11, datas.factory);  
	}

	m_listCtrl.SetRedraw(TRUE);
	m_listCtrl.Invalidate();
	m_listCtrl.UpdateWindow();
	setCountPumps();
}
void EditPumpDBDlg::OnBnClickedFindPump()
{
	UpdateData(TRUE);
	DBDatasVector datasV;
	//�鿴������˹��
	if(m_ball)
	{
		FindAllPumps(datasV);
	}
	//for (int i = 0; i < datasV.size(); i++)
	//{
	//	DBDatas datas = datasV[i];
	//	datas.Print();
	//}
	else
	{
		if(!FindPumpsByCondition(datasV)) datasV.clear();
	}
	UpdateList(datasV);
}

int EditPumpDBDlg::CheckBoxTable()
{
	UpdateData(TRUE);
	if((m_btype || m_bminabsp || m_bweight || m_blength || m_bweidth || m_bheight || m_bfactory)
		&& (!m_bspeed && !m_bpower && !m_bmaxq && !m_bmaxp && !m_babsp)) return 1;
	if((!m_btype && !m_bminabsp && !m_bweight && !m_blength && !m_bweidth && !m_bheight && !m_bfactory)
		&& (m_bspeed || m_bpower || m_bmaxq || m_bmaxp || m_babsp)) return 2;
	if((m_btype || m_bminabsp || m_bweight || m_blength || m_bweidth || m_bheight || m_bfactory)
		&& (m_bspeed || m_bpower || m_bmaxq || m_bmaxp || m_babsp)) return 3;

	return 0;
}

bool EditPumpDBDlg::FindPumpsByCondition( DBDatasVector& datasV )
{
	int indx = CheckBoxTable();
	CString msg;
	CString ttsql = _T("select * from TypeTable where ");
	CString ptsql = _T("select * from PropertyTable where ");
	switch(indx)
	{
	case 0:
		msg = _T("δ������ѯ����!");
		break;
	case 1:
		OnlyTypesql(ttsql,msg);
		FindTypeFirst(ttsql,datasV);
		break;
	case 2:
		OnlyPropertysql(ptsql,msg);
		FindPropertyFirst(ptsql,datasV);
		break;
	case 3:
		OnlyTypesql(ttsql,msg);
		OnlyPropertysql(ptsql,msg);
		FindTypeProperty(ttsql,ptsql,datasV);
		break;
	default:
		msg = _T("ѡ�����!");
		break;
	}
	if(!msg.IsEmpty()) 
	{
		AfxMessageBox(msg);
		return false;
	}
	//acutPrintf(_T("\n%s"),msg);
	return true;

}

static bool DeletePumpFromDB(CString& ttsql,CString& ptsql)
{
	CString dataDirName = _T( "Datas\\" );
	CString szDbPath = BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("pump.db") );
	if(!DeletePumpFronTable(szDbPath,ttsql)) return false;
	if(!DeletePumpFronTable(szDbPath,ptsql)) return false;
	return true;
}

bool EditPumpDBDlg::DeletePump( const DBDatas& datas )
{
	CString ttsql = _T("delete from TypeTable where type = \'") + datas.type + _T("\' and absP = ")
		+ datas.minabsp + _T(" and weight = ") + datas.weight + _T(" and length = ")
		+ datas.length + _T(" and weidth = ") + datas.weidth + _T(" and heigth = ") + datas.height
		+ _T(" and factoryName = \'") + datas.factory + _T("\'");
	CString ptsql = _T("delete from PropertyTable where speed = ") + datas.speed + _T(" and power = ")
		+ datas.power + _T(" and maxQ = ") + datas.maxq + _T(" and maxP = ") + datas.maxp + _T(" and absP = ")
		+ datas.absp;
	bool ret = DeletePumpFromDB(ttsql,ptsql);
	return ret;
}

static void DealStringType(CString& ttsql,CString& msg,CString& contents,const CString& condition)
{
	ttsql.Append(condition);
	if(contents.IsEmpty())
	{
		msg = _T("����Ϊ��!");
		return;
	}
	if(contents.Find(_T("<")) != -1 || contents.Find(_T(">")) != -1 || contents.Find(_T("=")) != -1) 
		msg = _T("�����а����Ƿ����š�>����<����=��");
	ttsql.Append(_T(" = \'"));
	ttsql.Append(contents);
	ttsql.Append(_T("\'"));
}

static void DealOtherType(CString& ttsql,CString& msg,CString& editString,const CString& condition)
{
	ttsql.Append(condition);
	if(editString.IsEmpty())
	{
		msg = _T("����Ϊ��!");
		return;
	}
	if(editString.Find(_T("<")) == -1 && editString.Find(_T(">")) == -1 && editString.Find(_T("=")) == -1)
		ttsql.Append(_T(" = "));
	ttsql.Append(editString);

}

static void DealProperty(CString& ptsql,CString& msg,CString& editString,const CString& condition)
{
	ptsql.Append(condition);
	if(editString.IsEmpty())
	{
		msg = _T("����Ϊ��!");
		return;
	}
	if(editString.Find(_T("<")) == -1 && editString.Find(_T(">")) == -1 && editString.Find(_T("=")) == -1)
		ptsql.Append(_T(" = "));
	ptsql.Append(editString);

}

void EditPumpDBDlg::OnlyTypesql(CString& ttsql,CString& msg)
{
	int checkNum = GetCheckBoxNum();
	int currentNum = 0;
	if(m_btype)
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and type ");
		}
		else
		{
			condition = _T(" type ");
		}
		DealStringType(ttsql,msg,m_type,condition);
		currentNum += 1;
	}

	if(m_bminabsp)
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and absP ");
		}
		else
		{
			condition = _T(" absP ");
		}

		DealOtherType(ttsql,msg,m_minabsp,condition);
		currentNum += 1;
	}

	if(m_bweight)
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and weight ");
		}
		else
		{
			condition = _T(" weight ");
		}

		DealOtherType(ttsql,msg,m_weight,condition);
		currentNum += 1;
	}

	if(m_blength)
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and length ");
		}
		else
		{
			condition = _T(" length ");
		}

		DealOtherType(ttsql,msg,m_length,condition);
		currentNum += 1;
	}

	if(m_bweidth)
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and weidth ");
		}
		else
		{
			condition = _T(" weidth ");
		}

		DealOtherType(ttsql,msg,m_weidth,condition);
		currentNum += 1;
	}

	if(m_bheight)
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and heigth ");
		}
		else
		{
			condition = _T(" heigth ");
		}

		DealOtherType(ttsql,msg,m_height,condition);
		currentNum += 1;
	}

	if(m_bfactory)
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and factoryName ");
		}
		else
		{
			condition = _T(" factoryName ");
		}

		DealStringType(ttsql,msg,m_factory,condition);
		currentNum += 1;
	}
}

void EditPumpDBDlg::OnlyPropertysql( CString& ptsql,CString& msg )
{
	int checkNum = GetCheckBoxNum();
	int currentNum = 0;
	if(m_bspeed)
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and speed ");
		}
		else
		{
			condition = _T(" speed ");
		}
		DealProperty(ptsql,msg,m_speed,condition);
		currentNum += 1;
	}

	if(m_bpower)
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and power ");
		}
		else
		{
			condition = _T(" power ");
		}

		DealProperty(ptsql,msg,m_power,condition);
		currentNum += 1;
	}

	if(m_bmaxq)
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and maxQ ");
		}
		else
		{
			condition = _T(" maxQ ");
		}

		DealProperty(ptsql,msg,m_maxq,condition);
		currentNum += 1;
	}

	if(m_bmaxp)
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and maxP ");
		}
		else
		{
			condition = _T(" maxP ");
		}

		DealProperty(ptsql,msg,m_maxp,condition);
		currentNum += 1;
	}

	if(m_babsp)
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and absP ");
		}
		else
		{
			condition = _T(" absP ");
		}

		DealProperty(ptsql,msg,m_absp,condition);
		currentNum += 1;
	}
}

int EditPumpDBDlg::GetCheckBoxNum()
{
	UpdateData(TRUE);
	int ret = 0;
	if(m_btype) ret += 1;
	if(m_bweight) ret += 1;
	if(m_bspeed) ret += 1;
	if(m_bmaxp) ret += 1;
	if(m_bfactory) ret += 1;
	if(m_blength) ret += 1;
	if(m_bheight) ret += 1;
	if(m_bweidth) ret += 1;
	if(m_bminabsp) ret += 1;
	if(m_babsp) ret += 1;
	if(m_bpower) ret += 1;
	if(m_bmaxq) ret += 1;
	return ret;
}
void EditPumpDBDlg::OnNMRClickFindPumpRetList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem == -1) return; 
	CPoint point;
	::GetCursorPos(&point);

	CMenu menu;
	menu.LoadMenu(IDR_LIST_DEL_MENU);
	CMenu* popup=menu.GetSubMenu(0);
	if(popup==NULL ) return;

	CBitmap m_bitmap1;//λͼ
	m_bitmap1.LoadBitmap(IDB_DELETE_BITMAP);//����IDΪIDB_BITMAP1��λͼ

	popup->SetMenuItemBitmaps(0,MF_BYPOSITION,&m_bitmap1,&m_bitmap1); 

	popup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this );

	*pResult = 0;
}

static void GetListDatas(const CListCtrl& listCtrl, DBDatas& datas,int index)
{
	datas.type = listCtrl.GetItemText(index,0);
	datas.weight = listCtrl.GetItemText(index,1);
	datas.length = listCtrl.GetItemText(index,2);
	datas.weidth = listCtrl.GetItemText(index,3);
	datas.height = listCtrl.GetItemText(index,4);
	datas.speed = listCtrl.GetItemText(index,5);
	datas.absp = listCtrl.GetItemText(index,6);
	datas.maxp = listCtrl.GetItemText(index,7);
	datas.maxq = listCtrl.GetItemText(index,8);
	datas.power = listCtrl.GetItemText(index,9);
	datas.minabsp = listCtrl.GetItemText(index,10);
	datas.factory = listCtrl.GetItemText(index,11);
}

void EditPumpDBDlg::OnDeleteItem()
{
	//ɾ����ѡ��
	int nIndex;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
	nIndex = m_listCtrl.GetNextSelectedItem(pos);  // ��ȡ�����m_nIndex��ֵΪ�û����б����ѡ�е��к�

	if(nIndex == -1) return;
	if(IDYES != AfxMessageBox(_T("�������������ɳ���ɾ����!�Ƿ�ɾ��?"),MB_YESNO)) return;
	DBDatas datas;
	GetListDatas(m_listCtrl,datas,nIndex); 
	if(!DeletePump(datas))
	{
		AfxMessageBox(_T("ɾ������ʧ��!"),MB_OK | MB_ICONSTOP);
		return;
	}
	m_listCtrl.DeleteItem(nIndex);
	UpdateData(TRUE);
	if(IsInt(m_itemsNum))
	{
		int num = _ttoi(m_itemsNum);
		num = num -1;
		m_itemsNum.Format(_T("%d"),num);
	}
	UpdateData(FALSE);
}

void EditPumpDBDlg::OnNMDblclkFindPumpRetList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nIdx=m_listCtrl.GetNextItem(-1,LVIS_SELECTED);//ѡ�е��е�����

	DBDatas datas;
	GetListDatas(m_listCtrl,datas,nIdx);
	m_type = datas.type;
	m_weight = datas.weight;
	m_length = datas.length;
	m_weidth = datas.weidth;
	m_height = datas.height;
	m_speed = datas.speed;
	m_absp = datas.absp;
	m_maxp = datas.maxp;
	m_maxq = datas.maxq;
	m_power = datas.power;
	m_minabsp = datas.minabsp;
	m_factory = datas.factory;
	setCountPumps();
	UpdateData(FALSE);
	*pResult = 0;
}

void EditPumpDBDlg::OnLvnColumnclickFindPumpRetList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int Length = m_listCtrl.GetItemCount();
	CArray<CString,CString> ItemData;
	ItemData.SetSize(Length);
	for (int i = 0; i < Length; i++)
	{
		ItemData[i] = m_listCtrl.GetItemText(i,pNMLV->iSubItem);
		m_listCtrl.SetItemData(i,(DWORD_PTR)&ItemData[i]);//��������ؼ���
	}
	static int sort = 0;
	static int SubItem = 0;
	if (SubItem != pNMLV->iSubItem)
	{
		sort = 0;
		SubItem = pNMLV->iSubItem;
	}
	else
	{
		if (sort == 0)
		{
			sort = 1;
		}
		else
		{
			sort = 0;
		}
	}
	m_listCtrl.SortItems(MyCompareProc,(DWORD_PTR)&sort);//����
	*pResult = 0;
}

HBRUSH EditPumpDBDlg::OnCtlColor( CDC *pDC,CWnd *pWnd,UINT nCtlColor )
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);  

	//if (pWnd->GetDlgCtrlID()==IDC_PUMP_ABSP_CHECK)
	//{
	//	//pDC->SetBkMode(TRANSPARENT); 
	//	pDC->SetTextColor(RGB(255, 0, 128));
	//	//pDC->SetTextColor(RGB(125,125,125)); 
	//	CFont font;
	//	font.CreatePointFont(1,_T("Consolas"),NULL);
	//	//GetDlgItem(IDC_PUMP_ABSP_CHECK)->SetFont(&font);
	//	pWnd->SetFont(&font);
	//	HBRUSH B = CreateSolidBrush(RGB(125,125,255)); 
	//	return (HBRUSH) B; 
	//	//pDC->SetTextColor(RGB(255,0,0));  //����������ɫ
	//	//pDC->SetBkMode(TRANSPARENT); //�������屳��Ϊ͸��
	//	//// TODO: Return a different brush if the default is not desired
	//	//return (HBRUSH)::GetStockObject(HOLLOW_BRUSH);  // ���ñ���ɫ
	//}

	return hbr;
}