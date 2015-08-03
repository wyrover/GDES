#include "StdAfx.h"
#include "COSensorGE.h"
#include "../MineGE/DataHelper.h"
#include "../MineGE/DataListHelper.h"

Adesk::UInt32 COSensorGE::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS ( COSensorGE, 
						 DirectionTagGE, AcDb::kDHL_CURRENT,
						 AcDb::kMReleaseCurrent,AcDbProxyEntity::kNoOperation,
						 CO´«¸ÐÆ÷, DEFGEAPP )

						 COSensorGE::COSensorGE ()
{
}

COSensorGE::COSensorGE( const AcGePoint3d& insertPt, double angle )
: DirectionTagGE( insertPt, angle )
{
}

COSensorGE::~COSensorGE ()
{
}

static void ReadWriteData( const AcDbObjectId& sObjId, const CString& sField, const AcDbObjectId& tObjId, const CString& tField )
{
	CString v;
	DataHelper::GetPropertyData( sObjId, sField, v );
	DataHelper::SetPropertyData( tObjId, tField, v );
}

void COSensorGE::writePropertyDataToGEDraw( MineGEDraw* pGEDraw ) const
{
	AcDbObjectId gID = getRelatedGE();
	AcDbObjectId objID = objectId();
	ReadWriteData(gID,_T("CO"),objID,_T("CO"));
	MineGE::writePropertyDataToGEDraw(pGEDraw);
}