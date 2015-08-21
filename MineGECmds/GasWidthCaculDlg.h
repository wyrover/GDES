#pragma once
#include "afxwin.h"
#include "Resource.h"
#include "ValueHelper.h"

//#include<iostream>
//#include<vector>  
//using namespace std;
// GasWidthCaculDlg �Ի���

enum coalQuality
{
	RICH_COAL		= 0,		//��ú
	CHARRED_COAL	= 1,		//��ú
	GAS_COAL		= 2,		//��ú
	LEAN_COAL		= 3,		//��ú
	BLIND_COAL		= 4,		//����ú
	LONGFLAME_COAL	= 5		//����ú
};

class GasWidthCaculDlg : public CDialog
{
	DECLARE_DYNAMIC(GasWidthCaculDlg)

public:
	GasWidthCaculDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GasWidthCaculDlg();

// �Ի�������
	enum { IDD = IDD_TENDENCY_TREND_WIDTH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	CComboBox m_coalQualComb;
	CString m_H1;
	CString m_H2;
	CString m_h1;
	CString m_h2;

	AcDbObjectId m_objId;

	afx_msg void OnBnClickedOk();
	void setMineGE( const AcDbObjectId& objId );

private:
	//double newTonDivided(int n,vector<double>&X,vector<double>&Y);
	//double getRetByNewTon(double x,vector<double>&X,vector<double> &Y);
	double getResult( double dTime );
	void WriteData();
	void ReadData();
};
