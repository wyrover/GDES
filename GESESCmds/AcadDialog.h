#pragma once

class AcadDialog : public CDialog
{
	DECLARE_DYNAMIC(AcadDialog)

public:
	AcadDialog(UINT nIDTemplate, CWnd* pParent = NULL, BOOL bModal = FALSE);
	virtual ~AcadDialog();
	//���жԻ���
	//ע:��ģ̬�Ի���ķ���ֵû������,���Ժ���
	INT_PTR Run();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	//��ʼ���Ի���
public:
	virtual BOOL OnInitDialog();
	//ģ�¶Ի����OK��Cancel��ť����
protected:
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();

private:
	//����Ƿ��ڴ�����
	BOOL m_mouseInWindow;
	//�Ƿ���Ҫ׷������ƶ���Ϣ
	BOOL m_bTracking;
	//�Ƿ�ģ̬�Ի���(Ĭ�Ϸ�ģ̬FALSE)
	BOOL m_bModal;
	//�Ի���ID
	UINT m_nIDTemplate;

public:
	//CAD������Ϣ����
	afx_msg LRESULT OnAcadKeepFocus(WPARAM wParam, LPARAM lParam);
	//���ڹر���Ϣ
	afx_msg void OnClose();
	//����ƶ���Ϣ(�����ж�����Ƿ��ڴ�����)
	//�ο�:http://www.cnblogs.com/greatverve/archive/2013/02/06/TRACKMOUSEEVENT.html
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};
