#pragma  once
#include "../MineGE/DataListHelper.h"
#include "../MineGE/DataHelper.h"

#include <iostream>
#include <fstream>
using namespace std;

//AcFStream.hһ��Ҫ����iostream��fstream�ȱ�׼��ͷ�ļ��ĺ���!!!(�м�)
#include "AcFStream.h"

#define ISNULL _T("**")

//��ʼ��ȫ�ֱ���(new������󣬲���ʼ��com)
extern bool initword();
//ж��ȫ�ֱ���(�ͷ��ڴ棬�ͷ�com)
extern void uninitword();
//���ɱ���
extern bool CreatReport(const CString& savePath,const AcDbObjectId& drill_site);

extern void OpenWordDocument( const CString& filePath );
/**
�޸ĵĵط���
1����wordOperate.cpp���CmyWord MyWord�ĳ�CmyWord* MyWord;
2���Ҹ�����ΪҪ�ֶ���ʼ��CmyWord* MyWordȫ�ֱ���
   ��CmyWord* MyWord������Ǩ�Ƶ�CmyWord.h���������CmyWord.cpp��
   -->�μ�CmyWorld.hβ����ע��˵��!!!
3����wordOperate.cpp��Ĵ��붼���Ƶ�CreateReport.cpp���ɾ��wordOperate.h��wordOperate.cpp�ļ�
4����CreateReport��ͷ�ļ�������2��ȫ�ֺ��������ڳ�ʼ����ж��com(initword��uninitword)
   ��������CmyWord�Ĺ��캯�������CoInitialize()����!
   -->�μ�CreateReport.h��CreateReport.cpp�еĺ���ʵ��!!
5���޸���CreatReport()������ǩ����ȥ����һ��ģ���ļ�����
   -->�μ�CreateReport.h CreateReport.cpp acrxEntryPoint.cpp�е�WordTest_ARXWORD()����!!
*/
