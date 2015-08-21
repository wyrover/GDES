// GasWidthCaculDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "GasWidthCaculDlg.h"
#include "../MineGE/DataHelper.h"
#include "../MineGE/DataListHelper.h"
// GasWidthCaculDlg �Ի���

IMPLEMENT_DYNAMIC(GasWidthCaculDlg, CDialog)

GasWidthCaculDlg::GasWidthCaculDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GasWidthCaculDlg::IDD, pParent)
	, m_H1(_T(""))
	, m_H2(_T(""))
	, m_h1(_T(""))
	, m_h2(_T(""))
{

}

GasWidthCaculDlg::~GasWidthCaculDlg()
{
}

void GasWidthCaculDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COALQUAL_COMBO, m_coalQualComb);
	DDX_Text(pDX, IDC_H1_EDIT, m_H1);
	DDX_Text(pDX, IDC_H2_EDIT, m_H2);
	DDX_Text(pDX, IDC_MINH1_EDIT, m_h1);
	DDX_Text(pDX, IDC_MINH2_EDIT, m_h2);
}


BEGIN_MESSAGE_MAP(GasWidthCaculDlg, CDialog)
	ON_BN_CLICKED(IDOK, &GasWidthCaculDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL GasWidthCaculDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_coalQualComb.AddString(_T("����ú"));
	m_coalQualComb.AddString(_T("��ú"));
	m_coalQualComb.AddString(_T("��ú"));
	m_coalQualComb.AddString(_T("��ú"));
	m_coalQualComb.AddString(_T("��ú"));
	m_coalQualComb.AddString(_T("����ú"));
	m_coalQualComb.SetCurSel(0);
	ReadData();
	//m_H1 = _T("25");
	//m_H2 = _T("25");
	//m_h1 = _T("25");
	//m_h2 = _T("25");

	UpdateData(FALSE);
	return TRUE;
}

// GasWidthCaculDlg ��Ϣ�������

void GasWidthCaculDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	double fH1 = _tstof(m_H1);
	double fH2 = _tstof(m_H2);
	double fh1 = _tstof(m_h1);
	double fh2 = _tstof(m_h2);
	
	if ( 0 > fH1 || 0 > fH2 || 0 > fh1 || 0 > fh2 )
	{
		AfxMessageBox(_T("�������ݴ���!"));
		return;
	}

	double H1 = getResult( fH1 );
	double H2 = getResult( fH2 );
	double h1 = getResult( fh1 );
	double h2 = getResult( fh2 );
	CString H1String,H2String,h1String,h2String;
	H1String.Format(_T("%f"),H1);
	H2String.Format(_T("%f"),H2);
	h1String.Format(_T("%f"),h1);
	h2String.Format(_T("%f"),h2);
	
	DataHelper::SetPropertyData(m_objId,_T("�����������˹Ԥ�ŵ�ֵ���1"),H1String);
	DataHelper::SetPropertyData(m_objId,_T("�����������˹Ԥ�ŵ�ֵ���2"),H2String);
	DataHelper::SetPropertyData(m_objId,_T("�����������˹Ԥ�ŵ�ֵ���1"),h1String);
	DataHelper::SetPropertyData(m_objId,_T("�����������˹Ԥ�ŵ�ֵ���2"),h2String);

	WriteData();
	//acutPrintf(_T("\n���:%f\n"),H1);
	OnOK();
}

//double GasWidthCaculDlg::getRetByNewTon( double x,vector<double>&X,vector<double> &Y )
//{
//	int i,j;
//	double result=0;  
//	for(i=0;i<X.size();i++)
//	{  
//		double temp=1;  
//		double f=newTonDivided(i,X,Y);  
//		for(j=0;j<i;j++)
//		{  
//			temp = temp*(x-X[j]);  
//		}  
//		result += f*temp;  
//	}  
//	return result;  
//}
//
//double GasWidthCaculDlg::newTonDivided( int n,vector<double>&X,vector<double>&Y )
//{
//	double f=0;  
//	double temp=0; 
//	int i,j;
//	for(i=0;i<n+1;i++)
//	{  
//		temp=Y[i];  
//		for(j=0;j<n+1;j++)  
//		{
//			if(j!=i) 
//				temp /= (X[i]-X[j]);
//		}
//		f += temp;  
//	}  
//	return f;  
//}

double GasWidthCaculDlg::getResult( double dTime )
{
	vector<double>X;  
	vector<double>Y;
	X.push_back(25);
	X.push_back(50);
	X.push_back(100);
	X.push_back(160);
	X.push_back(200);
	X.push_back(250);

	int indx = m_coalQualComb.GetCurSel();
	//acutPrintf(_T("\n��ǰ����:%d"),indx);
	double tmp;
	switch(indx)
	{
	case RICH_COAL:
	case GAS_COAL:
	case LONGFLAME_COAL:
		Y.push_back(11.5);
		Y.push_back(13.0);
		Y.push_back(16.0);
		Y.push_back(18.0);
		Y.push_back(19.7);
		Y.push_back(21.5);
		tmp = 23.0;
		break;
	case CHARRED_COAL:
	case LEAN_COAL:
		Y.push_back(9.0);
		Y.push_back(10.5);
		Y.push_back(12.4);
		Y.push_back(14.2);
		Y.push_back(15.4);
		Y.push_back(16.9);
		tmp = 18.0;
		break;
	case BLIND_COAL:
		Y.push_back(6.5);
		Y.push_back(7.4);
		Y.push_back(9.0);
		Y.push_back(10.5);
		Y.push_back(11.0);
		Y.push_back(12.0);
		tmp = 13.0;
		break;
	default:
		break;
	}
	double ret;
	if(dTime >= 300)
	{
		ret = tmp;
	}

	else if(dTime == 0)
	{
		ret = 0;
	}

	else
	{
		ret = ValueHelper::getRetByNewTon(dTime,X,Y);
	}
	return ret;
}

void GasWidthCaculDlg::setMineGE( const AcDbObjectId& objId )
{
	m_objId = objId;
}

void GasWidthCaculDlg::WriteData()
{
	AcDbObjectId objId;
	SingleDataObjectHelper::GetObjectId(_T("��˹Ԥ�ſ��"),objId);

	CString idxStr;
	idxStr.Format(_T("%d"),m_coalQualComb.GetCurSel());
	//acutPrintf(_T("\n��ǰ��ѡ���:%s->%d\n"),idxStr,m_coalQualComb.GetCurSel());
	DataHelper::SetPropertyData(objId,_T("ú��"),idxStr);
	DataHelper::SetPropertyData(objId,_T("������ú�ڱ�¶ʱ��"),m_H1);
	DataHelper::SetPropertyData(objId,_T("˳��������ú�ڱ�¶ʱ��"),m_H2);
	DataHelper::SetPropertyData(objId,_T("����˳��ú�ڱ�¶ʱ��"),m_h1);
	DataHelper::SetPropertyData(objId,_T("�ط�˳��ú�ڱ�¶ʱ��"),m_h2);

}

void GasWidthCaculDlg::ReadData()
{
	AcDbObjectId objId;
	SingleDataObjectHelper::GetObjectId(_T("��˹Ԥ�ſ��"),objId);

	CString idxStr = _T("0");
	DataHelper::GetPropertyData(objId,_T("ú��"),idxStr);
	if(idxStr.IsEmpty()) idxStr = _T("0");
	m_coalQualComb.SetCurSel(_ttoi(idxStr));
	DataHelper::GetPropertyData(objId,_T("������ú�ڱ�¶ʱ��"),m_H1);
	DataHelper::GetPropertyData(objId,_T("˳��������ú�ڱ�¶ʱ��"),m_H2);
	DataHelper::GetPropertyData(objId,_T("����˳��ú�ڱ�¶ʱ��"),m_h1);
	DataHelper::GetPropertyData(objId,_T("�ط�˳��ú�ڱ�¶ʱ��"),m_h2);
}