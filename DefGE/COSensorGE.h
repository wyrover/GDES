#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// CO´«¸ÐÆ÷
class DEFGE_EXPORT_API COSensorGE : public DirectionTagGE {

public:
	ACRX_DECLARE_MEMBERS(COSensorGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	COSensorGE() ;
	COSensorGE(const AcGePoint3d& insertPt, double angle);
	virtual ~COSensorGE();

private:
	virtual void writePropertyDataToGEDraw( MineGEDraw* pGEDraw ) const;
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(COSensorGE)
#endif