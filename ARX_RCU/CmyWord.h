#ifndef C_MY_WORD_H
#define C_MY_WORD_H

#include "msword.h"  
  
#define wdCharacter 1  
#define wdLine 5  
#define wdStory 6
#define wdCell 12  
#define wdExtend 1  
#define wdMove 0  
//ҳü��ͼ
#define wdSeekCurrentPageHeader 9
//ҳ����ͼ
#define wdSeekCurrentPageFooter 10
//��ǰ��ͼ
#define wdSeekMainDocument 0

//�����
#define wdAlignParagraphJustify 3
//����
#define wdAlignParagraphCenter 1
//�Ҷ���
#define wdAlignParagraphRight 2

#include "atlbase.h"  
  
  
class CmyWord    
{  
public:  
    CmyWord();  
    virtual ~CmyWord();  
private:  
    _Application m_wdApp;  
    Documents m_wdDocs;  
    _Document m_wdDoc;  
    Selection m_wdSel;  
    Range     m_wdRange; 
	Table m_table;
	Window m_activeWind;
	View m_view;

public:  
	BOOL isUsing(CString fileName);
    //����  
    //**********************�������ĵ�*******************************************  
    BOOL CreateApp();                    //����һ���µ�WORDӦ�ó���  
    BOOL CreateDocuments();                //����һ���µ�Word�ĵ�����  
    BOOL CreateDocument();                //����һ���µ�Word�ĵ�  
    BOOL Create();                        //�����µ�WORDӦ�ó��򲢴���һ���µ��ĵ�  
    void ShowApp();                        //��ʾWORD�ĵ�  
    void HideApp();                        //����word�ĵ�  
  
    //**********************���ĵ�*********************************************  
    BOOL OpenDocument(CString fileName);//���Ѿ����ڵ��ĵ���  
    BOOL Open(CString fileName);        //�����µ�WORDӦ�ó��򲢴�һ���Ѿ����ڵ��ĵ���  
    BOOL SetActiveDocument(short i);    //���õ�ǰ������ĵ���  
  
    //**********************�����ĵ�*********************************************  
    BOOL SaveDocument();                //�ĵ����Դ���ʽ�����档  
    BOOL SaveDocumentAs(CString fileName);//�ĵ��Դ�����ʽ�����档  
    BOOL CloseDocument();  
    void CloseApp();   
  
    //**********************�ı���д����*****************************************  
    void WriteText(CString szText,long alignment = wdAlignParagraphJustify);        //��ǰ��괦д�ı� 
	void SetFont(CString fonName,float fontSize);
    void WriteNewLineText(CString szText, int nLineCount = 1); //��N��д��  
    void WriteEndLine(CString szText);    //�ĵ���β��д�ı�  
    void WholeStory();                    //ȫѡ�ĵ�����  
    void Copy();                        //�����ı����ݵ�������  
    void InsertFile(CString fileName);    //�����ص��ļ�ȫ������д�뵽��ǰ�ĵ��Ĺ�괦��  
    //----------------------add by zxx--------------------------------------  
    //***********************������********************************************  
    //���°���ѡ��  
    void SelectMoveDown(short lineCount, short unit);//��ѡ��������ƶ�  
    void NoneSelectMoveDown(short lineCount, short unit);//�����ƶ���꣬��ѡ��  
    void SelectMoveUp(short lineCount, short unit);//��ѡ��������ƶ�  
    void NoneSelectMoveUp(short lineCount, short unit);//�����ƶ���꣬��ѡ��  
    //���Ұ���ѡ��  
    void SelectMoveLeft(short charCount, short unit);//��ѡ��������ƶ�  
    void NoneSelectMoveLeft(short charCount, short unit);//  
    void SelectMoveRight(short charCount, short unit);//��ѡ��������ƶ�  
    void NoneSelectMoveRight(short charCount, short unit);//  
  
	//�����ַ��� Ȼ��ȫ���滻
	void FindWord(CString FindW,CString RelWord);
	void CopyLine();
	//��λ����ǩ
	BOOL Goto(const CString& strBMark);

    void MoveToFirst();  
	void MoveToEnd();
    void MoveToNextPage();  
    void TypeParagraph();  
    void PasteAndFormat();  
    void Paste();  
    void TypeBackspace(int count);
	//����ǩλ�ø�����
	void UpdateField(CString strBMark);
	//������ǩ
	void InsetBookMark(CString strBMark);
	//�������
	void InsertSymbol(const CString& strFont,long charNum);
	//nRow������nCell����
	void CreateTable( int nRow, int nCell );
	//�ϲ�����еĵ�Ԫ��
	void MergeTable(int mergeFromRow, int mergeFromCol, int mergeToRow, int mergeToCol);

	//nRow�У�nCell��
	void SetTableText( int nRow, int nCell, const CString& text, int fontSize = 10,long align = 1 );

	//ɾ��N��
	void Delete(int nCount);

	//�Ƿ���ʾ��ǩ
	void ShowBookmarks(BOOL show = TRUE);
	void SetSeekView(const CString& text,long align, long currentView,const CString& findW = _T(""));
}; 
extern CmyWord* MyWord;
#endif //C_MY_WORD_H