#pragma once

#include "Resource.h"
#include "AcadDialog.h"
//#include "DockBarChildDlg.h"
/**
	CButtonSTʹ�ü��
	http://www.cnblogs.com/lidabo/archive/2012/12/17/2821122.html
*/

//ʯ�Ž�ú������Ի���
class RcuDesignDlg : public AcadDialog
{
	DECLARE_DYNAMIC(RcuDesignDlg)

public:
	RcuDesignDlg(CWnd* pParent = NULL, BOOL bModal = FALSE);
	virtual ~RcuDesignDlg();

// �Ի�������
	enum { IDD = IDD_RCU_DESIGN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//�곡�б�
	CListCtrl m_list;
	BOOL m_allPut;
	//������ť������Ϣ
	afx_msg void OnBnClickedExport();

	//�곡�б�˫����Ϣ
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	//�곡�б��Ҽ���Ϣ
	afx_msg void OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult);

	//�곡�б��Ҽ��˵�����Ϣ��Ӧ
	afx_msg void OnAddDrillSiteCommand();
	afx_msg void OnDeleteDrillSiteCommand();
	afx_msg void OnModifyDrillSiteCommand();
	afx_msg void OnHilightDrillSiteCommand();
	afx_msg void OnUpdateDrillSiteListCommand();
	afx_msg void OnHilightCoalSurfCommand();
	afx_msg void OnDisplayCoalSurfCommand();
	afx_msg void OnDesiginClosePoresCommand();

	//��ӦMyMsg.h���Զ�����Ϣ
	afx_msg LRESULT OnRcuAddMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRcuDelMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRcuModifyMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRcuUpdateMessage(WPARAM wParam, LPARAM lParam);

	//�Ի����麯������
	virtual BOOL OnInitDialog();
	virtual void OnClosing();

	//�������Щ�������ṩ���Զ�����Ϣӳ�亯��ʹ��
private:
	//�����û�ָ���Ĳ��������곡�б�
	void updateDrillSiteListCtrl(unsigned int op, const AcDbObjectId& drill_site);
	
	//�����곡
	void addDrillSite(const AcDbObjectId& drill_site);
	//ɾ���곡
	void delDrillSite(int row1);
	//�޸��곡
	void modifyDrillSite(int row1);

	//���¶Ի���
	void updateUI();
};
