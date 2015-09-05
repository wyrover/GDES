#pragma once

//#include "DockBarChildDlg.h"
#include "Resource.h"
#include "AcadDialog.h"

class DataListDlg : public AcadDialog
{
	DECLARE_DYNAMIC(DataListDlg)

public:
	DataListDlg(CWnd* pParent = NULL, BOOL bModal = FALSE);
	virtual ~DataListDlg();

// �Ի�������
	enum { IDD = IDD_DATA_LIST_DLG };

	//�ⲿ����Ҫ������ʼĩ�ڵ����ڹ�·��ID
	AcDbObjectId sObjId, tObjId;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CListCtrl m_geList;
	bool m_initFinished;

public:
	virtual BOOL OnInitDialog();
	//virtual void OnClosing();
	void UpdateData();
	afx_msg void OnBnClickedUpdateBtn();
	afx_msg void OnLvnItemchangedGeList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickGeList(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnBnClickedGeListReadTypeBtn();
	afx_msg void OnBnClickedDataListHilightSelectBtn();
	afx_msg void OnBnClickedExitBtn();
};
