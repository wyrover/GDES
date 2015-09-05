#include "StdAfx.h"
#include "COSensorGEDraw.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

ACRX_CONS_DEFINE_MEMBERS ( COSensorGEDraw, MineGEDraw, 1 )

COSensorGEDraw::COSensorGEDraw()
{

}

COSensorGEDraw::~COSensorGEDraw( void )
{
}

void COSensorGEDraw::setAllExtraParamsToDefault()
{
	m_radius = 3;
	m_temp = _T("");
}

void COSensorGEDraw::configExtraParams()
{

}

void COSensorGEDraw::updateExtraParams()
{

}

void COSensorGEDraw::writeKeyParam( DrawParamWriter& writer )
{
	writer.writePoint( m_insertPt );
	writer.writeDouble( m_angle );
}

void COSensorGEDraw::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_insertPt );
	reader.readDouble( m_angle );
}

void COSensorGEDraw::readExtraParam( DrawParamReader& reader )
{
	reader.readDouble( m_radius );
	reader.readString( m_temp );
}

void COSensorGEDraw::writeExtraParam( DrawParamWriter& writer )
{
	writer.writeDouble( m_radius );
	writer.writeString( m_temp );
}

Adesk::Boolean COSensorGEDraw::subWorldDraw( AcGiWorldDraw* mode )
{
	assertReadEnabled () ;

	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( m_angle, AcGeVector3d::kZAxis );

	if( v.x < 0 ) v.negate();
	m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

	DrawCircle( mode, m_insertPt, m_radius, false );
	v.rotateBy( PI / 2 , AcGeVector3d::kZAxis );
	v.normalize();

	AcGePoint3d pt = m_insertPt;
	if(!LESS_ZERO(m_temp))
	{
		pt += 0.2 * m_radius * v;
	}

	DrawMText(mode,pt,m_angle,_T("Y"),0.6 * m_radius);

	v.rotateBy( PI,AcGeVector3d::kZAxis );
	pt = m_insertPt + 0.5 * m_radius * v;

	if(LESS_ZERO(m_temp)) return Adesk::kTrue;
	CString temp;
	DealSpPoints(m_temp);
	temp.Format(_T("%s%s"),m_temp,_T("%"));
	DrawMText(mode,pt,m_angle, temp ,0.2 * m_radius);

	return Adesk::kTrue;
}

Acad::ErrorStatus COSensorGEDraw::subTransformBy( const AcGeMatrix3d& xform )
{
	// �����任
	//m_insertPt.transformBy( xform );
	//m_pt.transformBy( xform );
	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( m_angle, AcGeVector3d::kZAxis );
	v.transformBy( xform );
	//m_radius = v.length();

	// 1) ����һ��Բ
	AcDbCircle circle(m_insertPt, AcGeVector3d::kZAxis, m_radius);
	// 2) Բ����transformBy()�������б任
	circle.transformBy(xform);
	//3) ��ȡ���º�Ĳ���
	m_insertPt = circle.center();     // ��ȡ�任���Բ������
	m_radius = circle.radius(); // ��ȡ�任���Բ�뾶
	m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis ); // �任�����ת�Ƕ�

	return Acad::eOk;
}


Acad::ErrorStatus COSensorGEDraw::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d& pickPoint,
	const AcGePoint3d& lastPoint,
	const AcGeMatrix3d& viewXform,
	AcGePoint3dArray& snapPoints,
	AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;
	if( osnapMode != AcDb::kOsModeCen )
		return Acad::eOk;

	Acad::ErrorStatus es = Acad::eOk;

	if( osnapMode == AcDb::kOsModeCen )
	{
		snapPoints.append( m_insertPt );
	}

	return es;
}

Acad::ErrorStatus COSensorGEDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;

	gripPoints.append( m_insertPt );

	AcGeVector3d v(AcGeVector3d::kXAxis);
	v.rotateBy(m_angle,AcGeVector3d::kZAxis);
	v *= m_radius;

	gripPoints.append(m_insertPt + v);             // ������˵���Ϊ�е�

	return Acad::eOk;
}

Acad::ErrorStatus COSensorGEDraw::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
	assertWriteEnabled () ;

	int len = indices.length();
	for(int i=0;i<len;i++)
	{
		int idx = indices.at(i);
		if(idx == 0)
		{
			// ��ǰ�е���Բ�ģ��ƶ�ͼ��
			m_insertPt += offset;       // ��Բ��ִ��ƫ�Ʊ任
		}
		if(idx == 1)
		{
			// ����Բ
			// 1) ����x��Ķ˵�����
			AcGeVector3d v(AcGeVector3d::kXAxis);
			v.rotateBy(m_angle,AcGeVector3d::kZAxis);
			AcGePoint3d pt = m_insertPt + v*m_radius;

			// 2) ��������ƫ�Ƽ���
			pt += offset;

			// 3) ������������Բ��֮��ĳ��ȣ�����ΪԲ�뾶
			// ����������õ�һ��������Ȼ��õ���������
			m_radius = (pt - m_insertPt).length();
		}
	}

	return Acad::eOk;
}

void COSensorGEDraw::regPropertyDataNames( AcStringArray& names ) const
{
	names.append(_T("CO"));
}

void COSensorGEDraw::readPropertyDataFromGE( const AcStringArray& values )
{
	CString strLenth;
	strLenth.Format(_T("%s"),values[0].kACharPtr());
	m_temp = strLenth;
	//m_radius = _tstof(strLenth);
	//if( 0 >= m_radius ) m_radius = 6;
}