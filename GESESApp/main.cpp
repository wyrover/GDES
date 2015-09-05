#include <QApplication>
#include <QMessageBox>
#include <QProcess>
#include "RunGui.h"
#include "CADHelper.h"
#include <QTranslator>
#include "eoLogger.h"

//#include <psapi.h >
//#pragma comment(lib,"psapi.lib")

////�������Ƿ����
//#include <tlhelp32.h>  
//static BOOL FindProcess(LPCTSTR appName)  
//{  
//	int i=0;  
//	PROCESSENTRY32 pe32;  
//	pe32.dwSize = sizeof(pe32);   
//	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  
//	if(hProcessSnap == INVALID_HANDLE_VALUE)  
//	{  
//		i+=0;  
//	}  
//	BOOL bMore = ::Process32First(hProcessSnap, &pe32);  
//	while(bMore)  
//	{  
//		//printf(" �������ƣ�%s \n", pe32.szExeFile);  
//		if(stricmp(appName,pe32.szExeFile)==0)  
//		{  
//			//printf("����������");  
//			i+=1;  
//		}  
//		bMore = ::Process32Next(hProcessSnap, &pe32);  
//	}  
//	if(i>1){           //����1���ų�����  
//		return true;  
//	}else{  
//		return false;  
//	}  
//}  

static int initRunCAD()
{
	QString cadPath;
	QStringList argument;
	if (!isAutoCADExist(cadPath,argument ))
	{
		return 0;
	}
	if (!copyCadFile())
	{
		QMessageBox msg;
		msg.setWindowTitle(QObject::tr("reminder"));
		msg.setText(QObject::tr("failure"));
		msg.exec();
		return 0;
	}
	cadPath = QDir::currentPath();
	writeReg(cadPath);
	return 1;
}
/***************************************************************
�����Խ�������ÿ�������Ϊ����֮��������������ļ����ڵ�
Ŀ¼�£����Ի��Զ��ҵ�arx�ļ�.
 *  ���ű��ã���ʱ����ȷ��дע����ʱ���Ƿ���Ҫarx�ļ�����
 *	��CAD��װĿ¼��
 *	QDir dir(QDir::currentPath()+"/JLCAD/");
 *	QString fromDir = dir.currentPath()+"/JLCAD/";
 *	QString toDir;
 *	QStringList infolist = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);  
 *	for(int i=0; i<infolist.size(); i++)  
 *	{
 *		fromDir = infolist.at(i);
 *		writFiles(fromDir,toDir,infolist.at(i));
 *	}
 *
*******************************************************************************/


int main(int argc, char *argv[])
{
	QTextCodec *textCode = QTextCodec::codecForLocale(); 
	QTextCodec::setCodecForCStrings( textCode ); 
	QTextCodec::setCodecForLocale( textCode ); 
	QTextCodec::setCodecForTr( textCode );

	// At this time we are switching to warning message and messages
	// which are going to follow it are going to be warnings message too.
	// These messages can be displayed only if the user level (sets with
	// eo::setlevel function) is set to eo::warnings.
	//eo::log << eo::warnings;

	//eo::log << eo::setlevel(eo::logging);

	// With the following eo::file function we are defining that
	// all future logs are going to this new file resource which is
	// test.txt
	//eo::log << eo::file("log.txt") << "In FILE" << std::endl;

	// Now we are changing again the resources destination to cout which
	// is the standard output.
	//eo::log << std::cout << "In COUT" << std::endl;

	QApplication app(argc, argv);
	QTranslator translator;
	QTranslator translator_zh;
	translator.load(":/rungui_zh");
	translator_zh.load(":/qt_zh_CN");
	app.installTranslator(&translator);
	app.installTranslator(&translator_zh);

	if (0 == FindProcess("acad.exe"))
	{
		QMessageBox msg;
		msg.setWindowTitle(QObject::tr("reminder"));
		msg.setText(QObject::tr("CAD Process is running"));
		msg.exec();
		return 0;
	}


	if (1 == FindProcess("GESESApp.exe"))
	{
		QMessageBox msg;
		msg.setWindowTitle(QObject::tr("reminder"));
		msg.setText(QObject::tr("Process is running"));
		msg.exec();
		return 0;
	}
	if(0 == initRunCAD()) return 0;
	RunGui *mainWin = new RunGui;
	//QString cadPath;
	//QStringList argument;

	//if (!isAutoCADExist(cadPath,argument )) return 0;
	//cadPath.append("\\JLcad");
	//if(!QFile::exists(cadPath) || QFile::)
	//{
	//	QMessageBox msg;
	//	msg.setWindowTitle(QObject::tr("error"));
	//	msg.setText(QObject::tr("did not reg"));
	//	msg.show();
	//	return app.exec();
	//}

	mainWin->show();
	return app.exec();
}