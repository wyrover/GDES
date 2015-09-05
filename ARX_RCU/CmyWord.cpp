#include "StdAfx.h"
#include "CmyWord.h"

void CmyWord::FindWord(CString FindW,CString RelWord)
{
	m_wdSel=m_wdApp.GetSelection();
	Find myFind=m_wdSel.GetFind();
	if(!myFind.m_lpDispatch)
	{
		AfxMessageBox(_T("��ȡFind ����ʧ��"));
		return;
	}
	else
	{
		//���������ǰ���vba д��
		myFind.ClearFormatting();
		Replacement repla=myFind.GetReplacement();
		repla.ClearFormatting();

		COleVariant Text(FindW);
		COleVariant re(RelWord);
		COleVariant vTrue((short)TRUE), vFalse((short)FALSE);
		COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		CComVariant v(1);
		CComVariant v2(2);
		CComVariant v3(_T(""));
		//�����Replace ��Ӧ���滻�ķ�Χ������.
		// 1 ����һ�� 2 ���������ĵ�
		//myFind.Execute(Text,vFalse,vFalse,vFalse,vFalse,vFalse,vTrue,&v,vFalse,re,&v2,vOpt,vOpt,vOpt,vOpt);
		myFind.Execute(Text,vFalse,vFalse,vFalse,vFalse,vFalse,
			vTrue,&v,vFalse,&re,&v2,vOpt,vOpt,vOpt,vOpt);
	}
}

CmyWord::CmyWord()  
{  
  
}  
  
  
CmyWord::~CmyWord()  
{  
  
}  
  
BOOL CmyWord::isUsing(CString fileName)
{
	COleVariant vTrue((short)TRUE),      
		vFalse((short)FALSE),  
		vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	CLSID clsid;
	CLSIDFromProgID( TEXT("Word.Application"), &clsid);  

	IUnknown *pUnk = NULL;

	HRESULT hr = ::GetActiveObject(clsid,NULL,&pUnk);
	if(FAILED(hr)|| pUnk == NULL) return FALSE;
	
	_Application wdApp;
	hr = pUnk->QueryInterface(IID_IDispatch, (LPVOID *)&wdApp);
	if(FAILED(hr)) return FALSE;

	Documents wdDocs = wdApp.GetDocuments();
	short docNum = (short)wdDocs.GetCount();

	//��word����򿪣�����û�д��ĵ�����رճ��򣨷�ֹword��̨���У�Ӱ�����������ٶȣ�
	if(docNum <= 0) 
	{
		wdApp.Quit(vFalse,vOptional,vOptional);
		wdApp.ReleaseDispatch();
		return FALSE;
	}

	_Document  wdDoc;
	wdDoc = wdApp.GetActiveDocument(); 
	if(wdDoc != NULL)
	{
		CString bstrName = wdDoc.GetFullName(); 
		if(bstrName == fileName)
		{
			return TRUE;
		}
	}
	return FALSE;

}
//����  
BOOL CmyWord::CreateApp()  
{  
	COleException pe;  
	if (!m_wdApp.CreateDispatch(_T("Word.Application"), &pe))  
    {  
        AfxMessageBox(_T("Application����ʧ�ܣ���ȷ����װ��word 2000�����ϰ汾!"), MB_OK|MB_ICONWARNING);  
        pe.ReportError();  
        throw &pe;  
        return FALSE;  
    }  
    return TRUE;  
}  
  
BOOL CmyWord::CreateDocuments()  
{  
    if (FALSE == CreateApp())   
    {  
        return FALSE;  
    }  
    m_wdDocs.AttachDispatch(m_wdApp.GetDocuments());  
    if (!m_wdDocs.m_lpDispatch)   
    {  
        AfxMessageBox(_T("Documents����ʧ��!"), MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    return TRUE;  
}  
  
BOOL CmyWord::CreateDocument()  
{  
    if (!m_wdDocs.m_lpDispatch)   
    {  
        AfxMessageBox(_T("DocumentsΪ��!"), MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
  
    COleVariant varTrue(short(1),VT_BOOL),vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  
    CComVariant Template(_T(""));    //û��ʹ��WORD���ĵ�ģ��  
    CComVariant NewTemplate(false),DocumentType(0),Visible;  
  
    m_wdDocs.Add(&Template,&NewTemplate,&DocumentType,&Visible);      
  
    //�õ�document����  
    m_wdDoc = m_wdApp.GetActiveDocument();  
    if (!m_wdDoc.m_lpDispatch)   
    {  
        AfxMessageBox(_T("Document��ȡʧ��!"), MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    //�õ�selection����  
    m_wdSel = m_wdApp.GetSelection();  
    if (!m_wdSel.m_lpDispatch)   
    {  
        AfxMessageBox(_T("Select��ȡʧ��!"), MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    //�õ�Range����  
    m_wdRange = m_wdDoc.Range(vOptional,vOptional);  
    if(!m_wdRange.m_lpDispatch)  
    {  
        AfxMessageBox(_T("Range��ȡʧ��!"), MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
  
    return TRUE;  
}  
  
BOOL CmyWord::Create()  
{  
    if (FALSE == CreateDocuments())   
    {  
        return FALSE;  
    }  
    return CreateDocument();  
}  
  
void CmyWord::ShowApp()  
{  
    m_wdApp.SetVisible(TRUE);  
}  
  
void CmyWord::HideApp()  
{  
    m_wdApp.SetVisible(FALSE);  
}  
  
BOOL CmyWord::OpenDocument(CString fileName)  
{  
    if (!m_wdDocs.m_lpDispatch)   
    {  
        AfxMessageBox(_T("DocumentsΪ��!"), MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
  
    COleVariant vTrue((short)TRUE),      
                vFalse((short)FALSE),  
                vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR),  
                vZ((short)0);  
    COleVariant vFileName(fileName);  
      
	
	//�õ�document����  
	//m_wdDoc.AttachDispatch(m_wdDocs.Add(vFileName, vOptional, vOptional, vOptional)) ;                    // Close AttachDispatch  

    //�õ�document����  
    m_wdDoc.AttachDispatch(m_wdDocs.Open(  
                                vFileName,        // FileName  
                                vTrue,            // Confirm Conversion.  
                                vFalse,            // ReadOnly.  
                                vFalse,            // AddToRecentFiles.  
                                vOptional,        // PasswordDocument.  
                                vOptional,        // PasswordTemplate.  
                                vOptional,        // Revert.  
                                vOptional,        // WritePasswordDocument.  
                                vOptional,        // WritePasswordTemplate.  
                                vOptional,        // Format. // Last argument for Word 97  
                                vOptional,        // Encoding // New for Word 2000/2002  
                                vOptional,        // Visible  
                                //����4����word2003��Ҫ�Ĳ��������汾��word2000��  
                                vOptional,    // OpenAndRepair  
                                vZ,            // DocumentDirection wdDocumentDirection LeftToRight  
                                vOptional,    // NoEncodingDialog  
                                vOptional  
                                  
                                )                // Close Open parameters  
                            );                    // Close AttachDispatch  
      
    if (!m_wdDoc.m_lpDispatch)   
    {  
        AfxMessageBox(_T("Document��ȡʧ��!"), MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    //�õ�selection����  
    m_wdSel = m_wdApp.GetSelection();  
    if (!m_wdSel.m_lpDispatch)   
    {  
        AfxMessageBox(_T("Select��ȡʧ��!"), MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    //�õ�ȫ��DOC��Range����  
    m_wdRange = m_wdDoc.Range(vOptional,vOptional);  
    if(!m_wdRange.m_lpDispatch)  
    {  
        AfxMessageBox(_T("Range��ȡʧ��!"), MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    return TRUE;  
}  
  
BOOL CmyWord::Open(CString fileName)  
{  
	if(FALSE == PathFileExists(fileName) || -1 == fileName.Find(_T(".doc")))
	{
		AfxMessageBox(_T("��ȷ��·���Ƿ���ȷ��"),MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	CComVariant Read(FALSE);
	CComVariant AddToR(FALSE);
	CComVariant Name(fileName);

	COleVariant vTrue((short)TRUE);
	COleVariant vFalse((short)FALSE);
	COleVariant varstrNull(_T(""));
	COleVariant varZero((short)0);
	COleVariant varTrue(short(1),VT_BOOL);
	COleVariant varFalse(short(0),VT_BOOL);
	COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	if(!m_wdApp.m_lpDispatch)
	{
		if(CreateApp()==FALSE)
		{
			return FALSE;
		}
	}
	if(!m_wdDoc.m_lpDispatch)
	{
		m_wdDocs=m_wdApp.GetDocuments();
		if(!m_wdDocs.m_lpDispatch)
		{
			AfxMessageBox(_T("DocuMent ���󴴽�ʧ��"));
			CloseApp();
			return FALSE;
		}
	}
	CComVariant format(0);//�򿪷�ʽ 0 Ϊdoc�Ĵ򿪷�ʽ
	//doc=docs.Open(&Name,varFalse,&Read,&AddToR,vOpt,vOpt,vFalse,vOpt,vOpt,&format,vOpt,vTrue,vOpt,vOpt,vOpt,vOpt);
	m_wdDoc = m_wdDocs.Add(&Name, vOpt, vOpt, vOpt);
	if(!m_wdDoc.m_lpDispatch)
	{
		AfxMessageBox(_T("�ļ���ʧ��"));
		CloseApp();
		return FALSE;
	}
	else
	{
		m_wdSel=m_wdApp.GetSelection();
		if(!m_wdSel.m_lpDispatch)
		{
			AfxMessageBox(_T("��ʧ��"));
			CloseApp();
			return FALSE;
		}
		return TRUE;
	}
}  
  
BOOL CmyWord::SetActiveDocument(short i)  
{  
    COleVariant     vIndex(i),vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  
  
    m_wdDoc.AttachDispatch(m_wdDocs.Item(vIndex));  
    m_wdDoc.Activate();  
    if (!m_wdDoc.m_lpDispatch)   
    {  
        AfxMessageBox(_T("Document��ȡʧ��!"), MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    //�õ�selection����  
    m_wdSel = m_wdApp.GetSelection();  
    if (!m_wdSel.m_lpDispatch)   
    {  
        AfxMessageBox(_T("Select��ȡʧ��!"), MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    //�õ�ȫ��DOC��Range����  
    m_wdRange = m_wdDoc.Range(vOptional,vOptional);  
    if(!m_wdRange.m_lpDispatch)  
    {  
        AfxMessageBox(_T("Range��ȡʧ��!"), MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    HideApp();  
    return TRUE;  
}  
  
BOOL CmyWord::SaveDocument()  
{  
    if (!m_wdDoc.m_lpDispatch)   
    {  
        AfxMessageBox(_T("Document��ȡʧ��!"), MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    m_wdDoc.Save();  
    return TRUE;  
}  
  
BOOL CmyWord::SaveDocumentAs(CString fileName)  
{  
    if (!m_wdDoc.m_lpDispatch)   
    {  
        AfxMessageBox(_T("Document��ȡʧ��!"), MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    COleVariant vTrue((short)TRUE),      
                vFalse((short)FALSE),  
                vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  
    COleVariant vFileName(fileName);  
  
    m_wdDoc.SaveAs(  
                vFileName,    //VARIANT* FileName  
                vOptional,    //VARIANT* FileFormat  
                vOptional,    //VARIANT* LockComments  
                vOptional,    //VARIANT* Password  
                vOptional,    //VARIANT* AddToRecentFiles  
                vOptional,    //VARIANT* WritePassword  
                vOptional,    //VARIANT* ReadOnlyRecommended  
                vOptional,    //VARIANT* EmbedTrueTypeFonts  
                vOptional,    //VARIANT* SaveNativePictureFormat  
                vOptional,    //VARIANT* SaveFormsData  
                vOptional,    //VARIANT* SaveAsAOCELetter  
                vOptional,    //VARIANT* ReadOnlyRecommended  
                vOptional,    //VARIANT* EmbedTrueTypeFonts  
                vOptional,    //VARIANT* SaveNativePictureFormat  
                vOptional,    //VARIANT* SaveFormsData  
                vOptional    //VARIANT* SaveAsAOCELetter  
                );  
    return    TRUE;  
}  
  
BOOL CmyWord::CloseDocument()  
{  
	CComVariant vTrue(TRUE);
	CComVariant vFalse(FALSE);
	CComVariant vOpt;
	CComVariant cSavechage(FALSE);
	if(!m_wdDoc.m_lpDispatch)
	{
		AfxMessageBox(_T("_Document �����ȡʧ��,�رղ���ʧ��"));
		return FALSE;
	}
	else
	{
		//�����һ��������vTrue ����ִ��󣬿����Ǻ���Ĳ���ҲҪ��Ӧ�ı仯
		//��vba û�и���Ӧ���� �Ҿ������ַ���������

		m_wdDoc.Close(&vFalse,&vOpt,&vOpt);
	}
	return TRUE;
}  
  
void CmyWord::CloseApp()  
{  
	COleVariant vOpt(( long )DISP_E_PARAMNOTFOUND,  VT_ERROR);
	if(!m_wdApp.m_lpDispatch)
	{
		AfxMessageBox(_T("��ȡWord ����ʧ��,�رղ���ʧ��"));
		return;
	}
	else
	{
		m_wdApp.Quit(vOpt,vOpt,vOpt);
		m_wdRange.ReleaseDispatch();  
		m_wdSel.ReleaseDispatch();  
		m_wdDoc.ReleaseDispatch();  
		m_wdDocs.ReleaseDispatch(); 
		m_view.ReleaseDispatch();
		m_activeWind.ReleaseDispatch();
		m_wdApp.ReleaseDispatch(); 
	}
}  
  
void CmyWord::WriteText(CString szText,long alignment)  
{  
    m_wdSel.TypeText(szText);
	_ParagraphFormat format = m_wdSel.GetParagraphFormat();
	format.SetAlignment(alignment);
	m_wdSel.SetParagraphFormat(format);
}  
  
void CmyWord::WriteNewLineText(CString szText, int nLineCount /**//* = 1 */)  
{  
    int i;  
    if (nLineCount <= 0)  
    {  
        nLineCount = 0;  
    }  
    for (i = 0; i < nLineCount; i++)  
    {  
        m_wdSel.TypeParagraph();  
    }  
    WriteText(szText);  
}  
  
void CmyWord::WriteEndLine(CString szText)  
{  
    m_wdRange.InsertAfter(szText);  
}  
  
void CmyWord::WholeStory()  
{  
    m_wdRange.WholeStory();  
}  
  
void CmyWord::Copy()  
{  
    m_wdSel.Copy();  
    //m_wdSel.CopyFormat();  
}  
  
void CmyWord::TypeParagraph()  
{  
    m_wdSel.TypeParagraph();  
}  
  
void CmyWord::PasteAndFormat()  
{  
    m_wdSel.PasteAndFormat(0);  
}  
  
void CmyWord::Paste()  
{  
    m_wdSel.Paste();  
    //m_wdSel.PasteFormat();  
}  
  
void CmyWord::TypeBackspace(int count)  
{  
   for(int i = 0; i < count; i++)  
   m_wdSel.TypeBackspace();  
}  
   
void CmyWord::InsertFile(CString fileName)  
{  
    COleVariant     vFileName(fileName),  
                 vTrue((short)TRUE),  
                 vFalse((short)FALSE),  
                 vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR),  
                 vNull(_T(""));  
    /**//* 
    void InsertFile(LPCTSTR FileName, VARIANT* Range, VARIANT* ConfirmConversions, VARIANT* Link, VARIANT* Attachment); 
    */  
    m_wdSel.InsertFile(  
                    fileName,  
                    vNull,  
                    vFalse,  
                    vFalse,  
                    vFalse  
                    );  
}  
  
void CmyWord::SelectMoveDown(short lineCount, short unit)//��ѡ��������ƶ�  
{  
    m_wdSel.MoveDown(COleVariant(unit), COleVariant((short)lineCount),COleVariant((short)wdExtend));  
}  
  
void CmyWord::NoneSelectMoveDown(short lineCount, short unit)//�����ƶ���꣬��ѡ��  
{  
    m_wdSel.MoveDown(COleVariant(unit), COleVariant((short)lineCount),COleVariant((short)wdMove));  
}  
  
void CmyWord::SelectMoveUp(short lineCount, short unit)//��ѡ��������ƶ�  
{  
    m_wdSel.MoveUp(COleVariant(unit), COleVariant((short)lineCount),COleVariant((short)wdExtend));  
}  
  
void CmyWord::NoneSelectMoveUp(short lineCount, short unit)//�����ƶ���꣬��ѡ��  
{  
    m_wdSel.MoveUp(COleVariant(unit), COleVariant((short)lineCount),COleVariant((short)wdMove));  
}  
  
void CmyWord::SelectMoveLeft(short charCount, short unit)//��ѡ��������ƶ�  
{  
    m_wdSel.MoveLeft(COleVariant(unit), COleVariant((short)charCount),COleVariant((short)wdExtend));  
}  
  
void CmyWord::NoneSelectMoveLeft(short charCount, short unit)//  
{  
    m_wdSel.MoveLeft(COleVariant(unit), COleVariant((short)charCount),COleVariant((short)wdMove));  
}  
void CmyWord::SelectMoveRight(short charCount, short unit)//��ѡ��������ƶ�  
{  
    m_wdSel.MoveRight(COleVariant(unit), COleVariant((short)charCount),COleVariant((short)wdExtend));  
}  
void CmyWord::NoneSelectMoveRight(short charCount, short unit)//  
{  
    m_wdSel.MoveRight(COleVariant(unit), COleVariant((short)charCount),COleVariant((short)wdMove));  
}  
void CmyWord::MoveToFirst()  
{  
    m_wdSel.GoTo(COleVariant((short)1), COleVariant((short)2), COleVariant((short)0), COleVariant(_T("1")));  
}  
  
void CmyWord::MoveToNextPage()  
{  
    m_wdSel.GoTo(COleVariant((short)1), COleVariant((short)2), COleVariant((short)1), COleVariant(_T("")));  
}  

void CmyWord::UpdateField(CString strBMark)
{
	COleVariant varBMark_del(strBMark);		//���ñ���
	Bookmarks  bookmarks = m_wdDoc.GetBookmarks();
	if(!bookmarks.Exists((LPCTSTR)strBMark))
	{
		return;
	}
	m_wdSel.GoTo(COleVariant(short(1),VT_BOOL),COleVariant((short)0),COleVariant((short)0),varBMark_del);
	Fields fields = Fields(m_wdSel.GetFields());
	fields.Update();
	bookmarks.ReleaseDispatch();
}

void CmyWord::CopyLine()
{
	m_wdSel.HomeKey(COleVariant((short)wdLine),COleVariant((short)0));
	m_wdSel.EndKey(COleVariant((short)wdLine),COleVariant((short)wdExtend));
	m_wdSel.Copy();
}

BOOL CmyWord::Goto( const CString& strBMark )
{
	COleVariant varBMark(strBMark);		//���ñ���
	Bookmarks  bookmarks = m_wdDoc.GetBookmarks();
	if(!bookmarks.Exists((LPCTSTR)strBMark))
	{
		return FALSE;
	}
	m_wdSel.GoTo(COleVariant(short(1),VT_BOOL),COleVariant((short)0),COleVariant((short)0),varBMark);
	return TRUE;
}

void CmyWord::CreateTable(int nRow, int nCell )
{
	  COleVariant vTrue((short)TRUE),      
	              vFalse((short)FALSE),  
	              vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  

	Range range = m_wdSel.GetRange();
	Tables tables = m_wdDoc.GetTables();
	m_table = tables.Add(range,nRow,nCell,vOptional,vOptional);
	m_table.AutoFitBehavior(1);
	m_table.AutoFitBehavior(2);
	Borders borders = m_table.GetBorders();
	borders.SetEnable(TRUE);
	Rows rows  =  m_table.GetRows();   
}

/*
	@hd
	@2015/4/20/2:01
	@D¥����
	֮ǰд����ȡ��Ԫ��ķ���
	���ַ�����ȱ���ǣ�����е�Ԫ��
	�кϲ����ȡ��������������Ϊû��
	�жϻ��ƣ��ᵼ�±���
	Range r = m_wdSel.GetRange();
	Rows rows = m_table.GetRows();   
	Row row = rows.Item(nRow);		
	Cells cells = row.GetCells(); 
	cells.SetVerticalAlignment(align);//���־���
	Cell cell = cells.Item(nCell);
*/
void CmyWord::SetTableText( int nRow, int nCell, const CString& text,int fontSize,long align )
{
	Cell cell = m_table.Cell(nRow,nCell);
	cell.SetVerticalAlignment(align);
	Range r = cell.GetRange();
	
	_Font font = r.GetFont();
	font.SetSize(fontSize);//�������

	_ParagraphFormat parafm = r.GetParagraphFormat();
	parafm.SetAlignment(align); //Ĭ��Ϊ1��Ҳ���Ǿ���
	
	r.SetParagraphFormat(parafm);
	r.SetFont(font);
	r.SetText(text);
}

void CmyWord::InsetBookMark( CString strBMark )
{
	Bookmarks bMarks = m_wdDoc.GetBookmarks();
	Range range = m_wdSel.GetRange();
	VARIANT vRange;
	VariantInit(&vRange);
	vRange.vt = VT_DISPATCH;
	vRange.pdispVal = (IDispatch*)range;
	bMarks.Add((LPCTSTR)strBMark,&vRange);
	bMarks.ReleaseDispatch();
}

void CmyWord::InsertSymbol(const CString& strFont,long charNum)
{
	COleVariant vTrue((short)TRUE),      
		vFalse((short)FALSE),  
		vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  
	COleVariant font(strFont);
	//long charNum = -3842;
	m_wdSel.InsertSymbol(charNum,font,vTrue,vOptional);
}

void CmyWord::MergeTable( int mergeFromRow, int mergeFromCol, int mergeToRow, int mergeToCol )
{
	Cell cellTo = m_table.Cell(mergeToRow,mergeToCol);
	Cell cell = m_table.Cell(mergeFromRow,mergeFromCol);
	cell.Merge(cellTo);

	Range r = cellTo.GetRange();
	_ParagraphFormat parafm = r.GetParagraphFormat();
	parafm.SetAlignment(1); //Ĭ��Ϊ1��Ҳ���Ǿ���
	r.SetParagraphFormat(parafm);
}

void CmyWord::Delete(int nCount)
{
	VARIANT cCount;
	VariantInit(&cCount);
	cCount.vt = VT_INT;
	cCount.pdispVal = (IDispatch*)1;

	for(int i = 0; i < nCount; i++)
		m_wdSel.Delete(&cCount,&cCount);
}

void CmyWord::ShowBookmarks( BOOL show )
{
	m_wdApp.SetDisplayStatusBar(TRUE);
	m_wdApp.SetShowWindowsInTaskbar(TRUE);
	m_wdApp.SetShowStartupDialog(TRUE);
	//Window m_activeWind;
	//View m_view;
	m_activeWind.AttachDispatch(m_wdApp.GetActiveWindow());
	m_activeWind.SetDisplayHorizontalScrollBar(TRUE);
	m_activeWind.SetDisplayVerticalScrollBar(TRUE);
	m_activeWind.SetDisplayLeftScrollBar(FALSE);
	m_activeWind.SetStyleAreaWidth(/*m_wdApp.CentimetersToPoints(float(1))*/0);
	m_activeWind.SetDisplayVerticalRuler(TRUE);
	m_activeWind.SetDisplayRightRuler(FALSE);
	m_activeWind.SetDisplayScreenTips(TRUE);
	m_view.AttachDispatch(m_activeWind.GetView());
	m_view.SetShowAnimation(TRUE);
	m_view.SetDraft(FALSE);
	m_view.SetWrapToWindow(FALSE);
	m_view.SetShowPicturePlaceHolders(FALSE);
	m_view.SetShowFieldCodes(FALSE);
	m_view.SetShowBookmarks(show);
	m_view.SetFieldShading(2);
	m_view.SetShowTabs(FALSE);
	m_view.SetShowSpaces(FALSE);
	m_view.SetShowParagraphs(TRUE);
	m_view.SetShowHyphens(FALSE);
	m_view.SetShowHiddenText(FALSE);
	m_view.SetShowAll(FALSE);
	m_view.SetShowDrawings(TRUE);
	m_view.SetShowObjectAnchors(FALSE);
	m_view.SetShowTextBoundaries(FALSE);
	m_view.SetShowHighlight(TRUE);
	m_view.SetShowOptionalBreaks(FALSE);
	m_view.SetDisplayPageBoundaries(TRUE);
	m_view.SetDisplaySmartTags(TRUE);
}

//align 1���� 3�����
//currentView 10ҳ�� 9ҳü
void CmyWord::SetSeekView(const CString& text,long align, long currentView, const CString& findW)
{
	Pane mPane;
	m_activeWind = m_wdApp.GetActiveWindow();
	mPane = m_activeWind.GetActivePane();
	m_view = mPane.GetView();
	m_view.SetSeekView(currentView);
	if(!findW.IsEmpty())
	{
		FindWord(findW,text);
	}
	else
	{
		m_wdSel.TypeText(text);
	}
	_ParagraphFormat parafm = m_wdSel.GetParagraphFormat();
	parafm.SetAlignment(align);
	m_wdSel.SetParagraphFormat(parafm);
	m_view.SetSeekView(wdSeekMainDocument);
	mPane.ReleaseDispatch();
}

void CmyWord::SetFont( CString fonName,float fontSize )
{
	_Font font(m_wdSel.GetFont());
	font.SetName(fonName);
	font.SetSize(fontSize);
	m_wdSel.SetFont(font);

}

void CmyWord::MoveToEnd()
{
	m_wdSel.EndKey(COleVariant((short)wdStory),COleVariant((short)0));
}
CmyWord* MyWord = 0;