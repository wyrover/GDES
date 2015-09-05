#pragma once

#include "RcuGE.h"

//�곡
class DEFGE_EXPORT_API DrillSite : public MineGE
{
public:
	ACRX_DECLARE_MEMBERS(DrillSite) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	//���캯��
	DrillSite();

	// ��ȡ���������
	AcGePoint3d getInsertPt() const;

	// ���ò��������
	void setInsertPt(const AcGePoint3d& pt);

	//// ��ȡ���ӵ�����
	//AcGePoint3d getLinkPt() const;

	//// �������ӵ�����
	//void setLinkPt(const AcGePoint3d& pt);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // ���������
	//AcGePoint3d m_linkPt;    // ���ӵ�����
};

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DrillSite)
#endif
