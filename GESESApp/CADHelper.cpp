#include "CADHelper.h"
#include <qt_windows.h>
#include <QSettings>
#include <QMessageBox>

#include "Registry.h"
#include "RegEntry.h"
#include <tchar.h>

#include "eoLogger.h"


//#include <psapi.h >
//#pragma comment(lib,"psapi.lib")

//�������Ƿ����
#include <tlhelp32.h> 
//���� -1 û���ҵ�
//����  0 �ҵ�����
//����  1 �ҵ������������һ��

int FindProcess(LPCTSTR appName)  
{  
	int i=0;  
	PROCESSENTRY32 pe32;  
	pe32.dwSize = sizeof(pe32);   
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  
	if(hProcessSnap == INVALID_HANDLE_VALUE)  
	{  
		i+=0;  
	}  
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);  
	while(bMore)  
	{  
		//printf(" �������ƣ�%s \n", pe32.szExeFile);  
		if(stricmp(appName,pe32.szExeFile)==0)  
		{  
			//printf("����������");  
			i+=1;  
		}  
		bMore = ::Process32Next(hProcessSnap, &pe32);  
	}  
	if(i>1)//����1���ų�����  
	{           
		return 1;  
	}

	else if( i > 0 && i < 2 )
	{
		return 0;
	}

	else
	{  
		return -1;  
	}  
}  
static void msbbox(const QString& msg)
{
	QMessageBox msgBox;
	msgBox.setText(msg);
	msgBox.exec();
}
bool copyFileToPath( QString& sourceDir ,QString& toDir, bool coverFileIfExist )
{
	sourceDir.replace("/","\\");
	toDir.replace("/","\\");
	if (sourceDir == toDir)
	{ 
		return true; 
	} 
	bool ret1 = QFile::exists(sourceDir);
	bool ret2 = QFile::exists(toDir);
	if (!ret1 || !ret2)
	{ 
		//msbbox("�ļ�������");
		//msbbox(sourceDir);
		//msbbox(toDir);
		return false; 
	} 
	QDir *createfile = new QDir; 
	bool exist = createfile->exists(toDir); 
	if (exist)
	{ 
		if(coverFileIfExist)
		{ 
			createfile->remove(toDir); 
		} 
	}//end if 

	if(!QFile::copy(sourceDir, toDir)) 
	{ 
		//msbbox("yyy");
		return false; 
	} 
	//msbbox("���");
	//eo::log << sourceDir.toStdString().c_str() << "->�ļ������ɹ�->" << toDir.toStdString().c_str() << std::endl;
	return true; 
}

bool copyCadFile()
{
	QDir lsDir;
	//QString cadPath = lsDir.homePath();
	//cadPath.append("/AppData/Roaming/Autodesk/AutoCAD 2010/R18.0/chs/Support");
	
	QString cadPath = getCADPath("RoamableRootFolder",CURRENT_USER);
	cadPath.append("Support");
	lsDir.setPath(cadPath);

	//filepath == "C:\Users\Username\AppData\Roaming\Autodesk\AutoCAD 2010\R18.0\chs\Support"
	QString filepath = lsDir.filePath("acad.CUIX");
	//filepath == "C:\Users\Username\AppData\Roaming\Autodesk\AutoCAD 2010\R18.0\chs\Support\hd.txt"

	//��ȡ��ǰ����(.exe)���ڵ�·��
	QString CurrentPath = lsDir.currentPath();

	QString fromDir = CurrentPath;
	fromDir.append("\\Datas\\JL\\acad.CUIX");
	//�ļ�����:��ǰ��������λ��+"/Datas/JL/hd.txt"

	QString toDir = filepath;
	
	//QString toDir = testPath;
	//QMessageBox msgBox;
	//msgBox.setText(toDir);
	//msgBox.exec();
	if(!copyFileToPath(fromDir,toDir,true)) return false;

	filepath = lsDir.filePath("acad.mnr");
	CurrentPath = lsDir.currentPath();
	fromDir = CurrentPath;
	fromDir.append("\\Datas\\JL\\acad.mnr");
	toDir = filepath;
	return copyFileToPath(fromDir,toDir,true);
}

bool recorverCadFile()
{
	QDir lsDir;
	//QString cadPath = lsDir.homePath();
	//cadPath.append("/AppData/Roaming/Autodesk/AutoCAD 2010/R18.0/chs/Support");
	QString cadPath = getCADPath("RoamableRootFolder",CURRENT_USER);
	cadPath.append("/Support");
	lsDir.setPath(cadPath);

	//filepath == "C:\Users\Username\AppData\Roaming\Autodesk\AutoCAD 2010\R18.0\chs\Support"
	QString filepath = lsDir.filePath("acad.CUIX");
	//filepath == "C:\Users\Username\AppData\Roaming\Autodesk\AutoCAD 2010\R18.0\chs\Support\hd.txt"

	//��ȡ��ǰ����(.exe)���ڵ�·��
	QString CurrentPath = lsDir.currentPath();

	QString fromDir = CurrentPath;
	fromDir.append("/Datas/CAD/acad.CUIX");
	//�ļ�����:��ǰ��������λ��+"/Datas/JL/hd.txt"

	QString toDir = filepath;
	if(!copyFileToPath(fromDir,toDir,true)) return false;

	filepath = lsDir.filePath("acad.mnr");
	CurrentPath = lsDir.currentPath();
	fromDir = CurrentPath;
	fromDir.append("/Datas/CAD/acad.mnr");
	toDir = filepath;
	return copyFileToPath(fromDir,toDir,true);
}

QString GetCADPathByQt( QString locationKey ,pathType pat)
{
	QString path;
	if(CURRENT_USER == pat)
	{
		//path = "HKEY_CURRENT_USER\\SOFTWARE\\AutoDesk\\AutoCAD";
		path = "HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804";
	}
	else if(LOCAL_MACHINE == pat)
	{
		//path = "HKEY_LOCAL_MACHINE\\SOFTWARE\\AutoDesk\\AutoCAD";
		path = "HKEY_LOCAL_MACHINE\\Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804";
	}
	QSettings *settings = new QSettings( path, QSettings::NativeFormat);
	//QStringList key = settings->allKeys();
	//int num = (int)key.size();
	QString cadPath = settings->value(locationKey).toString();
	delete settings;
	settings = NULL;
	return cadPath;
}

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
BOOL IsWow64() 
{ 
	BOOL bIsWow64 = FALSE; 
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress( 
		GetModuleHandle(TEXT("kernel32")),"IsWow64Process"); 
	if (NULL != fnIsWow64Process) 
	{ 
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64)) 
		{ 
			// handle error 
			//AfxMessageBox(_T("IsWow64 error!")); 
		} 
	}
	return bIsWow64; 
}

QString GetCADPathByWinAPI( QString locationKey ,pathType pat)
{
	QString cadPath;

	TCHAR cadKey[MAX_PATH];
	_tcscpy(cadKey, _T("Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804"));

	HKEY hRoot = (CURRENT_USER == pat)?HKEY_CURRENT_USER:HKEY_LOCAL_MACHINE;
	CRegistry reg;

	 DWORD dwAccess = KEY_ALL_ACCESS;
	 //�ж��Ƿ�64λ
	 if(IsWow64())
	 {
		 dwAccess |= KEY_WOW64_64KEY;
	 }
	bool ret = reg.Open( cadKey, hRoot, dwAccess);
	if(ret)
	{
		ret = false;
		int n = reg.Count();
		for(int i=0;i<n;i++)
		{
			CRegEntry* entry = reg.GetAt(i);
			if(locationKey.compare(entry->lpszName, Qt::CaseInsensitive) == 0)
			{
				cadPath = QString::fromStdString((tstring)*entry);
				ret = true; break;
			}
		}
		//SubKeyExists����ʹ!
		//std::string name = locationKey.toStdString();
		//ret = reg.SubKeyExists(name.c_str());
		//if(ret)
		//{
		//	cadPath = QString::fromStdString((std::string)reg[name.c_str()]);
		//}
	}
	reg.Close();

	return cadPath;
}

QString getCADPath( QString locationKey ,pathType pat)
{
//#ifdef _WIN64
	return GetCADPathByWinAPI(locationKey, pat);
//#else
	//return GetCADPathByQt(locationKey, pat);
//#endif
}

bool isAutoCADExist( QString& cadPath,const QStringList& argument)
{
	bool isExist = false;
	//CAD�İ�װĿ¼
	cadPath = getCADPath("AcadLocation",LOCAL_MACHINE);
	if(cadPath.isEmpty()) 
	{
		cadPath = QObject::tr("Not install AutoCAD");
		QMessageBox msg;
		msg.setWindowTitle(QObject::tr("tip"));
		msg.resize(400,300);
		msg.setText(cadPath);
		msg.exec();
		isExist = false;
		return false;
	}

	else isExist = true;
	//cadPath.append("\\acad.exe");
	//cadPath.replace("\\","/");

	return isExist;
}

bool writeTestKeybyWinAPI(QString& cadPath)
{
	TCHAR cadKey[MAX_PATH];
	_tcscpy(cadKey, _T("Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\Applications\\test"));

	DWORD dwAccess = KEY_ALL_ACCESS;
	//�ж��Ƿ�64λ
	if(IsWow64())
	{
		dwAccess |= KEY_WOW64_64KEY;
	}

	CRegistry reg;
	bool ret = reg.Open( cadKey, HKEY_LOCAL_MACHINE, dwAccess);
	if(ret)
	{
		//if(reg.Count()>0)
		//{
			reg[_T("LOADCTRLS")] = (DWORD)2;
			cadPath.append("\\VVLoader.arx");
			reg[_T("LOADER")] = cadPath.toStdString();
		//}
	}
	reg.Close();

	return ret;
}

bool writeInfomationKeybyWinAPI(QString& cadPath)
{
	TCHAR cadKey[MAX_PATH];
	_tcscpy(cadKey, _T("Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\InfoCenter"));

	DWORD dwAccess = KEY_ALL_ACCESS;
	//�ж��Ƿ�64λ
	if(IsWow64())
	{
		dwAccess |= KEY_WOW64_64KEY;
	}

	CRegistry reg;
	bool ret = reg.Open( cadKey, HKEY_CURRENT_USER, dwAccess);
	if(ret)
	{
		ret = false;
		if(reg.Count()>0)
		{
			int n = reg.Count();
			for(int i=0;i<n;i++)
			{
				CRegEntry* entry = reg.GetAt(i);
				if(QString::compare("InfoCenterOn", entry->lpszName, Qt::CaseInsensitive) == 0)
				{
					*entry = 0;
					ret = true; break;
				}
			}
		}
	}
	reg.Close();

	return ret;
}

bool writeRegByWinAPI( QString& cadPath)
{
	if(!writeTestKeybyWinAPI(cadPath)) return false;
	if(!writeInfomationKeybyWinAPI(cadPath)) return false;
	return true;
}

void writeRegByQt( QString& cadPath)
{
/*2014.9.14--hd--hld--��Զ233
************************************************************************************
	����:�˴���õ���CAD�İ�װĿ¼,Ϊʲô��CAD��װĿ¼�������ÿ����ļ�����װĿ¼�� *
	����ļ�����ô�����ģ�������arx�ļ����ڵ�Ŀ¼Ϊʲô���У���				       *
	�������:��ΪCAD����RunGui.exe�����ģ�RunGui.exe���ڵ�Ŀ¼����cad�Ĺ���Ŀ¼��  *
	ֻҪ���Ŀ¼����Ҫ���ص��ļ�,����ע���д����CAD�İ�װĿ¼�����ļ����ڵ�Ŀ¼�� *
	������ʱ�򶼻�����ļ���     												   *
	���ս��:˵���ף�ֻҪ"LOADCTRLS"��ֵ��VVLoader.arx�Ϳ����ˣ�Ȼ����������Ŀ¼�� *
	��VVLoader.arx����ļ���һЩ�������ˡ�                                         *
************************************************************************************
*/
	//arx�ļ����Զ�����
	QSettings *reg = new QSettings("HKEY_LOCAL_MACHINE\\Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\Applications\\test",
		QSettings::NativeFormat);
	if(!reg->allKeys().isEmpty()) return;
	reg->setValue("LOADCTRLS",2);
	cadPath.append("\\VVLoader.arx");
	reg->setValue("LOADER",cadPath);
	delete reg; 

	//������������ȥ��
	QSettings *regFind = new QSettings("HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\InfoCenter",
		QSettings::NativeFormat);
	if(regFind->contains("InfoCenterOn"))
	{
		regFind->setValue("InfoCenterOn",0);
	}
	delete regFind;
}

void writeReg( QString& cadPath)
{
	writeRegByWinAPI(cadPath);
	//writeRegByQt(cadPath);
}

bool delTestKeybyWinAPI()
{
	TCHAR cadKey[MAX_PATH];
	_tcscpy(cadKey, _T("Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\Applications"));

	DWORD dwAccess = KEY_ALL_ACCESS;
	//�ж��Ƿ�64λ
	if(IsWow64())
	{
		dwAccess |= KEY_WOW64_64KEY;
	}

	CRegistry reg;
	reg.Clear(HKEY_LOCAL_MACHINE,cadKey,_T("test"),dwAccess);
	//bool ret = reg.Open( cadKey, HKEY_LOCAL_MACHINE, dwAccess);
	//if(ret)
	//{
	//	reg.DeleteKey();
	//}
	//reg.Close();

	return true;
}

bool delInfomationKeybyWinAPI()
{
	TCHAR cadKey[MAX_PATH];
	_tcscpy(cadKey, _T("Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\InfoCenter"));

	DWORD dwAccess = KEY_ALL_ACCESS;
	//�ж��Ƿ�64λ
	if(IsWow64())
	{
		dwAccess |= KEY_WOW64_64KEY;
	}

	CRegistry reg;
	bool ret = reg.Open( cadKey, HKEY_CURRENT_USER, dwAccess);
	if(ret)
	{
		ret = false;
		if(reg.Count()>0)
		{
			int n = reg.Count();
			for(int i=0;i<n;i++)
			{
				CRegEntry* entry = reg.GetAt(i);
				if(QString::compare(_T("InfoCenterOn"), entry->lpszName, Qt::CaseInsensitive) == 0)
				{
					*entry = 1;
					ret = true; break;
				}
			}
		}
	}
	reg.Close();

	return ret;
}

bool delRegByWinAPI()
{
	if(!delTestKeybyWinAPI()) return false;
	if(!delInfomationKeybyWinAPI()) return false;
	return true;
}

void deleteRegByQt()
{
	//�Զ�����ע����ɾ��
	QSettings *reg = new QSettings("HKEY_LOCAL_MACHINE\\Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\Applications\\test",
		QSettings::NativeFormat);
	if (reg->allKeys().isEmpty())
	{
		return;
	}
	reg->clear();
	delete reg;

	//�����������Ļָ�
	QSettings *regFind = new QSettings("HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\InfoCenter",
		QSettings::NativeFormat);
	if(regFind->contains("InfoCenterOn"))
	{
		regFind->setValue("InfoCenterOn",1);
	}
	delete regFind; 
}

bool deleteReg()
{
	return delRegByWinAPI();
	//return deleteRegByQt();
}

void writFiles( QString& fromDir,QString& toDir, const QString fileName )
{
	QStringList args;
	//�˴�toDirΪCAD�İ�װĿ¼
	toDir = toDir + "\\" + fileName;
	//isAutoCADExist(toDir,args);
	if(!copyFileToPath(fromDir,toDir,true)) return;
}