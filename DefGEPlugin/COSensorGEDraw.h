#pragma once

#include "../MineGEDraw/MineGEDraw.h"

class COSensorGEDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(COSensorGEDraw);

public:
	COSensorGEDraw();
	~COSensorGEDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void regPropertyDataNames(AcStringArray& names) const;
	virtual void readPropertyDataFromGE(const AcStringArray& values);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	//virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts);


protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subGetGripPoints( AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds ) const;

	virtual Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

private:
	// key param
	AcGePoint3d m_insertPt;   // ���������
	double m_angle;           // ����Ƕȣ���ʱ�룬��λ���ȣ�

	// extra param
	double m_radius;          // Բ�İ뾶
	CString m_temp;
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(COSensorGEDraw)
#endif