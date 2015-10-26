#ifndef FUNC_FIELD_HELPER_H
#define FUNC_FIELD_HELPER_H

#include "dlimexp.h"

class MINEGE_DLLIMPEXP FuncFieldHelper
{
public:
	static bool GetFunctions(AcStringArray& funcs);                                 // ��ȡ���й���
	static bool GetTypes(const CString& f, AcStringArray& types);                   // ��ȡ�����µ���������
	static bool GetFuncsByType(const CString& type, AcStringArray& funcs);			//��ȡĳ�������µ����й���
	static bool GetFields(const CString& f, const CString& type, AcStringArray& fields);      // ��ȡ�ֶ�
	static bool AddField(const CString& f, const CString& type, const CString& field);        // ����ֶ�
	static bool RemoveFunction(const CString& f);                                             // ɾ������
	static bool RemoveType(const CString& f, const CString& type);                            // ɾ������
	static bool RemoveField(const CString& f, const CString& type, const CString& field);     // ɾ���ֶ�
};

#endif // FUNC_FIELD_HELPER_H
