#pragma once

#include "afxpropertygridctrl.h"
#include "afxwin.h"
#include "Resource.h"

#include "AcadDialog.h"

class BaseParamDockDlg : public AcadDialog
{
	DECLARE_DYNAMIC(BaseParamDockDlg)

public:
	BaseParamDockDlg(CWnd* pParent = NULL, BOOL bModal = FALSE);   // ��׼���캯��
	virtual ~BaseParamDockDlg();

	afx_msg LRESULT OnPropertyChanged(WPARAM,LPARAM);
	// �Ի�������
	enum { IDD = IDD_PROPERTY_GRID_DLG };

	void onClosing();
	void updateProperty(bool writeOrRead);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	//// ����ͼԪ
	//void setMineGE(const AcDbObjectId& objId);

	//// ����ֶ�
	//void addField(const CString& field);
	void readPropertyData();
	void writePropertyData();

private:
	CMFCPropertyGridCtrl m_propertyDataList;
	AcDbObjectId m_objId;          // ���ݶ���id
	//AcStringArray m_fields;        // Ҫ��ʾ���ֶ�

};
