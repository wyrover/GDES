#include "StdAfx.h"
#include "CaculFunc.h"
#include "ValueHelper.h"

#include <math.h>
//#include <iostream>
//using namespace std;

#include "../MineGE/DataHelper.h"
#include "../MineGE/config.h"

#define LESSTHANZERO(x) x <= TOLERANCE
#define NOTZERO(x) abs(x) > TOLERANCE 

bool CaculFunc::coalResrevesCacul()
{
	CString strL,strH1,strH2,strh1,strh2,strR,strl,strm,strGama;
	DataHelper::GetPropertyData(m_objId,_T("���۵�Ԫ���򳤶�"),strL);
	DataHelper::GetPropertyData(m_objId,_T("���۵�Ԫ���򳤶�"),strl);
	DataHelper::GetPropertyData(m_objId,_T("�����������˹Ԥ�ŵ�ֵ���1"),strH1);
	DataHelper::GetPropertyData(m_objId,_T("�����������˹Ԥ�ŵ�ֵ���2"),strH2);
	DataHelper::GetPropertyData(m_objId,_T("�����������˹Ԥ�ŵ�ֵ���1"),strh1);
	DataHelper::GetPropertyData(m_objId,_T("�����������˹Ԥ�ŵ�ֵ���2"),strh2);
	DataHelper::GetPropertyData(m_objId,_T("��������ЧӰ��뾶"),strR);
	DataHelper::GetPropertyData(m_objId,_T("ƽ��ú����"),strm);
	DataHelper::GetPropertyData(m_objId,_T("ú���ܶ�"),strGama);
	double fL = _tstof(strL);
	double fH1 = _tstof(strH1);
	double fH2 = _tstof(strH2);
	double fh1 = _tstof(strh1);
	double fh2 = _tstof(strh2);
	double fR = _tstof(strR);
	double fl = _tstof(strl);
	double fm = _tstof(strm);
	double fGama = _tstof(strGama);

	unitTrendlenth = fL;
	unitTendencyLenth = fl;
	unitTrendWidthz1 = fH1;
	unitTrendWidthz2 = fH2;
	unitTendcyWidthz1 = fh1;
	unitTendcyWidthz2 = fh2;
	drainGasRadius = fR;
	averMinePly = fm;
	mineDencity = fGama;

	double leftPart = unitTrendlenth - unitTrendWidthz1 - unitTrendWidthz2 + 2*drainGasRadius;
	double middlePart = unitTendencyLenth - unitTendcyWidthz1 - unitTendcyWidthz2 + drainGasRadius;
	double rightPart = averMinePly * mineDencity;
	unitCoalResreves = leftPart * middlePart * rightPart / 10000;

	if ( LESSTHANZERO(unitTrendlenth) || LESSTHANZERO(unitTendencyLenth))
	{
		unitCoalResreves = 0;
	}

	if (LESSTHANZERO(unitCoalResreves))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}
	CString strValue;
	strValue.Format(_T("%lf"),unitCoalResreves);
	return DataHelper::SetPropertyData( m_objId, _T( "���۵�Ԫ�������ú̿����" ), strValue );
}

bool CaculFunc::remnantGasByQCacul()
{
	CString strW,strQ,strG;
	DataHelper::GetPropertyData(m_objId,_T("���۵�Ԫ��׳����˹����"),strQ);
	DataHelper::GetPropertyData(m_objId,_T("���۵�Ԫ�������ú̿����"),strG);
	DataHelper::GetPropertyData(m_objId,_T("ԭʼ��˹����"),strW);

	unitCoalResreves = _tstof(strG);
	oldgasContent = _tstof(strW);
	drainGasAmo = _tstof(strQ);

	if( LESSTHANZERO(unitCoalResreves))
	{
		AfxMessageBox(_T("���ȼ���������۵�ú��"));
		acutPrintf(_T("\n���ȼ���������۵�ú��"));
		return false;
	}

	unitCoalResreves = unitCoalResreves * 10000;

	remnantGas = (oldgasContent * unitCoalResreves - drainGasAmo) / unitCoalResreves;
	
	if (LESSTHANZERO(remnantGas))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),remnantGas);
	return DataHelper::SetPropertyData( m_objId, _T( "������˹����" ), strValue );
}

bool CaculFunc::remnantGasByPCacul()
{
	CString stra,strb,strPc,strAd,strMad,strPi,strGama;
	DataHelper::GetPropertyData(m_objId,_T("��������a"),stra);
	DataHelper::GetPropertyData(m_objId,_T("��������b"),strb);
	DataHelper::GetPropertyData(m_objId,_T("���������˹ѹ��"),strPc);
	DataHelper::GetPropertyData(m_objId,_T("ˮ��"),strMad);
	DataHelper::GetPropertyData(m_objId,_T("�ҷ�"),strAd);
	DataHelper::GetPropertyData(m_objId,_T("��϶��"),strPi);
	DataHelper::GetPropertyData(m_objId,_T("ú���ܶ�"),strGama);

	adsorptionA = _tstof(stra);
	adsorptionB = _tstof(strb);
	absoGasPress = _tstof(strPc);
	qualityWater = _tstof(strMad);
	qualityAsh	= _tstof(strAd);
	poriness = _tstof(strPi);
	mineDencity = _tstof(strGama);

	double part1 = adsorptionA*adsorptionB*(absoGasPress + 0.1) / (1 + adsorptionB*(absoGasPress + 0.1));
	double part2 = (100 - qualityWater - qualityAsh) / 100;
	double part3 = 1 / (1 + 0.31 * qualityWater);
	double part4 = poriness * (absoGasPress + 0.1) / (mineDencity * 0.101325);
	remnantGas = part1 * part2 * part3 + part4;

	if (LESSTHANZERO(remnantGas))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}
	CString strValue;
	strValue.Format(_T("%f"),remnantGas);
	return DataHelper::SetPropertyData( m_objId, _T( "������˹����" ), strValue );
}

bool CaculFunc::remnantPressCacul()
{
	CString stra,strb,strW,strAd,strMad,strPi,strGama;
	DataHelper::GetPropertyData(m_objId,_T("��������a"),stra);
	DataHelper::GetPropertyData(m_objId,_T("��������b"),strb);
	DataHelper::GetPropertyData(m_objId,_T("������˹����"),strW);
	DataHelper::GetPropertyData(m_objId,_T("ˮ��"),strMad);
	DataHelper::GetPropertyData(m_objId,_T("�ҷ�"),strAd);
	DataHelper::GetPropertyData(m_objId,_T("��϶��"),strPi);
	DataHelper::GetPropertyData(m_objId,_T("ú���ܶ�"),strGama);

	adsorptionA = _tstof(stra);
	adsorptionB = _tstof(strb);
	remnantGas = _tstof(strW);
	qualityWater = _tstof(strMad);
	qualityAsh	= _tstof(strAd);
	poriness = _tstof(strPi);
	mineDencity = _tstof(strGama);

	if( LESSTHANZERO(remnantGas))
	{
		AfxMessageBox(_T("���ȼ��������˹����"));
		acutPrintf(_T("\n���ȼ��������˹����"));
		return false;
	}

	double part1 = adsorptionB * remnantGas - adsorptionA*adsorptionB*((100 - qualityWater - qualityAsh)
		/ (100 * (1 + 0.31 * qualityWater))) - poriness / (mineDencity* 0.101325);
	double part2 = -part1 * (-part1);
	double part3 = part2 + 4 * adsorptionB * remnantGas * poriness / (mineDencity* 0.101325);
	double part4 = (part1 + sqrt(part3))/(2 * adsorptionB * poriness/(mineDencity* 0.101325));
	absoGasPress = part4 - 0.1;

	if (LESSTHANZERO(absoGasPress))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),absoGasPress);
	return DataHelper::SetPropertyData( m_objId, _T( "���������˹ѹ��" ), strValue );
}

bool CaculFunc::remnantGasStandCacul()
{
	CString stra,strb,strAd,strMad,strPi,strGama;
	DataHelper::GetPropertyData(m_objId,_T("��������a"),stra);
	DataHelper::GetPropertyData(m_objId,_T("��������b"),strb);
	DataHelper::GetPropertyData(m_objId,_T("ˮ��"),strMad);
	DataHelper::GetPropertyData(m_objId,_T("�ҷ�"),strAd);
	DataHelper::GetPropertyData(m_objId,_T("��϶��"),strPi);
	DataHelper::GetPropertyData(m_objId,_T("ú���ܶ�"),strGama);

	adsorptionA = _tstof(stra);
	adsorptionB = _tstof(strb);
	qualityWater = _tstof(strMad);
	qualityAsh	= _tstof(strAd);
	poriness = _tstof(strPi);
	mineDencity = _tstof(strGama);

	double part1 = 0.1 * adsorptionA * adsorptionB / (1 + 0.1*adsorptionB);
	double part2 = (100 - qualityWater - qualityAsh) / 100;
	double part3 = 1 / (1 + 0.31 * qualityWater);
	double part4 = poriness / mineDencity;
	remnantGasStand = part1 * part2 * part3 + part4;

	if (LESSTHANZERO(remnantGasStand))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),remnantGasStand);
	return DataHelper::SetPropertyData( m_objId, _T( "��׼����ѹ���µĲд���˹����" ), strValue );
}

bool CaculFunc::analyGasCacul()
{
	CString strWcy,strWcc;
	DataHelper::GetPropertyData(m_objId,_T("������˹����"),strWcy);
	DataHelper::GetPropertyData(m_objId,_T("��׼����ѹ���µĲд���˹����"),strWcc);

	remnantGas = _tstof(strWcy);
	remnantGasStand = _tstof(strWcc);

	if( LESSTHANZERO(remnantGas) && LESSTHANZERO(remnantGasStand))
	{
		AfxMessageBox(_T("���ȼ��������˹�����Ͳд���˹����"));
		acutPrintf(_T("\n���ȼ��������˹�����Ͳд���˹����"));
		return false;
	}

	else if( LESSTHANZERO(remnantGas) )
	{
		AfxMessageBox(_T("���ȼ��������˹����"));
		acutPrintf(_T("\n���ȼ��������˹����"));
		return false;
	}

	else if( LESSTHANZERO(remnantGasStand) )
	{
		AfxMessageBox(_T("���ȼ���д���˹����"));
		acutPrintf(_T("\n���ȼ���д���˹����"));
		return false;
	}

	analysisGas = remnantGas - remnantGasStand;

	if (LESSTHANZERO(analysisGas))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),analysisGas);
	return DataHelper::SetPropertyData( m_objId, _T( "�ɽ�����˹����" ), strValue );
}

bool CaculFunc::faceDrainGasRateCacul()
{
	CString strQmc,strQmf;
	DataHelper::GetPropertyData(m_objId,_T("���¹�������ƽ����˹�����"),strQmc);
	DataHelper::GetPropertyData(m_objId,_T("���¹����������˹��"),strQmf);

	averFaceDrainGas = _tstof(strQmc);
	faceWindGas = _tstof(strQmf);

	faceDrainGasRate = averFaceDrainGas / (averFaceDrainGas + faceWindGas) * 100;

	if (LESSTHANZERO(faceDrainGasRate))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),faceDrainGasRate);
	return DataHelper::SetPropertyData( m_objId, _T( "��������˹�����" ), strValue );
}

bool CaculFunc::mineDrainGasRateCacul()
{
	CString strQkc,strQkf;
	DataHelper::GetPropertyData(m_objId,_T("���¿���ƽ����˹�����"),strQkc);
	DataHelper::GetPropertyData(m_objId,_T("���¿󾮷�����˹��"),strQkf);

	averMineDrainGas = _tstof(strQkc);
	mineWindGas = _tstof(strQkf);

	mineDrainGasRate = averMineDrainGas / (averMineDrainGas + mineWindGas) * 100;

	if (LESSTHANZERO(mineDrainGasRate))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),mineDrainGasRate);
	return DataHelper::SetPropertyData( m_objId, _T( "����˹�����" ), strValue );
}

bool CaculFunc::miningFaceGasCacul()
{
	CString strX0,strX1,strG,strL,strlB,strlH;
	DataHelper::GetPropertyData(m_objId,_T("���ɲ�ԭʼ��˹����"),strX0);
	DataHelper::GetPropertyData(m_objId,_T("�˳���������ú̿�Ĳ�����˹����"),strX1);
	DataHelper::GetPropertyData(m_objId,_T("�ⶨ�ղ���"),strG);
	DataHelper::GetPropertyData(m_objId,_T("���������򳤶�"),strL);
	DataHelper::GetPropertyData(m_objId,_T("�����������˹Ԥ�ŵ�ֵ���1"),strlB);
	DataHelper::GetPropertyData(m_objId,_T("�����������˹Ԥ�ŵ�ֵ���2"),strlH);

	outFaceRemnantGas = _tstof(strX1);
	remnantGas = _tstof(strX0);
	protoDayYield = _tstof(strG);
	faceTendencyLenth = _tstof(strL);
	faceTendencyWidthz1 = _tstof(strlB);
	faceTendencyWidthz2 = _tstof(strlH);

	double part1 = remnantGas - outFaceRemnantGas;
	double part2 = faceTendencyLenth - faceTendencyWidthz1 - faceTendencyWidthz2;
	double part3 = part1 * part2 * protoDayYield;
	miningFaceGas = part3 / (24 * 60 * faceTendencyLenth);

	if (LESSTHANZERO(miningFaceGas))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),miningFaceGas);
	return DataHelper::SetPropertyData( m_objId, _T( "�����ɲ�ӿ�빤����ľ�����˹��" ), strValue );
}

bool CaculFunc::nearbyFaceGasCacul()
{
	CString strG,strm;
	DataHelper::GetPropertyData(m_objId,_T("�ⶨ�ղ���"),strG);
	DataHelper::GetPropertyData(m_objId,_T("ƽ��ú����"),strm);
	//AcStringArray miArray,X0iArray,EntaIArray,XiArray;
	double sum = 0;
	int i = 1;
	while (i)
	{
		CString strmi,strX0i,strEntai,strXi,strmiValue,strX0iValue,strEntaiValue,strXiValue;
		double dmi,dX0i,dEntai,dXi;
		strmi.Format(_T("��%d�ڽ����ú���"),i);
		strX0i.Format(_T("��%d�ڽ����ԭʼ��˹����"),i);
		strXi.Format(_T("��%d�ڽ���Ĳ�����˹����"),i);
		strEntai.Format(_T("��%d�ڽ������˹ӿ����"),i);
		if(DataHelper::GetPropertyData(m_objId,strmi,strmiValue)
			&&DataHelper::GetPropertyData(m_objId,strX0i,strX0iValue)
			&&DataHelper::GetPropertyData(m_objId,strXi,strXiValue))
		{
			dmi = _tstof(strmiValue);
			dX0i = _tstof(strX0iValue);
			dXi = _tstof(strXiValue);
			dEntai = (dX0i - dXi) / dX0i;
			sum += dmi*dX0i*dEntai / _tstof(strm);
			strEntaiValue.Format(_T("%f"),dEntai);
			DataHelper::SetPropertyData(m_objId,strEntai,strEntaiValue);
			i = i+1;
		}//end if

		else
		{
			i = 0;
		}
	}//end while

	protoDayYield = _tstof(strG);
	nearbyFaceGas = protoDayYield / 1440 * sum;

	if (LESSTHANZERO(nearbyFaceGas))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),nearbyFaceGas);
	return DataHelper::SetPropertyData( m_objId, _T( "�����ٽ���ӿ�뿪�ɲ����˹��" ), strValue );
}

static void DealDiameter(double& diameter)
{
	if (diameter <= 0.015)
	{
		diameter = 0.015;
	}
	else if ( 0.015 < diameter && diameter <= 0.02)
	{
		diameter = 0.02;
	}
	else if ( 0.02 < diameter && diameter <= 0.025)
	{
		diameter = 0.025;
	}
	else if ( 0.025 < diameter && diameter <= 0.032)
	{
		diameter = 0.032;
	}
	else if ( 0.032 < diameter && diameter <= 0.04)
	{
		diameter = 0.04;
	}
	else if ( 0.04 < diameter && diameter <= 0.05)
	{
		diameter = 0.05;
	}
	else if ( 0.05 < diameter && diameter <= 0.07)
	{
		diameter = 0.07;
	}
	else if ( 0.07 < diameter && diameter <= 0.08)
	{
		diameter = 0.08;
	}
	else if ( 0.08 < diameter && diameter <= 0.1)
	{
		diameter = 0.1;
	}
	else if ( 0.1 < diameter && diameter <= 0.125)
	{
		diameter = 0.125;
	}
	else if ( 0.125 < diameter && diameter <= 0.15)
	{
		diameter = 0.15;
	}
	//else
	//{
	//	diameter = 0.2;
	//}
}

bool CaculFunc::gasTubeDiameterCacul()
{
	CString strQ,strv;
	DataHelper::GetPropertyData(m_objId,_T("��˹���е���˹����"),strQ);
	DataHelper::GetPropertyData(m_objId,_T("��˹������˹ƽ������"),strv);
	
	gasTubeTraffic = _tstof(strQ);
	averGasTubeSpeed = _tstof(strv);

	//if(0 != gasTubeDiameter && 0 != gasTubeTraffic && 0 == averGasTubeSpeed)
	//{
	//	DealDiameter(gasTubeDiameter);
	//	averGasTubeSpeed = gasTubeTraffic / pow((gasTubeDiameter / 0.1457),2);
	//}

	//else if(0 != gasTubeDiameter && 0 != averGasTubeSpeed && 0 == gasTubeTraffic)
	//{
	//	DealDiameter(gasTubeDiameter);
	//	gasTubeTraffic = averGasTubeSpeed * pow((gasTubeDiameter / 0.1457),2);
	//}

	//else if(0 != gasTubeTraffic && 0 != averGasTubeSpeed && 0 == gasTubeDiameter)
	//{
		gasTubeDiameter = 0.1457 * sqrt( gasTubeTraffic / averGasTubeSpeed );
		DealDiameter(gasTubeDiameter);
		gasTubeDiameter = gasTubeDiameter * 1000;
	//}

	if (LESSTHANZERO(gasTubeDiameter) /*|| gasTubeTraffic < 0 || averGasTubeSpeed < 0*/)
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),gasTubeDiameter);
	DataHelper::SetPropertyData( m_objId, _T( "��˹���ھ�" ), strValue );

	//strValue.Format(_T("%f"),gasTubeTraffic);
	//DataHelper::SetPropertyData( m_objId, _T( "��˹���е���˹����" ), strValue );

	//strValue.Format(_T("%f"),averGasTubeSpeed);
	//DataHelper::SetPropertyData( m_objId, _T( "��˹������˹ƽ������" ), strValue );

	return true;
}

bool CaculFunc::tubeFrictionResisCacul()
{
	CString strD,strL,strK,strQ,strT1,strP1,strRange,strGama;
	DataHelper::GetPropertyData(m_objId,_T("��˹���ھ�"),strD);

	gasTubeDiameter = _tstof(strD);

	tubeFactor = ValueHelper::getKValueByLagrange(gasTubeDiameter);
	strK.Format(_T("%f"),tubeFactor);
	DataHelper::SetPropertyData(m_objId,_T("ϵ��"),strK);

	DataHelper::GetPropertyData(m_objId,_T("��·����"),strL);
	DataHelper::GetPropertyData(m_objId,_T("��˹���е���˹����"),strQ);
	DataHelper::GetPropertyData(m_objId,_T("�ܵ�������ľ���ѹ��"),strP1);
	DataHelper::GetPropertyData(m_objId,_T("��·�¶�"),strT1);
	DataHelper::GetPropertyData(m_objId,_T("�ֲ������ٷֱ�"),strRange);
	DataHelper::GetPropertyData(m_objId,_T("�����˹�Կ���������ܶ�"),strGama);

	gasAirDencity = _tstof(strGama);
	gasTubeTraffic = _tstof(strQ);
	tubeLenth = _tstof(strL);
	tubeTemperature = _tstof(strT1);
	tubeGasPress = _tstof(strP1);
	rangeFactor = _tstof(strRange) * 0.01;

	gasTubeTraffic = gasTubeTraffic * 60;	//��λm3/min->m3/h
	gasTubeDiameter = gasTubeDiameter * 0.1;	//��λmm->cm
	double part1 = 9.81 * gasTubeTraffic * gasTubeTraffic * gasAirDencity * tubeLenth * 101325 * (275 + tubeTemperature);
	double part2 = tubeFactor * pow(gasTubeDiameter,5) * tubeGasPress * 273;
	tubeFrictionResistance = part1 / part2;

	tubePartResistance = rangeFactor * tubeFrictionResistance;

	if (LESSTHANZERO(tubeFrictionResistance) || LESSTHANZERO(tubePartResistance))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),tubeFrictionResistance);
	//acutPrintf(_T("\n����ֵ:%s"),strValue);
	bool ret1 = DataHelper::SetPropertyData( m_objId, _T( "��·��Ħ������" ), strValue );
	strValue.Format(_T("%f"),tubePartResistance);
	bool ret2 = DataHelper::SetPropertyData( m_objId, _T( "��·�ľֲ�����" ), strValue );
	return ( ret1 && ret2 );
}

//void CaculFunc::tubePartResisCacul()
//{
//	tubePartResistance = rangeFactor * tubeFrictionResistance;
//}

bool CaculFunc::pumbFlowCacul()
{
	CString strQz,strx,strP,strH,strK;
	DataHelper::GetPropertyData(m_objId,_T("��˹���������������"),strQz);
	DataHelper::GetPropertyData(m_objId,_T("��˹���Ũ��"),strx);
	DataHelper::GetPropertyData(m_objId,_T("��˹����������ϵ��"),strK);
	DataHelper::GetPropertyData(m_objId,_T("���ش���ѹ"),strP);
	DataHelper::GetPropertyData(m_objId,_T("��˹�õ�ѹ��"),strH);

	pureMineDrainGas = _tstof(strQz);
	double p = _tstof(strP);
	mineDrainGasDencity = _tstof(strx);
	double h = _tstof(strH);
	double k = _tstof(strK);

	//double part1 = 100 * pureMineDrainGas * llReserveFactor;
	//double part2 = mineDrainGasDencity * pumbEfficiency;
	//pumbFlow = part1 / part2;

	if(LESSTHANZERO(h))
	{
		AfxMessageBox(_T("���ȼ�����˹��ѹ��"));
		return false;
	}

	if(NOTZERO((p - h)) && NOTZERO(mineDrainGasDencity))
	{
		pumbFlow = (k * 100 * pureMineDrainGas * p) / (mineDrainGasDencity * (p - h));
	}
	else
	{
		pumbFlow = 0;
	}
	
	if (LESSTHANZERO(pumbFlow))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),pumbFlow);
	return DataHelper::SetPropertyData( m_objId, _T( "��˹�õĶ����" ), strValue );
}

//void CaculFunc::groundZyAllLossCacul()
//{
//	groundZyAllLoss = groundZyMcLoss + groundZyJbLoss + zyNeed;
//}
//
//void CaculFunc::underFyAllLossCacul()
//{
//	underFyAllLoss = drainDrillFy + underFyMcLoss + underFyJbLoss;
//}

bool CaculFunc::pumbPressCacul()
{
	CString strhcm,strcj,strzh,strhzk,strhrm,strhrj,strkyl;
	DataHelper::GetPropertyData(m_objId,_T("��ѹ��·��Ħ��������ʧ"),strhcm);
	DataHelper::GetPropertyData(m_objId,_T("��ѹ��·�ľֲ�������ʧ"),strcj);
	DataHelper::GetPropertyData(m_objId,_T("�û�������ѹ"),strzh);

	DataHelper::GetPropertyData(m_objId,_T("�����׸�ѹ"),strhzk);
	DataHelper::GetPropertyData(m_objId,_T("��ѹ��·��Ħ��������ʧ"),strhrm);
	DataHelper::GetPropertyData(m_objId,_T("��ѹ�ľֲ�������ʧ"),strhrj);

	DataHelper::GetPropertyData(m_objId,_T("����ϵ��"),strkyl);

	groundZyMcLoss = _tstof(strhcm);
	groundZyJbLoss = _tstof(strcj);
	zyNeed = _tstof(strzh);

	drainDrillFy = _tstof(strhzk);
	underFyMcLoss = _tstof(strhrm);
	underFyJbLoss = _tstof(strhrj);

	ylReserveFactor = _tstof(strkyl);

	groundZyAllLoss = groundZyMcLoss + groundZyJbLoss + zyNeed;
	underFyAllLoss = drainDrillFy + underFyMcLoss + underFyJbLoss;
	pumbPress = (groundZyAllLoss + underFyAllLoss + zyNeed) * ylReserveFactor;

	if (LESSTHANZERO(groundZyAllLoss) || LESSTHANZERO(underFyAllLoss) || LESSTHANZERO(pumbPress))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),groundZyAllLoss);
	bool ret1 = DataHelper::SetPropertyData( m_objId, _T( "��ѹ��·��ȫ��������ʧ" ), strValue );
	//CString strValue;
	strValue.Format(_T("%f"),underFyAllLoss);
	bool ret2 = DataHelper::SetPropertyData( m_objId, _T( "��ѹ��·��ȫ��������ʧ" ), strValue );
	//CString strValue;
	strValue.Format(_T("%f"),pumbPress);
	bool ret3 = DataHelper::SetPropertyData( m_objId, _T( "��˹�õ�ѹ��" ), strValue );
	return ( ret1 && ret2 && ret3 );
}

bool CaculFunc::drainDrillFyCacul()
{
	CString strH,strHc,strhrm,strhrj,strkyl;
	DataHelper::GetPropertyData(m_objId,_T("��˹�õ�ѹ��"),strH);
	DataHelper::GetPropertyData(m_objId,_T("��ѹ��·��ȫ��������ʧ"),strHc);
	DataHelper::GetPropertyData(m_objId,_T("��ѹ��·��Ħ��������ʧ"),strhrm);
	DataHelper::GetPropertyData(m_objId,_T("��ѹ�ľֲ�������ʧ"),strhrj);
	DataHelper::GetPropertyData(m_objId,_T("����ϵ��"),strkyl);

	pumbPress = _tstof(strH);
	reserveFactor = _tstof(strkyl);
	groundZyAllLoss = _tstof(strHc);
	underFyAllLoss = _tstof(strhrm);
	underFyJbLoss = _tstof(strhrj);

	drainDrillFy = pumbPress / reserveFactor - groundZyAllLoss - underFyAllLoss - underFyJbLoss;

	if (LESSTHANZERO(drainDrillFy))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),drainDrillFy);
	return DataHelper::SetPropertyData( m_objId, _T( "�����׸�ѹ" ), strValue );
}

bool CaculFunc::zkDbHeightCacul()
{
	CString strIndx,strH;
	DataHelper::GetPropertyData(m_objId,_T("��������"),strIndx);
	DataHelper::GetPropertyData(m_objId,_T("�����濪�ɸ߶�"),strH);
	double H = _tstof(strH);
	int indx = _ttoi(strIndx);
	switch(indx)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		maoluoHeight = 100 * H / (2.1*H + 16) + 2.5;
		liexiHeight = 100 * H / (1.2*H + 2.0) + 8.9;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		maoluoHeight = 100 * H / (4.7*H + 19) + 2.2;
		liexiHeight = 100 * H / (1.6*H + 3.6) + 5.6;
		break;
	case 8:
	case 9:
		maoluoHeight = 100 * H / (6.2*H + 32) + 1.5;
		liexiHeight = 100 * H / (3.1*H + 5.0) + 4.0;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
		maoluoHeight = 100 * H / (7.0*H + 63) + 1.2;
		liexiHeight = 100 * H / (5.0*H + 8.0) + 3.0;
		break;
	default:
		break;
	}
	zkDbHeight = maoluoHeight + (liexiHeight - maoluoHeight) / 3;

	if (LESSTHANZERO(zkDbHeight) || LESSTHANZERO(maoluoHeight) || LESSTHANZERO(liexiHeight))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),zkDbHeight);
	bool ret1 = DataHelper::SetPropertyData( m_objId, _T( "�տ�λ�þ�ú��װ�Ĵ�ֱ����" ), strValue );

	strValue.Format(_T("%f"),maoluoHeight);
	bool ret2 = DataHelper::SetPropertyData( m_objId, _T( "ð����ĸ߶�" ), strValue );

	strValue.Format(_T("%f"),liexiHeight);
	bool ret3 = DataHelper::SetPropertyData( m_objId, _T( "��϶���ĸ߶�" ), strValue );
	return ( ret1 && ret2 && ret3 );
}

static double AngleToArc(double angle)
{
	return angle * PI / 180;
}

static double ArcToAngle(double fArc)
{
	return fArc * 180 / PI;
}

bool CaculFunc::drillDeflectionCacul()
{
	CString strHz,strh,strX,strY,strA,strB;
	DataHelper::GetPropertyData(m_objId,_T("���׸߶�(����λ�þ�ú��װ�߶�)"),strh);
	DataHelper::GetPropertyData(m_objId,_T("�տ�λ�þ�ú��װ�Ĵ�ֱ����"),strHz);
	DataHelper::GetPropertyData(m_objId,_T("�տ׵�������λ��ˮƽ����"),strY);
	DataHelper::GetPropertyData(m_objId,_T("�տ׵����ط��ﴹ��"),strX);
	DataHelper::GetPropertyData(m_objId,_T("���������"),strA);
	DataHelper::GetPropertyData(m_objId,_T("�ط�˳�����"),strB);

	double a = _tstof(strA);
	double b = _tstof(strB);
	kkHeight		= _tstof(strh);
	zkTrendKkDis	= _tstof(strX);
	zkTrendcyHfDis	= _tstof(strY);
	zkDbHeight = _tstof(strHz);

	if (LESSTHANZERO(zkDbHeight))
	{
		AfxMessageBox(_T("���ȼ����տ���Ƹ߶Ȳο�ֵ!"));
		acutPrintf(_T("\n���ȼ����տ���Ƹ߶Ȳο�ֵ!"));
		return false;
	}

	double part1 = pow(zkTrendcyHfDis,2.0);
	double part2 = pow((zkDbHeight-kkHeight-zkTrendKkDis*tan(AngleToArc(a))+ zkTrendcyHfDis * tan(AngleToArc(b))),2);
	double fRet = atan(zkTrendKkDis / sqrt(part1+part2));
	drillDeflection = ArcToAngle(fRet);

	//drillDeflection = atan(zkTrendcyHfDis / zkTrendKkDis)*180/3.1415926;

	if ( LESSTHANZERO(drillDeflection) )
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),drillDeflection);
	return DataHelper::SetPropertyData( m_objId, _T( "���ƫ��" ), strValue );
}

bool CaculFunc::drillElevationCacul()
{
	CString strH,strh,strX,strY,strA,strB;
	DataHelper::GetPropertyData( m_objId, _T( "�տ�λ�þ�ú��װ�Ĵ�ֱ����" ), strH );
	DataHelper::GetPropertyData(m_objId,_T("���׸߶�(����λ�þ�ú��װ�߶�)"),strh);
	DataHelper::GetPropertyData(m_objId,_T("�տ׵�������λ��ˮƽ����"),strY);
	DataHelper::GetPropertyData(m_objId,_T("�տ׵����ط��ﴹ��"),strX);
	DataHelper::GetPropertyData(m_objId,_T("���������"),strA);
	DataHelper::GetPropertyData(m_objId,_T("�ط�˳�����"),strB);
	zkTrendcyHfDis = _tstof(strY);
	zkTrendKkDis = _tstof(strX);
	kkHeight		= _tstof(strh);
	double a = _tstof(strA);
	double b = _tstof(strB);
	double H = _tstof(strH);
	if(LESSTHANZERO(zkTrendcyHfDis))
	{
		AfxMessageBox(_T("�տ׵����ط��ﴹ��û������"));
		return false;
	}
	//acutPrintf(_T("\ntan45:%f"),tan(AngleToArc(45)));
	drillElevation = atan((H - kkHeight - zkTrendKkDis * tan(AngleToArc(a)) + zkTrendcyHfDis * tan(AngleToArc(b))) / zkTrendcyHfDis);
	drillElevation = ArcToAngle(drillElevation);

	//double part1 = maoluoHeight + (liexiHeight - maoluoHeight) / 3 - kkHeight;
	//double part2 = sqrt(zkTrendKkDis*zkTrendKkDis + zkTrendcyHfDis*zkTrendcyHfDis);
	//drillElevation = atan(part1 / part2) * 180 / 3.1415926;

	if ( LESSTHANZERO(drillElevation) )
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),drillElevation);
	return DataHelper::SetPropertyData( m_objId, _T( "�������" ), strValue );
}

bool CaculFunc::drillLengthCacul()
{
	CString strHz,strh,strX,strY,strA,strB;
	DataHelper::GetPropertyData(m_objId,_T("���׸߶�(����λ�þ�ú��װ�߶�)"),strh);
	DataHelper::GetPropertyData(m_objId,_T("�տ�λ�þ�ú��װ�Ĵ�ֱ����"),strHz);
	DataHelper::GetPropertyData(m_objId,_T("�տ׵�������λ��ˮƽ����"),strY);
	DataHelper::GetPropertyData(m_objId,_T("�տ׵����ط��ﴹ��"),strX);
	DataHelper::GetPropertyData(m_objId,_T("���������"),strA);
	DataHelper::GetPropertyData(m_objId,_T("�ط�˳�����"),strB);

	double a = _tstof(strA);
	double b = _tstof(strB);
	kkHeight		= _tstof(strh);
	zkTrendKkDis	= _tstof(strX);
	zkTrendcyHfDis	= _tstof(strY);
	zkDbHeight		= _tstof(strHz);
	
	if ( LESSTHANZERO(zkDbHeight) )
	{
		AfxMessageBox(_T("���ȼ����տ���Ƹ߶Ȳο�ֵ!"));
		acutPrintf(_T("\n���ȼ����տ���Ƹ߶Ȳο�ֵ!"));
		return false;
	}

	//double part1 = (zkDbHeight-kkHeight)*(zkDbHeight-kkHeight);
	//double part2 = zkTrendKkDis*zkTrendKkDis + zkTrendcyHfDis*zkTrendcyHfDis;

	//drillLength = sqrt(part1+part2);

	double part1 = pow(zkTrendcyHfDis,2.0);
	double part2 = pow((zkDbHeight-kkHeight-zkTrendKkDis*tan(AngleToArc(a))+ zkTrendcyHfDis * tan(AngleToArc(b))),2);
	double part3 = pow(zkTrendKkDis,2.0);
	drillLength = sqrt(part1 + part2 + part3);

	if ( LESSTHANZERO(drillLength) )
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}


	CString strValue;
	strValue.Format(_T("%f"),drillLength);
	return DataHelper::SetPropertyData( m_objId, _T( "��׳���" ), strValue );
}

bool CaculFunc::drillEffectiveLenthCacul()
{
	CString strH,strh,strHm,strL,strX,strY;
	DataHelper::GetPropertyData( m_objId, _T( "�տ�λ�þ�ú��װ�Ĵ�ֱ����" ), strH );
	DataHelper::GetPropertyData(m_objId,_T("���׸߶�(����λ�þ�ú��װ�߶�)"),strh);
	DataHelper::GetPropertyData(m_objId,_T("ð����ĸ߶�"),strHm);
	DataHelper::GetPropertyData(m_objId,_T("��׳���"),strL);

	maoluoHeight = _tstof(strHm);
	kkHeight = _tstof(strh);
	drillLength = _tstof(strL);
	zkDbHeight = _tstof(strH);

	if ( LESSTHANZERO(zkDbHeight) && LESSTHANZERO(drillLength) )
	{
		AfxMessageBox(_T("���ȼ����տ���Ƹ߶Ȳο�ֵ����׳���!"));
		//acutPrintf(_T("\n���ȼ����տ���Ƹ߶Ȳο�ֵ����׳���!"));
		return false;
	}

	else if ( LESSTHANZERO(zkDbHeight) )
	{
		AfxMessageBox(_T("���ȼ����տ���Ƹ߶Ȳο�ֵ!"));
		//acutPrintf(_T("\n���ȼ����տ���Ƹ߶Ȳο�ֵ!"));
		return false;
	}
	else if ( LESSTHANZERO(drillLength) )
	{
		AfxMessageBox(_T("���ȼ�����׳���!"));
		//acutPrintf(_T("\n���ȼ�����׳���!"));
		return false;
	}

	drillEffectiveLenth = (zkDbHeight - maoluoHeight)*drillLength / (zkDbHeight - kkHeight);
	
	//double part1 = pow(zkTrendcyHfDis,2.0);
	//double part2 = pow((maoluoHeight-kkHeight-zkTrendKkDis*tan(AngleToArc(a))+ zkTrendcyHfDis * tan(AngleToArc(b))),2);
	//double part3 = pow(zkTrendKkDis,2.0);
	//drillEffectiveLenth = drillLength - sqrt(part1 + part2 + part3);

	if ( LESSTHANZERO(drillEffectiveLenth) )
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}


	CString strValue;
	strValue.Format(_T("%f"),drillEffectiveLenth);
	return DataHelper::SetPropertyData( m_objId, _T( "�����Ч����" ), strValue );
}

bool CaculFunc::faceEffectiveLenthCacul()
{
	CString strB,strAlpha,strBeita,strELenth;
	DataHelper::GetPropertyData(m_objId,_T("�ط�˳�����"),strB);
	DataHelper::GetPropertyData(m_objId,_T("���ƫ��"),strAlpha);
	DataHelper::GetPropertyData(m_objId,_T("�������"),strBeita);
	DataHelper::GetPropertyData(m_objId,_T("�����Ч����"),strELenth);
	
	double b = _tstof(strB);
	double fAlpha = _tstof(strAlpha);
	double fBeita = _tstof(strBeita);
	double fELenth = _tstof(strELenth);
	
	double part1 = cos(AngleToArc(fBeita));
	double part2 = cos(AngleToArc(fAlpha));
	double part3 = cos(AngleToArc(b));
	if(LESSTHANZERO(part3))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}
	faceEffectiveLenth = fELenth * part2 * part1 / part3;	

	if(LESSTHANZERO(faceEffectiveLenth))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),faceEffectiveLenth);
	return DataHelper::SetPropertyData( m_objId, _T( "�������ƽ���Ч����" ), strValue );
}

bool CaculFunc::highDrillDisCacul()
{
	//CString strHz,strh,strX,strHm;
	//DataHelper::GetPropertyData(m_objId,_T("���׸߶�(����λ�þ�ú��װ�߶�)"),strh);
	//DataHelper::GetPropertyData(m_objId,_T("�տ�λ�þ�ú��װ�Ĵ�ֱ����"),strHz);
	//DataHelper::GetPropertyData(m_objId,_T("�տ׵�������λ��ˮƽ����"),strX);
	//DataHelper::GetPropertyData(m_objId,_T("ð����ĸ߶�"),strHm);

	//kkHeight		= _tstof(strh);
	//zkTrendKkDis	= _tstof(strX);
	//maoluoHeight	= _tstof(strHm);
	//zkDbHeight		= _tstof(strHz);

	//if ( LESSTHANZERO(zkDbHeight) )
	//{
	//	AfxMessageBox(_T("���ȼ����տ���Ƹ߶Ȳο�ֵ!"));
	//	acutPrintf(_T("\n���ȼ����տ���Ƹ߶Ȳο�ֵ!"));
	//	return false;
	//}

	//highDrillDis = zkTrendKkDis - (maoluoHeight - kkHeight) * zkTrendKkDis / (zkDbHeight - kkHeight);
	
	CString strB,strAlpha,strBeita,strELenth;
	DataHelper::GetPropertyData(m_objId,_T("�ط�˳�����"),strB);
	DataHelper::GetPropertyData(m_objId,_T("���ƫ��"),strAlpha);
	DataHelper::GetPropertyData(m_objId,_T("�������"),strBeita);
	DataHelper::GetPropertyData(m_objId,_T("�����Ч����"),strELenth);

	double b = _tstof(strB);
	double fAlpha = _tstof(strAlpha);
	double fBeita = _tstof(strBeita);
	double fELenth = _tstof(strELenth);

	double part1 = sin(AngleToArc(fBeita));
	double part2 = cos(AngleToArc(fAlpha));
	double part3 = cos(AngleToArc(b));
	if(LESSTHANZERO(part3))
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}
	highDrillDis = fELenth * part2 * part1 / part3;	

	if ( LESSTHANZERO(highDrillDis) )
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}

	CString strValue;
	strValue.Format(_T("%f"),highDrillDis);
	return DataHelper::SetPropertyData( m_objId, _T( "��λ����곡���ֵ" ), strValue );
}
