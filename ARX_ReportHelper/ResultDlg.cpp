#include "stdafx.h"
#include "ResultDlg.h"
#include "ReasonsDescDlg.h"
#include "CGridColumnTraitImage.h"

IMPLEMENT_DYNAMIC(ResultDlg, GasBaseAcesDlg)

ResultDlg::ResultDlg(CWnd* pParent /*=NULL*/)
	: GasBaseAcesDlg(ResultDlg::IDD, pParent),m_isTrue(TRUE),m_isOthers(FALSE)
{
}

ResultDlg::~ResultDlg()
{
}

void ResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESULT_LIST, m_resultCtrlList);
	DDX_Control(pDX, IDC_COMBO1, m_methodCombBox);
}


BEGIN_MESSAGE_MAP(ResultDlg, CDialog)
	ON_BN_CLICKED(IDOK, &ResultDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_RESULT_LIST, &ResultDlg::OnNMClickResultList)
END_MESSAGE_MAP()

static void UncheckOthers(CGridListCtrlGroups& listCtrl,int nIndex)
{
	//�õ�ǰ����ѡ��״̬������ȡ���������ѡ��
	for(int i = 0; i < listCtrl.GetItemCount(); i++)
	{
		if(nIndex != i)
			listCtrl.SetCheck(i, FALSE);	//set unchecked
	}
}

BOOL ResultDlg::OnInitDialog()
{
	GasBaseAcesDlg::OnInitDialog();
	m_methodCombBox.AddString(_T("�ֳ��˲�"));
	m_methodCombBox.AddString(_T("�������"));
	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(m_objectName,datasVector,2);
	
	InitListCtrl();
	if(datasVector.empty()) 
	{
		int methodIndx = EvaluMethod(m_objectName);
		m_methodCombBox.SetCurSel(methodIndx);
		return TRUE;
	}
	m_methodCombBox.SetWindowText(datasVector[1][1]);
	int rowNum = m_resultCtrlList.GetItemCount();
	for(int i = 0; i < rowNum; i++)
	{
		CString text = m_resultCtrlList.GetItemText(i,0);
		CString conclusion = datasVector[2][1].kACharPtr();
		if(conclusion != text) continue;
		m_resultCtrlList.SetCheck(i);
		UncheckOthers(m_resultCtrlList,i);
	}

	ArrayVector otherDatas;
	ReportDataHelper::ReadDatas(m_descObjectName,otherDatas,2);
	if(otherDatas.empty()) return TRUE;
	m_reasonsDesc = otherDatas[0][0].kACharPtr();
	m_isTrue = _ttoi(otherDatas[0][1]);

	return TRUE;
}

void ResultDlg::setItems()
{
	int nItem = 0;
	for(int i = 0; i < m_datas.length() ; ++i)
	{
		const CString& strCellText = m_datas[i].kACharPtr();
		m_resultCtrlList.InsertItem(nItem,strCellText);
		m_resultCtrlList.SetItemText(nItem, 1, strCellText);
		nItem++;
	}	
}
void ResultDlg::InitListCtrl()
{
	m_resultCtrlList.SetExtendedStyle(m_resultCtrlList.GetExtendedStyle() | LVS_EX_CHECKBOXES);
	m_resultCtrlList.SetCellMargin(3);

	CRect rect;
	m_resultCtrlList.GetClientRect(rect); //��õ�ǰ�ͻ�����Ϣ  

	CGridColumnTrait* pTrait = new CGridColumnTraitImage(1, 2);
	m_resultCtrlList.InsertColumnTrait(0, _T("���ѡ��"), LVCFMT_LEFT, rect.Width(), 0, pTrait);
	setItems();
	m_resultCtrlList.SetCheck(0);
	//UpdateData(FALSE);
}

void ResultDlg::OnBnClickedOk()
{
	BOOL ret = FALSE;
	CString strConlusion,strMethod;
	int retIndx;
	m_methodCombBox.GetWindowText(strMethod);

	//�ж�ѡ�е����ĸ�����ȡ��ѡ�е��ı�
	for(int i = 0; i < m_resultCtrlList.GetItemCount(); i++)
	{
		ret = ret | m_resultCtrlList.GetCheck(i);
		if(ret)
		{
			strConlusion = m_resultCtrlList.GetItemText(i,0);
			retIndx = i;
			break;
		}
	}

	if(!ret)
	{
		AfxMessageBox(_T("����ѡ��һ�����!"));
		return;
	}

	//���ѡ��������ѡ��򲻸����Ƿ���ȷ�������
	if(!m_isOthers)
	{
		//�ж�ѡ�е��������ǲ�����ȷ��
		vector<int>::iterator iter = find( m_retTrueIndxs.begin(),m_retTrueIndxs.end(),retIndx);
		if(iter != m_retTrueIndxs.end())
			m_isTrue = TRUE;
		else 
			m_isTrue = FALSE;
	}
	writeDatasToFile(strConlusion,strMethod);
	OnOK();
}

void ResultDlg::setItemDatas( const AcStringArray& datas )
{
	m_datas.removeAll();
	m_datas.append(datas);
}

void ResultDlg::OnNMClickResultList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nIndex;
	POSITION pos = m_resultCtrlList.GetFirstSelectedItemPosition();
	nIndex = m_resultCtrlList.GetNextSelectedItem(pos);  // ��ȡ������

	//if(nIndex < 0) return;
	//acutPrintf(_T("\n�Ƿ�ѡ��:%s"),m_resultCtrlList.GetCheck(nIndex)?_T("��"):_T("��"));
	//acutPrintf(_T("\n%d\n"),nIndex);

	//�����ǰû��ѡ����ѡ��
	//�����ǰ�Ѿ���ѡ��״̬��������
	if(m_resultCtrlList.GetCheck(nIndex))
	{
		//m_resultCtrlList.SetCheck(nIndex, FALSE);
	}
	else
	{
		m_resultCtrlList.SetCheck(nIndex,TRUE); //set checked
		UncheckOthers(m_resultCtrlList,nIndex);
	}

	showReasonsDescDlg(nIndex);

	*pResult = 0;
}

void ResultDlg::setTrueIndex( const IntArray& trueIndexs, const CString& objectName ,const CString& descObjectName)
{
	m_retTrueIndxs.clear();
	copy(trueIndexs.begin(), trueIndexs.end(), back_inserter(m_retTrueIndxs));
	//m_retTrueIndxs.push_back(trueIndexs);

	m_objectName = objectName;
	m_descObjectName = descObjectName;
}

void ResultDlg::writeDatasToFile(CString& strConlusion,const CString& strMethod)
{
	CString strIsTrue;
	strIsTrue.Format(_T("%d"),m_isTrue);
	CString strRet = _T("��");
	strRet.Append(strMethod);
	strRet.Append(_T("��Ϊ��"));
	strRet.Append(strConlusion);

	if(m_isOthers)
	{
		m_reasonsDesc.Replace(_T(" "), NULL);
		strRet = m_reasonsDesc;
		strConlusion = m_reasonsDesc;
	}

	strRet.Append(_T("��"));
	AcStringArray datas;
	datas.append(strRet);
	datas.append(strMethod);
	datas.append(strConlusion);
	datas.append(strIsTrue);
	ArrayVector dataVector;
	dataVector.push_back(m_bookMks);
	dataVector.push_back(datas);
	ReportDataHelper::WriteDatas(m_objectName,dataVector);
}

void ResultDlg::setBookMarks( const AcStringArray& bookMks )
{
	m_bookMks.removeAll();
	m_bookMks.append(bookMks);
}

void ResultDlg::showReasonsDescDlg(int nIndex)
{
	CString itemText = m_resultCtrlList.GetItemText(nIndex,0);
	if(-1 != itemText.Find(_T("����")) )
	{
		m_isOthers = TRUE;
		ReasonsDescDlg dlg;
		dlg.m_reasonsDesc = m_reasonsDesc;
		dlg.m_yesOrNo = m_isTrue;
		AfxInitRichEdit2();
		if(IDOK == dlg.DoModal())
		{
			m_isTrue = dlg.m_yesOrNo;
			m_reasonsDesc = dlg.m_reasonsDesc;
			ArrayVector dataVector;
			AcStringArray tmp;
			tmp.append(m_reasonsDesc);
			dataVector.push_back(tmp);
			CString strTrue;
			strTrue.Format(_T("%s"),m_isTrue?_T("1"):_T("0"));
			tmp.removeAll();
			tmp.append(strTrue);
			dataVector.push_back(tmp);
			ReportDataHelper::WriteDatas(m_descObjectName,dataVector);
		}
	}
	else m_isOthers = FALSE;

}