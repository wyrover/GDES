#pragma once

#include "RcuAcUiBaseDlg.h"

#include "RcuDataLink.h"

//ʯ�Ž�ú�༭�Ի���(��"�곡���"�Ҽ��˵��е��øöԻ���)
class RcuEditDrillSiteDlg : public RcuAcUiBaseDlg
{
	DECLARE_DYNAMIC(RcuEditDrillSiteDlg)

public:
	RcuEditDrillSiteDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~RcuEditDrillSiteDlg();

// �Ի�������
	enum { IDD = IDD_RCU_DRILL_SITE_EDIT_DLG };

public:
	//�곡ͼԪid
	AcDbObjectId m_drill_site;
	//��/д����
	void readFromDataLink( DrillSiteLink& ds_link, CoalSurfaceLink& cs_link);
	void writeToDataLink( DrillSiteLink& ds_link, CoalSurfaceLink& cs_link);

private:
	//�Ի������곡ͼԪ֮�佻������
	void exchangeDrillSiteData( DrillSiteLink& ds_link, bool save);
	//�Ի�����ú��ͼԪ֮�佻������
	void exchangeCoalSurfaceData(CoalSurfaceLink& cs_link, bool save);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_name;
	double m_x;
	double m_y;
	double m_z;
	double m_width;
	double m_height;
	double m_left;
	double m_right;
	double m_top;
	double m_bottom;
	double m_gas_radius;
	double m_thick;
	double m_angle;
	double m_dist;
	double m_pore_size;

	//"ȷ��"��ť��Ϣ
	afx_msg void OnBnClickedOk();

	//�麯������
	virtual BOOL OnInitDialog();
};
