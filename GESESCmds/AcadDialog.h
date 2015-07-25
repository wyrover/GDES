#pragma once

class AcadDialog : public CDialog
{
	DECLARE_DYNAMIC(AcadDialog)

public:
	AcadDialog(UINT nIDTemplate, CWnd* pParent = NULL, BOOL bModal = FALSE);
	virtual ~AcadDialog();
	//运行对话框
	//注:非模态对话框的返回值没有意义,可以忽略
	INT_PTR Run();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	//初始化对话框
public:
	virtual BOOL OnInitDialog();
	//模仿对话框的OK和Cancel按钮处理
protected:
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();

private:
	//鼠标是否在窗口内
	BOOL m_mouseInWindow;
	//是否需要追踪鼠标移动消息
	BOOL m_bTracking;
	//是否模态对话框(默认非模态FALSE)
	BOOL m_bModal;
	//对话框ID
	UINT m_nIDTemplate;

public:
	//CAD焦点消息处理
	afx_msg LRESULT OnAcadKeepFocus(WPARAM wParam, LPARAM lParam);
	//窗口关闭消息
	afx_msg void OnClose();
	//鼠标移动消息(用于判断鼠标是否在窗口内)
	//参考:http://www.cnblogs.com/greatverve/archive/2013/02/06/TRACKMOUSEEVENT.html
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};
