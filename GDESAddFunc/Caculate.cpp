#include "StdAfx.h"
#include "Caculate.h"
#include <vector>

typedef std::vector<double> doubleVector;

static void StringsToNum( const AcStringArray& strDatas, doubleVector& doubleDatas)
{
	for(int i = 0; i < strDatas.length(); i++)
	{
		double temp = _tstof(strDatas[i].kACharPtr());
		doubleDatas.push_back(temp);
	}
}

/*
 * 参数1代表
 *	|-0矿井相对瓦斯涌出量
 *	|-1矿井总回风巷瓦斯浓度
 *	|-2当地大气压
 *	|-3矿井最大总回风风量
 *	|-4抽采系统工况系数
 *	|-5瓦斯泵台数
 * 参数2代表
 * 单台瓦斯泵额定流量、瓦斯抽采浓度、瓦斯泵压力、抽采泵富余系数、每套抽采主管道实际内径、每套管道混合经济流速、管道富余系数
 * 参数3代表计算结果
*/
bool Calculate::MineGasCapacityCacul( const AcStringArray& baseDatas,const AcStringArray& pumpDatas,CString& strPumpRet,CString& strSysRet )
{
	if(baseDatas.isEmpty() || pumpDatas.isEmpty()) return false;
	doubleVector dBaseDatas,dPumpDatas;
	StringsToNum(baseDatas,dBaseDatas);
	StringsToNum(pumpDatas,dPumpDatas);
	if(dBaseDatas[0] <= 0) return false;
	int pumpNum = (int)dBaseDatas[5];
	double sumPump = 0;
	double sumTube = 0;
	for(int j = 0; j < pumpNum; j++)
	{
		double pumpQ = dPumpDatas[7*j];
		double pumpCon = dPumpDatas[7*j+1];
		double pumpP = dPumpDatas[7*j+2];
		double pumpKP = dPumpDatas[7*j + 3];
		double tubeD = dPumpDatas[7*j+4];
		double tubeV = dPumpDatas[7*j+5];
		double tubeKS = dPumpDatas[7*j+6];

		double tmp1 = pumpQ * pumpCon* 0.01 * dBaseDatas[4] * (dBaseDatas[2]-pumpP);
		double temp = tmp1 / pumpKP / 101.325;
		sumPump += temp;

		double tmp2 = (tubeD / 0.1457) * (tubeD / 0.1457) * tubeV * pumpCon * 0.01  * (dBaseDatas[2]-pumpP);
		double temp2 = tmp2 / tubeKS / 101.325;
		sumTube += temp2;

		//acutPrintf(_T("\n第%d台泵\t Q:%.2lf,C:%.2lf,P:%.2lf"),j+1,pumpQ,pumpCon,pumpP);
	}

	double ret = (dBaseDatas[1]*0.01*dBaseDatas[3] + sumPump) * 330 * 1440 * 0.0001 / dBaseDatas[0];
	strPumpRet.Format(_T("%.2lf"),ret);

	ret = (dBaseDatas[1]*0.01*dBaseDatas[3] + sumTube) * 330 * 1440 * 0.0001 / dBaseDatas[0];
	strSysRet.Format(_T("%.2lf"),ret);

	//for(int i = 0 ; i < dBaseDatas.size(); i++)
	//{
	//	acutPrintf(_T("\n第%d个值:%.2lf"),i+1,dBaseDatas[i]);
	//}
	//acutPrintf(_T("\n第一个值:%.2lf\t第二个值:%.2lf\t第三个值:%.2lf"),dBaseDatas[1]*0.01*dBaseDatas[3],sumPump,330 * 1440 * 0.0001 / dBaseDatas[0]);
	return true;
}

/*
 * 参数1代表
 *	|-0矿井上年度实际抽采瓦斯量
 *	|-1预开采区域瓦斯含量最大煤层应抽瓦斯吨煤含量
 *	|-2矿井总回风巷瓦斯浓度
 *	|-3矿井最大总回风风量
 *	|-4矿井超前抽采系数
*/

bool Calculate::MineGasRealCacul( const AcStringArray& datas,CString& strYearRet )
{
	if(datas.isEmpty()) return false;
	doubleVector dDatas;
	StringsToNum(datas,dDatas);

	if(dDatas[4] <= 0) return false;
	double ret = (dDatas[2]*0.01*dDatas[3] + dDatas[0]/dDatas[4]/365/1440) * 330 * 1440 * 0.0001 / dDatas[1];
	//acutPrintf(_T("\nQs:%.2lf,k:%.2lf"),dDatas[0],dDatas[4]);
	//acutPrintf(_T("\n第一个值:%.2lf\t第二个值:%.2lf\t第三个值:%.2lf"),dDatas[2]*0.01*dDatas[3], dDatas[0]/dDatas[4]/365/1440,330 * 1440 * 0.0001 / dDatas[1]);

	strYearRet.Format(_T("%.2lf"),ret);
	return true;
}

/*
 * 参数1代表
 * |-0核定区域内煤层的最大瓦斯含量
 * |-1抽采后满足防突要求的残余瓦斯含量
 * |-2矿井回采率
 * |-3矿井年实际预抽瓦斯量
 * |-4邻近层和围岩瓦斯储量系数
 * |-5矿井掘进出煤系数
 * |-6工作面数
 * 参数2代表
 * 工作面平均长度、平均采高、原煤视密度、平均日推进度、工作面回采率
*/

bool Calculate::MinePreGasCacul( const AcStringArray& baseDatas,const AcStringArray& listDatas,CString& strRet )
{
	if(baseDatas.isEmpty() || listDatas.isEmpty()) return false;
	doubleVector dBaseDatas,dListDatas;
	StringsToNum(baseDatas,dBaseDatas);
	StringsToNum(listDatas,dListDatas);
	if(dBaseDatas[4] <= 0) return false;
	if(abs(dBaseDatas[0] - dBaseDatas[1]) < 0.00001) return false;
	int faceNum = (int)dBaseDatas[6];
	double sumFace = 0;
	for(int j = 0; j < faceNum; j++)
	{
		double length = dListDatas[5*j];
		double height = dListDatas[5*j+1];
		double dencity = dListDatas[5*j+2];
		double dayAdvance = dListDatas[5*j + 3];
		double recovRate = dListDatas[5*j+4];

		double temp = length*height*dencity*dayAdvance*0.01*recovRate;
		sumFace += temp;
	}

	double ret = dBaseDatas[3]*0.01*dBaseDatas[2]*0.0001/dBaseDatas[4]/(dBaseDatas[0] - dBaseDatas[1]) + dBaseDatas[5]*330*0.0001*sumFace;
	strRet.Format(_T("%.2lf"),ret);
	return true;

}

/*
 * 参数1代表
 * |-0矿井相对瓦斯涌出量
 * |-1瓦斯抽采达标允许最大矿井绝对瓦斯涌出量
*/

bool Calculate::MineRateGasCacul( const AcStringArray& datas,CString& strRet )
{
	if(datas.isEmpty()) return false;
	doubleVector dDatas;
	StringsToNum(datas,dDatas);

	if(dDatas[0] <= 0) return false;
	double ret = dDatas[1] * 330 * 1440 * 0.0001 / dDatas[0];
	strRet.Format(_T("%.2lf"),ret);
	return true;
}