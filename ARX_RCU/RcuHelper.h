#pragma once

#include "RcuDataLink.h"

class RcuHelper
{
public:
	//查找所有的钻场
	static void FindAllDrillSite(AcDbObjectIdArray& drill_sites);
	//查找与钻场关联的煤层
	static void GetRelatedCoalSurface(const AcDbObjectId& drill_site, AcDbObjectId& coal_surf);
	////查找与钻场关联的钻场
	//static void GetRelatedDrillSites(const AcDbObjectId& drill_site, AcDbObjectIdArray& drill_sites);
	//查找与煤层关联的终孔
	static void GetRelatedClosePores(const AcDbObjectId& coal_surf, AcDbObjectIdArray& pores);
	//查找与钻场关联的开孔
	static void GetRelatedOpenPores(const AcDbObjectId& drill_site, AcDbObjectIdArray& pores);
	//删除与钻场关联的所有钻孔
	static void ClearRelatedOpenPores(const AcDbObjectId& drill_site);
	//删除与煤层关联的所有钻孔
	static void ClearRelatedClosePores(const AcDbObjectId& coal_surf);

	//从煤层中提取数据
	static bool ReadCoalSurfaceData(const AcDbObjectId& coal_surf, CoalSurfaceLink& cs_link);
	//从钻场提取数据
	static bool ReadDrillSiteData(const AcDbObjectId& drill_site, DrillSiteLink& ds_link);
	//从钻孔提取数据
	static bool ReadPoreData(const AcDbObjectId& pore, PoreLink& po_link);

	//得到钻场以及关联煤层的数据
	static bool GetDrillSiteAndCoalSurfDatas(const AcDbObjectId& drill_site, DrillSiteLink& ds_link, CoalSurfaceLink& cs_link);
	//得到钻场的数据
	static bool GetDrillSiteData(const AcDbObjectId& drill_site, DrillSiteLink& ds_link);

	//向量的偏角(与Y轴的夹角，有正负)和仰角(与水平面的夹角，有正负)
	static void VectorToAngle(const AcGeVector3d& v, double& ang1, double& ang2);
	//测试VectorToAngle函数
	static void VectorToAngleTest();

	//计算煤层面参数
	static bool CaculCoalSurfParam(DrillSiteLink& ds_link, CoalSurfaceLink& cs_link);
	//计算钻场的钻孔相对坐标(基点是钻场底帮点)
	static bool CaculRelativeOpenPorePts(CoalSurfaceLink& cs_link,DrillSiteLink& ds_link,AcGePoint3dArray& pts,AcGePoint3dArray& drawPts);
	//计算煤层的钻孔相对坐标(基点是煤层面的中心点)
	static bool CaculRelativeClosePorePts(CoalSurfaceLink& cs_link,AcGePoint3dArray& pts,AcGePoint3dArray& drawPts);

	//创建钻场和煤层图元(rg_link和cs_link的数据会与新增的图元关联，并被更新到图元)
	static bool CreateDrillSite(const AcGePoint3d& pt, DrillSiteLink& ds_link, CoalSurfaceLink& cs_link);
	//创建与钻场关联的钻孔图元(开孔)
	static bool CreateOpenPores(const AcDbObjectId& drill_site, CoalSurfaceLink& cs_link, DrillSiteLink& ds_link);
	//创建与煤层关联的钻孔图元(终孔)
	static bool CreateClosePores(const AcDbObjectId& coal_surf, CoalSurfaceLink& cs_link, DrillSiteLink& ds_link);

	//根据钻场数据修改关联的图元(开孔)
	static bool ModifyDrillSitePoreGEs(const AcDbObjectId& drill_site, CoalSurfaceLink& cs_link, DrillSiteLink& ds_link);
	//根据钻场数据修改关联的图元(煤层、钻孔)
	static bool ModifyDrillSiteRelatedGEs(const AcDbObjectId& drill_site,CoalSurfaceLink& cs_link, DrillSiteLink& ds_link);
	//根据煤层数据修改关联的图元(终孔)
	static bool ModifyCoalSurfRelatedGEs(const AcDbObjectId& coal_surf, CoalSurfaceLink& cs_link, DrillSiteLink& ds_link);

	//修改钻场参数
	static bool ModifyCoalSurfParams(const AcDbObjectId& drill_site);
	//获取钻场的插入点坐标
	static bool GetDrillSiteInsertPt(const AcDbObjectId& drill_site, AcGePoint3d& insertPt);
	//获取煤层的插入点坐标
	static bool GetCoalSurfInsertPt(const AcDbObjectId& coal_surf, AcGePoint3d& insertPt);
	//获取煤层的插入点坐标
	static bool SetCoalSurfParams(const AcDbObjectId& coal_surf, AcGePoint3d& insertPt);
	//设置钻场图元的2点坐标
	static bool SetDrillSitePt(const AcDbObjectId& drill_site, const AcGePoint3d& insertPt);
	//获取钻场的2点坐标
	static bool GetDrillSitePt(const AcDbObjectId& drill_site, AcGePoint3d& insertPt);

	//计算钻孔个数
	static void CaculPoreNums(int& row_num, int& col_num, double width, double height,double gap);

	//生成报告
	static void CreatReportHelper(const AcDbObjectId& drill_site);
};