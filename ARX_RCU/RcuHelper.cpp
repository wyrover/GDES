#include "StdAfx.h"
#include "Rcu2.h"
#include "RcuDataLink.h"
#include "SwitchHelper.h"
#include "RcuHelper.h"
#include "CreatReport.h"
#include <algorithm>

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

#include "../DefGE/RcuGE.h"
#include "../DefGE/Pore.h"
#include "../DefGE/DrillSite.h"
#include "../DefGE/CoalSurface.h"

void RcuHelper::FindAllDrillSite(AcDbObjectIdArray& drill_sites)
{
	ArxDataTool::GetEntsByType(_T("DrillSite"), drill_sites, true);
}

void RcuHelper::GetRelatedCoalSurface(const AcDbObjectId& drill_site, AcDbObjectId& coal_surf)
{
	//�������е�ú��ͼԪ
	AcDbObjectIdArray tags;
	ArxDataTool::GetEntsByType(_T("CoalSurface"), tags, true);

	//����ú��ͼԪ����������
	AcDbObjectIdArray hosts;
	DrawHelper::GetHosts(tags, hosts);

	//ƥ��
	int n = hosts.length();
	for(int i=0;i<n;i++)
	{
		if(hosts[i] == drill_site)
		{
			coal_surf = tags[i];
			break;
		}
	}
}

void RcuHelper::GetRelatedClosePores(const AcDbObjectId& coal_surf, AcDbObjectIdArray& pores)
{
	//�������е����ͼԪ
	AcDbObjectIdArray tags;
	ArxDataTool::GetEntsByType(_T("Pore"), tags, true);

	//�������ͼԪ����������
	AcDbObjectIdArray hosts;
	DrawHelper::GetHosts(tags, hosts);

	//ƥ��
	int n = hosts.length();
	for(int i=0;i<n;i++)
	{
		if(hosts[i] == coal_surf)
		{
			pores.append(tags[i]);
		}
	}
}

void RcuHelper::GetRelatedOpenPores(const AcDbObjectId& drill_site, AcDbObjectIdArray& pores)
{
	//�������е����ͼԪ
	AcDbObjectIdArray tags;
	ArxDataTool::GetEntsByType(_T("Pore"), tags, true);

	//�������ͼԪ����������
	AcDbObjectIdArray hosts;
	DrawHelper::GetHosts(tags, hosts);

	//ƥ��
	int n = hosts.length();
	for(int i=0;i<n;i++)
	{
		if(hosts[i] == drill_site)
		{
			pores.append(tags[i]);
		}
	}
}

void RcuHelper::ClearRelatedOpenPores(const AcDbObjectId& drill_site)
{
	//�����������곡���������
	AcDbObjectIdArray pores;
	RcuHelper::GetRelatedOpenPores(drill_site, pores);
	if(pores.isEmpty()) return;

	//ɾ�����
	ArxEntityHelper::EraseObjects(pores, Adesk::kTrue);
}

void RcuHelper::ClearRelatedClosePores(const AcDbObjectId& coal_surf)
{
	//�����������곡���������
	AcDbObjectIdArray pores;
	RcuHelper::GetRelatedOpenPores(coal_surf, pores);
	if(pores.isEmpty()) return;

	//ɾ�����
	ArxEntityHelper::EraseObjects(pores, Adesk::kTrue);
}

bool RcuHelper::GetDrillSiteAndCoalSurfDatas(const AcDbObjectId& drill_site, DrillSiteLink& ds_link, CoalSurfaceLink& cs_link)
{
	//��ȡ�곡�������������ݲ���䵽�Ի�����
	if(!RcuHelper::ReadDrillSiteData(drill_site, ds_link)) return false;

	//�����곡������ú��
	AcDbObjectId coal_surf;
	RcuHelper::GetRelatedCoalSurface(drill_site, coal_surf);
	if(coal_surf.isNull()) return false;

	//��ȡú�������
	if(!RcuHelper::ReadCoalSurfaceData(coal_surf, cs_link))	return false;

	return true;
}

bool RcuHelper::GetDrillSiteData(const AcDbObjectId& drill_site, DrillSiteLink& ds_link)
{
	//��ȡ�곡����
	return RcuHelper::ReadDrillSiteData(drill_site, ds_link);
}

bool RcuHelper::ReadCoalSurfaceData(const AcDbObjectId& coal_surf, CoalSurfaceLink& cs_link)
{
	if(coal_surf.isNull()) return false;

	cs_link.setDataSource(coal_surf);
	return cs_link.updateData(false);
}

bool RcuHelper::ReadDrillSiteData(const AcDbObjectId& drill_site, DrillSiteLink& ds_link)
{
	if(drill_site.isNull()) return false;

	ds_link.setDataSource(drill_site);
	return ds_link.updateData(false);
}

bool RcuHelper::ReadPoreData(const AcDbObjectId& pore, PoreLink& po_link)
{
	if(pore.isNull()) return false;

	po_link.setDataSource(pore);
	po_link.updateData(false);
	return true;
}

void RcuHelper::VectorToAngle(const AcGeVector3d& v, double& ang1, double& ang2)
{
	//������xoyƽ���ϵ�����ͶӰ
	AcGeVector3d u = v.orthoProject(AcGeVector3d::kZAxis);
	//acutPrintf(_T("\nͶӰֵ:x=%.3f, y=%.3f z=%.3f"), u.x, u.y, u.z);

	//angleTo����Ǽнǣ���Χ��[0,180]
	//��������u��Y��ļн�(ƫ��)
	ang1 = u.angleTo(AcGeVector3d::kYAxis);
	if(u.x < 0) ang1 *= -1;

	//��������u��v�ļн�(����)����Χ��[0,180]
	ang2 = u.angleTo(v);
	if(v.z < 0) ang2 *= -1;
}

void RcuHelper::VectorToAngleTest()
{
	AcGeVector3d v(1,1,1);
	double ang1, ang2;
	RcuHelper::VectorToAngle(v, ang1, ang2);
	acutPrintf(_T("\n����:(%.3f, %.3f, %.3f)"), v.x, v.y, v.z);
	acutPrintf(_T("\n����-->�����1:%f  ���ǣ�%f"), ang1, ang2);
	acutPrintf(_T("\n�Ƕ�-->�����1:%f  ���ǣ�%f"), ang1*57.295, ang2*57.295);

	AcGeVector3d v1(1,0,0);
	RcuHelper::VectorToAngle(v1, ang1, ang2);
	acutPrintf(_T("\n����:(%.3f, %.3f, %.3f)"), v1.x, v1.y, v1.z);
	acutPrintf(_T("\n����-->�����1:%f  ���ǣ�%f"), ang1, ang2);
	acutPrintf(_T("\n�Ƕ�-->�����1:%f  ���ǣ�%f"), ang1*57.295, ang2*57.295);

	AcGeVector3d v2(-1,-1,1);
	RcuHelper::VectorToAngle(v2, ang1, ang2);
	acutPrintf(_T("\n����:(%.3f, %.3f, %.3f)"), v2.x, v2.y, v2.z);
	acutPrintf(_T("\n����-->�����1:%f  ���ǣ�%f"), ang1, ang2);
	acutPrintf(_T("\n�Ƕ�-->�����1:%f  ���ǣ�%f"), ang1*57.295, ang2*57.295);
}

bool RcuHelper::CaculCoalSurfParam( DrillSiteLink& ds_link, CoalSurfaceLink& cs_link)
{
	AcGePoint3d orign = ds_link.m_pt;

	//����rcu����м���
	Rcu2 rcu;
	//����ú�����(��Ǻͺ��)
	rcu.setCoalSurf(DegToRad(cs_link.m_angle), cs_link.m_thick);
	//�����곡����������
	rcu.setDrillSite1(ds_link.m_height, ds_link.m_width);
	//�����곡���������ұ������
	rcu.setDrillSite2(ds_link.m_top, ds_link.m_bottom, ds_link.m_left, ds_link.m_right);
	//�����곡����ú��Ĵ���
	rcu.setDrillSite3(ds_link.m_dist);

	//����ú����ĳ�ɷ�Χ��Ⱥ͸߶�
	rcu.drillExtent(cs_link.m_width, cs_link.m_height);
	//����ú����(���ĵ����ꡢ��������������������������)
	AcGePoint3d cnt;
	AcGeVector3d normV, headV, dipV;
	if(!rcu.drillSurface(cnt, normV, headV, dipV)) return false;
	cs_link.m_pt = cnt + orign.asVector();
	cs_link.m_normV = normV;
	cs_link.m_headV = headV;
	cs_link.m_dipV = dipV;

	return true;
}

bool RcuHelper::CaculRelativeOpenPorePts(CoalSurfaceLink& cs_link, DrillSiteLink& ds_link, AcGePoint3dArray& pts,AcGePoint3dArray& drawPts)
{
	//������׵��к����ϵķֲ�����
	int n1,n2;
	CaculPoreNums(n1,n2,cs_link.m_width,cs_link.m_height,2*cs_link.m_gas_radius);

	//������׵�����
	double gap1 = ds_link.m_width/(n1+1);
	double gap2 = ds_link.m_height/(n2+1);
	//acutPrintf(_T("\ngap1:%.3f, gap2:%.3f"), gap1, gap2);

	AcGeVector3d headV(AcGeVector3d::kXAxis);
	AcGeVector3d dipV(AcGeVector3d::kZAxis);

	//�Ծ������½�Ϊ����(���ڼ���)
	AcGePoint3d origin = AcGePoint3d::kOrigin + headV*(-0.5)*ds_link.m_width + dipV*0;
	AcGeVector3d v1 = AcGeVector3d::kXAxis;
	AcGeVector3d v2 = AcGeVector3d::kYAxis;
	AcGePoint3d origin2 = AcGePoint3d::kOrigin + v1*(-0.5)*ds_link.m_width + v2*0;

	for(int i=0;i<n1;i++)
	{
		AcGePoint3d pt = origin + headV*gap1*(i + 1);
		for(int j=0;j<n2;j++)
		{
			pt += dipV * gap2;
			pts.append(pt);
			drawPts.append(origin2+v1*gap1*(i+1)+v2*gap2*(j+1));
		}
	}
	return true;
}

bool RcuHelper::CaculRelativeClosePorePts(CoalSurfaceLink& cs_link,AcGePoint3dArray& pts,AcGePoint3dArray& drawPts)
{
	//�����к�������׵ĸ���
	int n1,n2;
	CaculPoreNums(n1,n2,cs_link.m_width,cs_link.m_height,2*cs_link.m_gas_radius);

	double gap = 2*cs_link.m_gas_radius;
	//�������½ǵĵ�����,������㿪ʼ����
	AcGePoint3d origin = AcGePoint3d::kOrigin + cs_link.m_headV*cs_link.m_width*-0.5 + cs_link.m_dipV*cs_link.m_height*-0.5;
	AcGeVector3d v1 = AcGeVector3d::kXAxis;
	AcGeVector3d v2 = -1*AcGeVector3d::kYAxis;
	AcGePoint3d origin2 = AcGePoint3d::kOrigin + v1*cs_link.m_width*-0.5 + v2*cs_link.m_height*-0.5;
	for(int i=0;i<n1;i++)
	{
		//����������
		AcGePoint3d pt = origin + cs_link.m_headV*gap*(i+1);
		for(int j=0;j<n2;j++)
		{
			//����������
			pt += cs_link.m_dipV*gap;
			pts.append(pt);
			drawPts.append(origin2+v1*gap*(i+1)+v2*gap*(j+1));
		}
	}
	return true;
}

bool RcuHelper::GetDrillSiteInsertPt( const AcDbObjectId& drill_site, AcGePoint3d& insertPt )
{
	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return false;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, drill_site, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	DrillSite* pDS = DrillSite::cast( pObj );
	if(pDS == 0)
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	insertPt = pDS->getInsertPt();

	actrTransactionManager->endTransaction();
	return true;
}


bool RcuHelper::SetCoalSurfParams( const AcDbObjectId& coal_surf, AcGePoint3d& insertPt )
{
	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return false;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, coal_surf, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	CoalSurface* pCS = CoalSurface::cast( pObj );
	if(pCS == 0)
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	pCS->setInsertPt(insertPt);
	//pCS->setHeight(h);
	//pCS->setWidth(w);
	actrTransactionManager->endTransaction();
	return true;
}

bool RcuHelper::GetCoalSurfInsertPt( const AcDbObjectId& coal_surf, AcGePoint3d& insertPt )
{
	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return false;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, coal_surf, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	CoalSurface* pCS = CoalSurface::cast( pObj );
	if(pCS == 0)
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	insertPt = pCS->getInsertPt();

	actrTransactionManager->endTransaction();
	return true;
}

bool RcuHelper::SetDrillSitePt(const AcDbObjectId& drill_site, const AcGePoint3d& insertPt)
{
	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return false;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, drill_site, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	DrillSite* pDS = DrillSite::cast( pObj );
	if(pDS == 0)
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	pDS->setInsertPt(insertPt);

	actrTransactionManager->endTransaction();
	return true;
}

bool RcuHelper::GetDrillSitePt(const AcDbObjectId& drill_site, AcGePoint3d& insertPt)
{
	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return false;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, drill_site, AcDb::kForRead ) )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	DrillSite* pDS = DrillSite::cast( pObj );
	if(pDS == 0)
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	insertPt = pDS->getInsertPt();

	actrTransactionManager->endTransaction();
	return true;
}

bool RcuHelper::CreateDrillSite(const AcGePoint3d& pt, DrillSiteLink& ds_link, CoalSurfaceLink& cs_link)
{
	//����ú�����
	if(!RcuHelper::CaculCoalSurfParam(ds_link, cs_link)) return false;

	//�ĵ����л�
	DocumentLockSwitch lock_switch;

	//�½��곡�����ò��������
	DrillSite* pDS = new DrillSite();
	pDS->setInsertPt(pt);
	//pDS->enableFollow(true); // ��������Ч��

	CoalSurface* pCS = new CoalSurface();
	//pCS->setInsertPt(pt + (cnt - origin));
	cs_link.m_GEwidth = cs_link.m_width;
	cs_link.m_GEheight = cs_link.m_height;
	//cs_link.m_GEwidth = ds_link.m_width + ds_link.m_left + ds_link.m_right;
	//cs_link.m_GEheight = ds_link.m_height + ds_link.m_top + ds_link.m_bottom;
	cs_link.updateData(true);
	pCS->setInsertPt(pt + AcGeVector3d(0,1,0)*(cs_link.m_GEheight*0.5+ds_link.m_height*1.618));
	//pCS->setWidth(w);
	//pCS->setHeight(h);
	//pCS->enableFollow(true); // ��������Ч��

	//����곡��cadͼ�����ݿ�
	if(!ArxUtilHelper::PostToModelSpace(pDS))
	{
		delete pDS; pDS = 0;
		return  false;
	}

	//ú��������곡
	pCS->setRelatedGE(pDS->objectId());
	//���ú�㵽cadͼ�����ݿ�
	if(!ArxUtilHelper::PostToModelSpace(pCS))
	{
		delete pCS; pCS = 0;
		//ú���ύʧ��,��ɾ������ӵ��곡
		ArxEntityHelper::EraseObject(pDS->objectId(), true);
		return false;
	}

	//ͨ��DataLink�޸��곡��ú�������
	ds_link.setDataSource(pDS->objectId());
	ds_link.updateData(true);
	cs_link.setDataSource(pCS->objectId());
	cs_link.updateData(true);

	//���ú�����(�տ�)
	if(!RcuHelper::CreateClosePores(pCS->objectId(), cs_link, ds_link)) return false;

	//����ú������׵�
	return true;
}


bool RcuHelper::ModifyCoalSurfParams(const AcDbObjectId& drill_site)
{
	if(drill_site.isNull()) return false;

	AcDbObjectIdArray coal_surfs;
	DrawHelper::GetTagGEById2(drill_site, _T("CoalSurface"),coal_surfs);
	if(coal_surfs.isEmpty()) return false;
	AcDbObjectId coal_surf = coal_surfs[0];

	//��ȡ�곡����
	DrillSiteLink ds_link;
	if(!RcuHelper::ReadDrillSiteData(drill_site, ds_link)) return false;

	//��ȡú������
	CoalSurfaceLink cs_link;
	if(!RcuHelper::ReadCoalSurfaceData(coal_surf,cs_link)) return false;

	//���¼���ú�����
	if(!RcuHelper::CaculCoalSurfParam(ds_link, cs_link)) return false;

	//��ȡ�곡�Ĳ��������
	AcGePoint3d pt;
	if(!RcuHelper::GetDrillSiteInsertPt(drill_site, pt)) return false;
	
	cs_link.m_GEwidth = cs_link.m_width;
	cs_link.m_GEheight = cs_link.m_height;
	//cs_link.m_GEwidth = ds_link.m_width + ds_link.m_left + ds_link.m_right;
	//cs_link.m_GEheight = ds_link.m_height + ds_link.m_top + ds_link.m_bottom;
	cs_link.updateData(true);

	AcGePoint3d insertPt = pt + AcGeVector3d(0,1,0)*(cs_link.m_GEheight*0.5+ds_link.m_height*1.618);

	if(!RcuHelper::SetCoalSurfParams(coal_surf,insertPt)) return false;
	// 	if(!RcuHelper::CaculDrillSitePt(ds_link, ds_link, pt, insertPt, linkPt)) return false;
	CString value;
	DataHelper::GetPropertyData(coal_surf, _T("$���θ߶�"), value);

	return true;
	//�޸��곡ͼԪ�ļ��ε�����
	//return RcuHelper::SetDrillSitePt(drill_site, insertPt);
}

bool RcuHelper::CreateOpenPores(const AcDbObjectId& drill_site, CoalSurfaceLink& cs_link, DrillSiteLink& ds_link)
{
	//��ȡ�곡�ļ�������
	AcGePoint3d insertPt;
	if(!RcuHelper::GetDrillSitePt(drill_site, insertPt)) return false;

	//�����������
	AcGePoint3dArray pts;
	AcGePoint3dArray drawPts;
	if(!RcuHelper::CaculRelativeOpenPorePts(cs_link, ds_link, pts, drawPts)) return false;

	//acutPrintf(ArxUtilHelper::Point3dToString(ds_link.m_pt)+_T("\n"));
	//�½��곡�����ò��������
	for(int i = 0;i < pts.length(); i++)
	{
		//���µ�������
		//AcGePoint3d pt = pts[i];

		Pore* pPore = new Pore();
		//����xozƽ��,����Ҫ�ѵ�pt��y��z����
		//AcGePoint3d pt2 = pt;
		//std::swap(pt2.y, pt2.z);
		pPore->setInsertPt(insertPt + drawPts[i].asVector());
		pPore->setRelatedGE(drill_site);

		//����곡��cadͼ�����ݿ�
		if(!ArxUtilHelper::PostToModelSpace(pPore))
		{
			delete pPore; pPore = 0;
			continue;
		}
		else
		{
			PoreLink pore_link;
			pore_link.setDataSource(pPore->objectId());
			pore_link.m_pore_num = i + 1;
			pore_link.m_pore_size = ds_link.m_pore_size;
			//����Ҫ���곡�ĵװ����ĵ���ʵ���������׵���ʵ����
			pore_link.m_pt = ds_link.m_pt + pts[i].asVector();
			//acutPrintf(ArxUtilHelper::Point3dToString(pore_link.m_pt)+_T(" "));
			pore_link.updateData(true);
		}
	}

	return true;
}

bool RcuHelper::CreateClosePores(const AcDbObjectId& coal_surf, CoalSurfaceLink& cs_link, DrillSiteLink& ds_link)
{
	//����ú������������
	AcGePoint3dArray pts;
	AcGePoint3dArray drawPts;
	if(!RcuHelper::CaculRelativeClosePorePts(cs_link, pts, drawPts)) return false;

	//��ȡú��Ĳ��������
	AcGePoint3d insertPt;
	if(!RcuHelper::GetCoalSurfInsertPt(coal_surf, insertPt)) return false;

	//�½��곡�����ò��������
	for(int i=0;i<pts.length();i++)
	{
		//���µ�������

		Pore* pPore = new Pore();
		////����xoyƽ��
		//AcGeVector3d v = pt.asVector();
		//AcGeVector3d normalYV(AcGeVector3d::kYAxis);
		//v = v.orthoProject(normalYV);
		//std::swap(v.y, v.z);
		pPore->setInsertPt(insertPt + drawPts[i].asVector());
		pPore->setRelatedGE(coal_surf);

		//����곡��cadͼ�����ݿ�
		if(!ArxUtilHelper::PostToModelSpace(pPore))
		{
			delete pPore; pPore = 0;
			continue;
		}
		else
		{
			PoreLink pore_link;
			pore_link.setDataSource(pPore->objectId());
			pore_link.m_pore_num = i + 1;
			pore_link.m_pore_size = ds_link.m_pore_size;
			pore_link.m_pt = cs_link.m_pt + pts[i].asVector();
			pore_link.updateData(true);
		}
	}

	return true;
}

bool RcuHelper::ModifyDrillSitePoreGEs( const AcDbObjectId& drill_site, CoalSurfaceLink& cs_link, DrillSiteLink& ds_link )
{
	return CreateOpenPores(drill_site, cs_link, ds_link);
}

bool RcuHelper::ModifyCoalSurfRelatedGEs(const AcDbObjectId& coal_surf, CoalSurfaceLink& cs_link, DrillSiteLink& ds_link)
{
	return CreateClosePores(coal_surf, cs_link,ds_link);
	//return true;
}

bool RcuHelper::ModifyDrillSiteRelatedGEs(const AcDbObjectId& drill_site, CoalSurfaceLink& cs_link, DrillSiteLink& ds_link)
{
	//�����곡������ú��
	AcDbObjectId coal_surf;
	RcuHelper::GetRelatedCoalSurface(drill_site, coal_surf);
	if(coal_surf.isNull()) return false;

	if(!drill_site.isNull()) 
	{
		ClearRelatedOpenPores(drill_site);
		ClearRelatedClosePores(coal_surf);
	}

	//���¼���ú�����
	if(!RcuHelper::CaculCoalSurfParam(ds_link, cs_link)) return false;
	//���¼����ú�㷶Χ
	cs_link.m_GEwidth = cs_link.m_width;
	cs_link.m_GEheight = cs_link.m_height;
	//�������ݵ�ú��ͼԪ��
	if(!cs_link.updateData(true)) return false;

	//�޸�ú�������ͼԪ
	if(!ModifyCoalSurfRelatedGEs(coal_surf, cs_link, ds_link)) return false;

	ds_link.setDataSource(drill_site);
	ds_link.updateData(false);

	//�������ݵ��곡ͼԪ��
	if(ds_link.updateData(true))
	{
		//�޸��곡������ͼԪ
		ModifyDrillSitePoreGEs(drill_site, cs_link, ds_link);
	}

	return true;
}

void RcuHelper::CaculPoreNums( int& row_num, int& col_num, double width, double height, double gap )
{
	//��ɷ�Χ�Ŀ���ϲ�����׸���
	row_num = floor(width/gap);
	//��ɷ�Χ�ĸ߶��ϲ�����׸���
	col_num = floor(height/gap);
}

static void GetDocPath( CString& defaultPath )
{
	TCHAR pPath[MAX_PATH]={0};
	SHGetSpecialFolderPath(NULL,pPath,CSIDL_PERSONAL,0);

	defaultPath.Format(_T("%s"),pPath);
}

static BOOL SaveAndOpenReport(CString outName,const AcDbObjectId& drill_site)
{
	TCHAR szFileFilter[] = _T("doc�ĵ�(*.doc)|*.doc||");
	TCHAR szFileExt[] = _T("doc");

	CString defaultPath;
	GetDocPath(defaultPath);

	CFileDialog dlg(FALSE,szFileExt,defaultPath,OFN_READONLY,szFileFilter);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���

	TCHAR* pFileBuf = new TCHAR[MAX_PATH * 1024];
	if (NULL == pFileBuf)
	{
		return FALSE;
	}
	_tcscpy(pFileBuf,outName);

	dlg.m_ofn.lpstrFile = pFileBuf;
	dlg.m_ofn.nMaxFile = MAX_PATH * 1024;
	CString selectedPath;
	if(IDOK == dlg.DoModal())
	{
		selectedPath = dlg.GetPathName();
	}

	else
	{
		return FALSE;
	}

	delete pFileBuf;
	pFileBuf = NULL;

	acutPrintf( _T( "\n����������...\n" ) );
	//��ʼ��com
	if(initword())
	{
		if(!CreatReport(selectedPath,drill_site)) 
		{
			//ж��com
			uninitword();
			acutPrintf( _T( "\n��������ʧ��!\n" ) );
			return FALSE;
		}
		//ж��com
		uninitword();
	}
	acutPrintf( _T( "\n���汣�浽:%s" ),selectedPath);
	if(IDYES == AfxMessageBox(_T("�������ɳɹ�!�Ƿ����ڴ�?"),MB_YESNO))
	{
		if(initword())
		{
			OpenWordDocument(selectedPath);
			//ж��com
			uninitword();
		}
	}
	return TRUE;
}

void RcuHelper::CreatReportHelper(const AcDbObjectId& drill_site)
{
	CAcModuleResourceOverride myResources;

	TCHAR szFileFilter[] = _T("doc�ĵ�(*.doc)|*.doc||");
	TCHAR szFileExt[] = _T("doc");
	CString defaultPath;
	GetDocPath(defaultPath);
	CString outName;
	if(drill_site.isNull())
		outName = _T("ʯ����Ʊ���");
	else
	{
		//��ȡ�곡����
		DrillSiteLink ds_link;
		if(!RcuHelper::ReadDrillSiteData(drill_site, ds_link)) return;
		outName.Format(_T("%s�곡--ʯ����Ʊ���"),ds_link.m_name);
	}
	SaveAndOpenReport(outName,drill_site);
}
