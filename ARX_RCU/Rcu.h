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

//ʯ�Ž�ú������
//ע:�Ƕ�Ĭ��ʹ�û���
class Rcu
{
public:
	//���캯��
	Rcu() : origin(AcGePoint3d::kOrigin), beta(PI*0.5),
	        w1(0), w2(0), h1(0), h2(0),d0(0) {}

	//���û�������
	void setOrigin(const AcGePoint3d& origin)
	{
		this->origin = origin;
	}
	//��ȡ��������
	AcGePoint3d getOrigin() const
	{
		return this->origin;
	}
	//����ú�����
	void setCoalParams(double alpha, double beta, double h)
	{
		this->alpha = alpha;
		this->beta = beta;
		this->h = h;
	}
	//��ȡú�����
	void getCoalParams(double& alpha, double& beta, double& h) const
	{
		alpha = this->alpha;
		beta = this->beta;
		h = this->h;
	}
	//�����곡����������
	void setTunnelParams(double H, double W1, double W2)
	{
		this->H = H;
		this->W1 = W1;
		this->W2 = W2;
	}
	//��ȡ�곡����������
	void getTunnelParams(double& H, double& W1, double& W2) const
	{
		H = this->H;
		W1 = this->W1;
		W2 = this->W2;
	}
	//�����곡����
	void setDrillSiteParams(double S1,double f1, double f2, double d1, double d2)
	{
		this->S1 = S1;
		this->f1 = f1;
		this->f2 = f2;
		this->d1 = d1;
		this->d2 = d2;
	}
	//��ȡ�곡����
	void getDrillSiteParams(double& f1, double& f2, double& d1, double& d2) const
	{
		f1 = this->f1;
		f2 = this->f2;
		d1 = this->d1;
		d2 = this->d2;
	}
	//���������
	AcGePoint3d minNormalDistPoint() const
	{
		return origin + AcGeVector3d(W1*0.5, H, 0);
	}
	//����ӭͷ��ײ���
	void setHeadDrillParams(double w1, double w2)
	{
		this->w1 = w1;
		this->w2 = w2;
	}
	//�����������
	void setDrillMachineParams(double h1, double h2)
	{
		this->h1 = h1;
		this->h2 = h2;
	}
	//�������ֱ��
	void setDrillDiameter(double d0)
	{
		this->d0 = d0;
	}

	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////

	//�����úƽ�淽��
	void beginCoalSurface(AcGePlane& plane)
	{
		coalSurface(plane, 0);
	}
	//����ֹúƽ�淽��
	void endCoalSurface(AcGePlane& plane)
	{
		//��ú�����ú�����ֹú��
		coalSurface(plane, h);
	}
	//ֹú��Ŀ�͸�
	void drillExtent(double& Lw, double& Lh)
	{
		AcGePoint3d p12 = k2(1);
		AcGePoint3d p22 = k2(2);
		AcGePoint3d p32 = k2(3);

		Lw = sqrt(square(p12.x-p22.x) + square(p12.y-p22.y) + square(p12.z-p22.z));
		Lh = sqrt(square(p12.x-p32.x) + square(p12.y-p32.y) + square(p12.z-p32.z));
		acutPrintf(_T("\n(%.2lf,%.2lf,%.2lf)\t(%.2lf,%.2lf,%.2lf)\t(%.2lf,%.2lf,%.2lf)\t"),p12.x,p12.y,p12.z,p22.x,p22.y,p22.z,p32.x,p32.y,p32.z);
		acutPrintf(_T("\nLw:%.2lf\tLh:%.2lf\n"),Lw,Lh);
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
	//����
	double minNormalDist() const
	{
		using namespace std;
		//����F��(�������ú��������)
		return S1*sin(beta);
	}
	//���ֻ�����곡ˮƽ����delta����alpha(ú�����)
	double getDelta() const
	{
		return this->alpha;
	}

	double getPhi() const
	{
		using namespace std;
		return atan(abs(tan(getDelta()))*cos(beta));
	}

	void coalSurface(AcGePlane& plane, double h) const
	{
		using namespace std;
		double L = origin.z;
		double Lf = minNormalDist() + h;
		double D = Lf*sqrt(1+square(cot(getDelta())));
		D += L*cos(beta);
		D += H*cot(beta);
		D += 0.5*W1*sin(beta);

		double A = sin(beta);
		double B = -1*cot(getDelta());
		double C = -1.0*cos(beta);

		//����ƽ�����
		plane.set(A, B, C, D);
	}

	AcGePoint3d k0(int k) const
	{
		using namespace std;
		int c1 = (k%2==0)?1:-1;
		int c2 = ((1-k/3)%2==0)?1:-1;
		int c3 = ((k/3)%2==0)?1:-1;
		double x0 = c1*(W1*0.5 - w1*(1-c1))*0.5-w1*(1+c1*0.5);
		double y0 = h2*(1-c2)*0.5+h1*(1-c3)*0.5;
		double z0 = origin.z;
		return AcGePoint3d(x0,y0,z0);
	}

	AcGePoint3d k1(int k) const
	{
		using namespace std;
		double L = origin.z;
		double Lf = minNormalDist();
		int c1 = (k%2==0)?1:-1;
		int c2 = ((1-k/3)%2==0)?1:-1;
		int c3 = ((k/3)%2==0)?1:-1;
		double x1 = c1*(W2 + (1-c1)*d1 + (1+c1)*d2)*0.5 - c1*d0*cos(beta)*0.5;
		double y1 = (H + f1 - 0.5*d0*sin(getPhi()))*(1-c2)*0.5-(f2-0.5*d0*sin(getPhi()))*(1-c3);
		double z1 = (x1*sin(beta) - y1*cot(getDelta()) + Lf*sqrt(1+square(cot(getDelta()))) + L*cos(beta) + H*cot(getDelta()) - 0.5*W1*sin(beta)) / cos(beta);
		return AcGePoint3d(x1,y1,z1);
	}

	AcGePoint3d k2(int k) const
	{
		using namespace std;

		AcGePoint3d p0 = k0(k);
		AcGePoint3d p1 = k1(k);
		double Lf = minNormalDist();
		double L = origin.z;

		double a= p0.y*cot(getDelta()) + p0.z*cos(beta) -p0.x*sin(beta) - (Lf+h)*sqrt(1+square(cot(getDelta()))) - L*cos(beta) - H*cot(getDelta()) +0.5*W1*sin(beta);
		double b = (p1.x -p0.x)*sin(beta) - (p1.y - p0.y)*cot(getDelta()) - (p1.z-p0.z)*cos(beta);
		double tk = a/b;

		double x2 = (p0.x+tk*p1.x)/(1-tk);
		double y2 = (p0.y+tk*p1.y)/(1-tk);
		double z2 = (p0.z+tk*p1.z)/(1-tk);
		return AcGePoint3d(x2,y2,z2);
	}

private:
	AcGePoint3d origin; // �곡��������²����ĵ������

	double alpha; // ú�����
	double beta; // �곡������ú������ļн�
	double h;    // ú����

	double H;    // �곡����߶�
	double W1, W2; // �곡������(W1��W2Ϊ��������ϡ��µף����������ʱW1 = W2��ʾ�����)
	
	double S1; // �곡���߾�ú�����
	double f1; // ʯ�Ž�ú�������Ͽؾ���
	double f2; // ʯ�Ž�ú�������¿ؾ���
	double d1; // ʯ�Ž�ú��������ؾ���
	double d2; // ʯ�Ž�ú�������ҿؾ���

	double w1, w2; // �곡ӭͷ������ұ߾�
	double h1, h2; // �������С������߶�
	double d0;     // ���жѹֱ��
};
