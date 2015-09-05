#pragma once

#include "../MineGE/DataLink.h"

//4��ʯ�Ž�úͼԪ���ݹ���,���ٲ���Ҫ���ַ����������������͵�ת������
class CoalSurfaceLink : public DataLink
{
public:
	CoalSurfaceLink() : m_thick(0), m_angle(0), m_width(0), m_height(0), m_GEwidth(0),m_GEheight(0),
		                m_gas_radius(3)
	{

	}
protected:
	virtual void regDatas()
	{
		linkDoubleData( _T( "���" ), &m_thick );
		linkDoubleData( _T( "���" ), &m_angle );
		linkDoubleData( _T( "��׳�ɰ뾶" ), &m_gas_radius );
		//��$�ŵ��ֶα�ʾ���ֶν������ڲ�����,��Ӧ��Ҳ����Ҫ��ʾ�ڶԻ��������
		linkDoubleData( _T( "$��ɿ��" ), &m_width );
		linkDoubleData( _T( "$��ɸ߶�" ), &m_height );
		linkPointData(_T("$���ĵ�����"), &m_pt);
		linkVectorData(_T("$ƽ�淨����"), &m_normV);
		linkVectorData(_T("$ú����������"), &m_headV);
		linkVectorData(_T("$ú����������"), &m_dipV);
		linkDoubleData( _T( "$���ο��" ), &m_GEwidth );
		linkDoubleData( _T( "$���θ߶�" ), &m_GEheight );
	}

public:
	double m_thick;
	double m_angle;
	double m_width;
	double m_height;
	//�����ڲ��洢�����ַ�����ʾ��,xyz֮��ͨ�����ŷָ�
	AcGePoint3d m_pt;
	AcGeVector3d m_normV;
	AcGeVector3d m_headV;
	AcGeVector3d m_dipV;
	double m_gas_radius;
	double m_GEwidth;
	double m_GEheight;

};

class DrillSiteLink : public DataLink
{
public:
	DrillSiteLink() : m_dist(0), m_width(0), m_height(0), 
					  m_left(0), m_right(0), m_top(0), m_bottom(0),
					  m_pore_size(0)
	{

	}
protected:
	virtual void regDatas()
	{
		linkStringData( _T( "����" ), &m_name );
		linkDoubleData( _T( "���" ), &m_width );
		linkDoubleData( _T( "�߶�" ), &m_height );
		linkDoubleData( _T( "����" ), &m_dist );
		linkPointData( _T( "ӭͷ�װ����ĵ�����" ), &m_pt );
		linkDoubleData( _T( "��ﱣ��" ), &m_left );
		linkDoubleData( _T( "�Ұﱣ��" ), &m_right );
		linkDoubleData( _T( "�ϰﱣ��" ), &m_top );
		linkDoubleData( _T( "�°ﱣ��" ), &m_bottom );
		linkDoubleData( _T( "�׾�" ), &m_pore_size );
		//��$�ŵ��ֶα�ʾ���ֶν������ڲ�����,��Ӧ��Ҳ����Ҫ��ʾ�ڶԻ��������
		linkPointData( _T( "$�װ�����" ), &m_pt );
	}

public:
	CString m_name;
	double m_dist;
	double m_width;
	double m_height;
	//�����ڲ��洢�����ַ�����ʾ��,xyz֮��ͨ�����ŷָ�
	AcGePoint3d m_pt;
	double m_pore_size;
	double m_left;
	double m_right;
	double m_top;
	double m_bottom;
};

class PoreLink : public DataLink
{
public:
	PoreLink() : m_pore_num(0), m_pore_size(0)
	{

	}
protected:
	virtual void regDatas()
	{
		linkIntData( _T( "���" ), &m_pore_num );
		linkDoubleData( _T( "�׾�" ), &m_pore_size );
		//��$�ŵ��ֶα�ʾ���ֶν������ڲ�����,��Ӧ��Ҳ����Ҫ��ʾ�ڶԻ��������
		linkPointData( _T( "$����" ), &m_pt );
	}

public:
	int m_pore_num;
	double m_pore_size;
	//���������ַ�����ʾ��,xyz֮��ͨ�����ŷָ�
	//��ͨ��ArxUtilHelper::StringToPoint3d��̬��������ת��
	AcGePoint3d m_pt;
};