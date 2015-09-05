#pragma once
#include "dlimexp.h"

class MINEGECMDS_DLLIMPEXP PropertyDataDlgHelper
{
public:
	// ��ʾ"���ԶԻ���"(��ʾȫ������)
	static void DisplayAllPropertyDataDlg(const AcDbObjectId& objId);

	// ��ʾ"���ԶԻ���"(��ʾ��������)
	static void DisplayPartialPropertyDataDlg(const AcDbObjectId& objId, const AcStringArray& fields);

	// ͨ�����ԶԻ����Զ��ж���ͼԪ�������ݶ���
	static bool DisplayPropertyDataDlg(const AcDbObjectId& objId);
	
	static void InputPropertyDataDlg( const AcDbObjectId& objId, const AcStringArray& fields, const CString& func );
	// ͨ����������������ʾ���ԶԻ���
	static bool DisplayPropertyByFunName(const AcDbObjectId& objId, const CString funName);
};
