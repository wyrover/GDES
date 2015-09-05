#include "StdAfx.h"
#include "SimpleDrillSiteDraw.h"
#include "DrawTool.h"
#include "DrawSpecial.h"

#include "../ArxHelper/HelperClass.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleDrillSiteDraw, MineGEDraw, 1 )

SimpleDrillSiteDraw::SimpleDrillSiteDraw () : MineGEDraw ()
{
}

SimpleDrillSiteDraw::~SimpleDrillSiteDraw ()
{
}

void SimpleDrillSiteDraw::setAllExtraParamsToDefault()
{
}

void SimpleDrillSiteDraw::configExtraParams()
{

}
void SimpleDrillSiteDraw::updateExtraParams()
{
}

void SimpleDrillSiteDraw::readKeyParam(DrawParamReader& reader)
{
	reader.readPoint(m_insertPt);
	//reader.readPoint(m_linkPt);
}

void SimpleDrillSiteDraw::writeKeyParam(DrawParamWriter& writer)
{
	writer.writePoint(m_insertPt);
	//writer.writePoint(m_linkPt);
}

void SimpleDrillSiteDraw::readExtraParam( DrawParamReader& reader )
{
    //MineGEDraw::readExtraParam( reader );
}

void SimpleDrillSiteDraw::writeExtraParam( DrawParamWriter& writer )
{
    //MineGEDraw::writeExtraParam( writer );
}

void SimpleDrillSiteDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "����" ) );
    names.append( _T( "���" ) );
	names.append( _T( "�߶�" ) );
}

void SimpleDrillSiteDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    m_name = values[0].kACharPtr();
	m_width = abs(_tstof(values[1].kACharPtr()));
	m_height = abs(_tstof(values[2].kACharPtr()));
}

Adesk::Boolean SimpleDrillSiteDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

	//�������С��1m���򲻻����곡������
	//if(m_dist > 1.0)
	//{
		//DrawLine(mode, m_insertPt, m_linkPt);
	//}

	//AcGeVector3d v = m_linkPt - m_insertPt;

	////����v��x���ϵ�ͶӰ
	//AcGeVector3d v1 = v.dotProduct( AcGeVector3d::kXAxis ) * AcGeVector3d::kXAxis;
	//v1.normalize();
	//if(v1.y < 0) v1.negate();

	////����v��y���ϵ�ͶӰ
	//AcGeVector3d v2 = v.dotProduct( AcGeVector3d::kYAxis ) * AcGeVector3d::kYAxis;
	//v2.normalize();
	//if(v2.y < 0) v2.negate();

	////����ε����ĵ�����
	//AcGePoint3d pt = m_linkPt;
	//pt += v2*m_height*0.5;

	//����Ƕ�
	//double angle = v1.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis);
	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( PI * 0.5, AcGeVector3d::kZAxis );
	v.normalize();
	AcGePoint3d pt = m_insertPt + 0.5*m_height * v;
	//���ƾ���
	DrawRect(mode, pt, 0, m_width, m_height, false);

    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleDrillSiteDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

	gripPoints.append(m_insertPt);
	//gripPoints.append(m_linkPt);

    return Acad::eOk;
}

Acad::ErrorStatus SimpleDrillSiteDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

	for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );

        if ( idx == 0 )
        {
			m_insertPt += offset;
			//m_linkPt += offset;
        }

        if ( idx == 1 )
        {
          // m_insertPt += offset;
        }
    }

    return Acad::eOk;
}

Acad::ErrorStatus SimpleDrillSiteDraw::subTransformBy( const AcGeMatrix3d& xform )
{
	m_insertPt.transformBy(xform);
	//m_linkPt.transformBy(xform);
	return Acad::eOk;
}

Acad::ErrorStatus SimpleDrillSiteDraw::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d& pickPoint,
	const AcGePoint3d& lastPoint,
	const AcGeMatrix3d& viewXform,
	AcGePoint3dArray& snapPoints,
	AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;
	// ֻ��׽1�����͵ĵ㣺�˵�
	if( osnapMode != AcDb::kOsMaskEnd ) return Acad::eOk;

	if( osnapMode == AcDb::kOsMaskEnd )
	{
		snapPoints.append(m_insertPt);
	}

	return Acad::eOk;
}