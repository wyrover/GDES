#pragma once

#include <math.h>

#define PI 3.1415926535897932384626433832795
//����ת����
#define DegToRad(x) x*0.01745329251994329576923690768489
//����ת����
#define RadToDeg(x) x*57.295779513082320876798154814105
//ƽ��
#define square(x)  x*x
//����
#define cot(x) 1.0/tan(x)

//��һ���򻯰��ʯ�Ž�ú������
//ע:�Ƕ�Ĭ��ʹ�û���
class Rcu2
{
public:
	//���캯��
	Rcu2() : beta(PI*0.5) {}

	//����ú�����
	void setCoalSurf(double alpha, double thick)
	{
		this->alpha = alpha;
		//this->beta = beta;
		this->thick = thick;
	}
	//�����곡����������
	void setDrillSite1(double height, double width)
	{
		this->height = height;
		this->width = width;
	}
	//�����곡���������Ұﱣ�����
	void setDrillSite2(double f1, double f2, double d1, double d2)
	{
		this->f1 = f1;
		this->f2 = f2;
		this->d1 = d1;
		this->d2 = d2;
	}
	//�����곡��ú����Ĵ���
	void setDrillSite3(double minDist)
	{
		this->minDist = minDist;
	}
	//�������ֱ��
	void setDrillDiameter(double d0)
	{
		this->d0 = d0;
	}

	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////

	/**
	 * ����ú����ĳ�ɷ�Χ
	 * �������Ǽ���ӭͷ��ú��������Ǵ�ֱ��
	 * ��ɷ�Χ�Ŀ�ȵ������ұ�������������Ŀ��
	 * ��ɷ�Χ�ĸ߶ȼ�������΢�鷳һЩ
	 */
	void drillExtent(double& Lw, double& Lh)
	{
		//��ɷ�Χ���
		Lw = width + d1 + d2;
		//��ɷ�Χ�߶�
		Lh = (height + f1 + f2)/sin(alpha);
	}

	//ú����(��Ҫ4�����ݲ���ȷ��һ��ú��ƽ��)
	//ƽ���ϵ�һ��cnt, ƽ�淨����normV, ú�����������headV, ú�����������dipV
	//����3ά����ϵ��normV-Z, dipV-Y, headV-X
	bool drillSurface(AcGePoint3d& cnt, AcGeVector3d& normV, AcGeVector3d& headV, AcGeVector3d& dipV)
	{
		//����úƽ�淽��
		AcGePlane plane1, plane2;
		coalSurface(plane1, plane2);

		//��ɷ�Χ�����ĵ�����
		//��������ϰ�����°�Խǵ�,��2����е㼴Ϊ���ε����ĵ�
		AcGePoint3d left_top(0, 0, height+f1);
		AcGePoint3d right_bottom(0, 0, -1*f2);
		AcGeLine3d lt_ray(left_top, AcGeVector3d::kYAxis);
		AcGeLine3d rb_ray(right_bottom, AcGeVector3d::kYAxis);
		//����������ƽ��Ľ���
		AcGePoint3d p1, p2;
		if(Adesk::kTrue != plane1.intersectWith(lt_ray, p1)) return false;
		if(Adesk::kTrue != plane1.intersectWith(rb_ray, p2)) return false;

		//2��������е㼴Ϊ���ĵ�
		cnt = p1 + 0.5*(p2-p1);
		//ƽ�淨����
		normV = plane1.normal();
		//acutPrintf(_T("\n������(%.2f,%.2f,%.2f)"),normV.x,normV.y,normV.z);
		//����ú�����������
		AcGeVector3d v(-AcGeVector3d::kYAxis);
		v.rotateBy(-1*alpha, AcGeVector3d::kXAxis);
		dipV = v;
		//acutPrintf(_T("\n��������(%.2f,%.2f,%.2f)"),dipV.x,dipV.y,dipV.z);
		//����ú�����������
		v.rotateBy(-0.5*PI, normV);
		headV = v;
		//acutPrintf(_T("\n��������(%.2f,%.2f,%.2f)"),headV.x,headV.y,headV.z);

		return true;
	}

	//��׸��������
	void drillNumber(double Lw, double Lh, int& N1, int& N2, double& e1, double& e2)
	{
		N1 = int(Lw/d0)+2;
		N2 = int(Lh/d0)+2;
		e1 = Lw/(N1-1);
		e2 = Lh/(N2-1);
	}

private:
	//����úƽ�淽��(��ú��ֹú)
	void coalSurface(AcGePlane& plane1, AcGePlane& plane2)
	{
		//ӭͷú������߾���
		double L = minDist/sin(alpha);
		//acutPrintf(_T("\nӭͷú������߾���L=%.2f"),L);
		//ӭͷ������ú��Ľ���
		AcGePoint3d pt(0, L, 0);

		//����ƽ��ķ�����
		AcGeVector3d v(-AcGeVector3d::kYAxis);
		v.rotateBy(PI*0.5 - alpha, AcGeVector3d::kXAxis);

		//��ƽ���ϵĵ�ͷ�����ȷ��һ��ƽ��
		plane1.set(pt, v);
		//acutPrintf(_T("\n��ú���(%.2f,%.2f,%.2f),����(%.2f,%.2f,%.2f)"),
		//	pt.x,pt.y,pt.z,v.x,v.y,v.z);
		plane2.set(pt - v*thick, v);
		//pt = pt - v*thick;
		//acutPrintf(_T("\nֹú���(%.2f,%.2f,%.2f),����(%.2f,%.2f,%.2f)"),
		//	pt.x,pt.y,pt.z,v.x,v.y,v.z);

	}

private:
	double alpha;    // ú�����
	double beta;     // �곡������ú������ļн�(ʼ����90��)
	double thick;    // ú����

	double height;    // �곡����߶�
	double width;     // �곡������(�����곡Ϊ�������)
	
	double minDist;     // �곡����ú��Ĵ���
	double f1; // ʯ�Ž�ú�������Ͽؾ���
	double f2; // ʯ�Ž�ú�������¿ؾ���
	double d1; // ʯ�Ž�ú��������ؾ���
	double d2; // ʯ�Ž�ú�������ҿؾ���

	double d0;     // ���жѹֱ��
};
