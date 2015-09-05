#ifndef FILE_COPY_H
#define FILE_COPY_H

#include <QDir>
#include <QFileInfoList>
#include <QString>
#include <windows.h>
/*
qCopyDirectory -- ����Ŀ¼
fromDir : ԴĿ¼
toDir   : Ŀ��Ŀ¼
bCoverIfFileExists : ture:ͬ��ʱ����  false:ͬ��ʱ����false,��ֹ����
����: ture�����ɹ� false:����δ���
*/
enum pathType
{
	CURRENT_USER = 0, //�õ���Ŀ¼C:\Users\hd\AppData\Roaming\Autodesk\AutoCAD 2010\R18.0\chs\Support
	LOCAL_MACHINE = 1, //�õ���Ŀ¼��CAD�İ�װĿ¼
};
extern bool copyFileToPath( QString& sourceDir , QString& toDir, bool coverFileIfExist) ;

extern bool copyCadFile();

extern bool recorverCadFile();

extern void writeReg(QString& cadPath);

extern bool deleteReg();

//QString getPath(int index,QString keyorvalue,pathType pat);

extern  QString getCADPath(QString locationKey,pathType pat);

extern  bool isAutoCADExist(QString& cadPath,const QStringList& argument);

extern void writFiles( QString& fromDir, QString& toDir, const QString fileName);

extern int FindProcess(LPCTSTR appName);
//void ReadFluentInstallPath( QString& path );
#endif //FILE_COPY_H
