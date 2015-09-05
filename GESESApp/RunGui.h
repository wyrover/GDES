#ifndef RUN_GUI_H
#define RUN_GUI_H

#include  <windows.h>
#include  <tchar.h>
#include  <QDialog>
#include "RegHelper.h"

//#include "FileOpt.h"
#include "HardWare.h"
#include "Reg.h"
#include "RegTbOpt.h"
#define MAX_BUFF 255

class QTimer;
class QLabel;
class QPushButton;

class   RunGui : public   QDialog
{
	Q_OBJECT
public :
	RunGui ();
	~ RunGui ();

private slots:
	void startCADNull();
	void startCADByFile();
	void about();
	void updateTime();

protected:
	void closeEvent(QCloseEvent *event);  

private :
	void paintEvent(QPaintEvent *e);
	void getTime();
	void initDialog();
	void setBtnStyle();
	void cheakRegCode();

	QString lastPath;
	QTimer *myTimer;
	QString sTime; //��ǰʱ��
	QLabel *timeLabel;
	//QString windTitle;

	QPushButton *startNullBtn;
	QPushButton *startFileBtn;
	QPushButton *helpBtn;

	RegHelper *regHelper;
	bool m_registed;

protected:
	// ��ʹ��ע��, �����ж��Ƿ�ע�ᡢ�Ƿ����쳣����ȵ�
	// ����ֵ: 1�� δע�ᡢini�ļ������ڡ���ע�������ֵ
	//         2�� Ӳ��ID���û�ID����
	//         3�� ini�ļ��е�ֵ��ע����м��ܵ�ʹ�ô���ֵ������
	//         4�� ��ע��, ��ʹ�ô����ѵ�

	UINT InitRegProc();

	// δע�ᡢini�ļ������ڡ���ע�������ֵ
	BOOL IsErrorOne();

	// Ӳ��ID���û�ID����
	BOOL IsErrorTwo();

	// ini�ļ��е�ֵ��ע����м��ܵ�ʹ�ô���ֵ������
	BOOL IsErrorThree();

	// ���ʹ�ô����ѵ�
	BOOL IsErrorFour();

	// ��ʾ��ʾ��Ϣ���Լ�ע��Ի���
	bool ShowRegDlg();

	// ����ע�ắ��
	void ProcReg();

public:
	CHardWare   m_hWare;     // Ӳ����Ϣ������
	CRegTbOpt   m_regOpt;    // ע��������
	//CFileOpt    m_fileOpt;   // �ļ�������
	CReg        m_reg;       // ע���������
	//CRegDlg     m_regDlg;

	int         m_nIniSelect;           // ��INI�ļ��ж�����ѡ�����������
	int         m_nIniUTime;            // INI�ļ��ж�����ʣ��ʹ�ô���
	int         m_nRegUTime;            // ע����ж�����ʣ��ʹ�ô���

	TCHAR        m_chWinPath[MAX_BUFF];  // ���Easson2d.ini�ļ�ȫ·��
	DISKINFO    m_dInfo;                // ���Ӳ����Ϣ
	CString     m_strUID;               // ���û���(���ֻ����)

	UINT        m_nErrorCode;           // ��ע������ʹ������ʱ,���մ�������
	BOOL        m_bFirstReg;            // �Ƿ��ǵ�һ��ע��(Ĭ��ΪFALSE)

};
#endif //RUN_GUI_H