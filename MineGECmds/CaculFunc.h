#ifndef CACUL_FUNC_H
#define CACUL_FUNC_H

#define PI 3.1415926535897932384626433832795
//�����࣬������������������й�ʽ�ͱ�����CaculFunc�Ǽ�������
class CaculFunc
{
public:
	bool coalResrevesCacul();		//�������۵�ú������
	bool remnantGasByQCacul();		//������˹��������(ͨ����˹�����)
	bool remnantGasByPCacul();		//������˹��������(ͨ����˹ѹ������)
	bool remnantPressCacul();		//������˹ѹ������
	bool remnantGasStandCacul();	//�д���˹�������� 
	bool analyGasCacul();			//�ɽ�����˹��������
	bool faceDrainGasRateCacul();	//��������˹����ʼ���
	bool mineDrainGasRateCacul();	//����˹����ʼ���
	bool miningFaceGasCacul();		//��ú����˹ӿ��������
	bool nearbyFaceGasCacul();		//�ڽ�ú����˹ӿ��������
	bool gasTubeDiameterCacul();	//��˹���ھ�����
	bool tubeFrictionResisCacul();	//��·Ħ����������
	//void tubePartResisCacul();		//��·�ֲ���������
	bool pumbFlowCacul();			//��˹����������
	//void groundZyAllLossCacul();	//��ѹ�ι�·ȫ��������ʧ����
	//void underFyAllLossCacul();		//��ѹ��·��ȫ��������ʧ����
	bool pumbPressCacul();			//��˹��ѹ������
	bool drainDrillFyCacul();		//�׿ڸ�ѹ����
	bool zkDbHeightCacul();			//�տ�λ�þ�ú��װ�Ĵ�ֱ����
	bool drillDeflectionCacul();	//���ƫ��
	bool drillElevationCacul();		//������Ǽ���
	bool drillLengthCacul();		//��׳��ȼ���
	bool drillEffectiveLenthCacul();//�����Ч���ȼ��� 
	bool faceEffectiveLenthCacul();//�������ƽ���Ч���ȼ���
	bool highDrillDisCacul();		//��λ����곡������

public:
	//�����������
	//�������۵�ú������������� 
	double unitCoalResreves;		//���۵�Ԫ�������ú̿����
	double unitTrendlenth;			//���۵�Ԫ���򳤶�
	double unitTendencyLenth;		//���۵�Ԫ���򳤶�
	double unitTrendWidthz1;		//�����������˹Ԥ�ŵ�ֵ���1
	double unitTrendWidthz2;		//�����������˹Ԥ�ŵ�ֵ���2
	double unitTendcyWidthz1;		//�����������˹Ԥ�ŵ�ֵ���1
	double unitTendcyWidthz2;		//�����������˹Ԥ�ŵ�ֵ���2
	double drainGasRadius;			//��������ЧӰ��뾶
	double averMinePly;				//ƽ��ú����
	double mineDencity;				//ú���ܶ�

	//������˹����
	double remnantGas;				//������˹����
	//ͨ����˹��������� 
	double oldgasContent;			//ԭʼ��˹����
	double drainGasAmo;				//���۵�Ԫ��׳����˹����
	//ͨ��������˹�������� 
	double adsorptionA;				//��������a
	double adsorptionB;				//��������b
	double absoGasPress;			//���������˹ѹ�� 
	double qualityWater;			//ˮ��
	double qualityAsh;				//�ҷ�
	double poriness;				//��϶�� 
	
	//�д���˹����
	double remnantGasStand;			//��׼����ѹ�µĲд���˹����

	//�ɽ�����˹����
	double analysisGas;				//�ɽ�����˹����

	//��˹�����
	//��������˹�����
	double faceDrainGasRate;		//��������˹�����
	double averFaceDrainGas;		//���¹�������ƽ����˹�����
	double faceWindGas;				//���¹����������˹��
	//����˹�����
	double mineDrainGasRate;		//����˹�����
	double averMineDrainGas;		//���¿�ƽ����˹���
	double mineWindGas;				//���¿��ŷ���˹����

	//��������˹ӿ����
	//��ú����˹ӿ����
	double miningFaceGas;			//�����ɲ�ӿ�빤����ľ�����˹��
	double outFaceRemnantGas;		//�˳���������ú̿�Ĳ�����˹����
	double protoDayYield;			//�ⶨ�ղ���
	double faceTendencyLenth;		//���������򳤶�
	double faceTendencyWidthz1;		//�����������˹Ԥ�ŵ�ֵ���1
	double faceTendencyWidthz2;		//�����������˹Ԥ�ŵ�ֵ���2

	//�ڽ�����˹ӿ����
	double nearbyFaceGas;		//�����ڽ���ӿ�뿪�ɲ����˹ӿ����
	//double nearbyMinePlyi;		//��i�ٽ����ú��
	//�ڽ�����˹ӿ�����ļ�����Ҫ����

	//��վ������������
	//��˹ֱ��
	double gasTubeDiameter;		//��˹���ھ�
	double gasTubeTraffic;		//��˹���е���˹����
	double averGasTubeSpeed;	//��˹������˹ƽ������

	//��·����
	double tubeFrictionResistance;		//��·�е�Ħ������
	double tubeLenth;					//��·����
	double gasAirDencity;				//�����˹�Կ���������ܶ�
	double tubeFactor;					//ϵ��
	double tubeGasPress;				//�ܵ�������ľ���ѹ��
	double tubeTemperature;				//��·�¶�
	double tubePartResistance;			//��·�еľֲ�����

	double rangeFactor;			//�ֲ���������ʱ�ķ�Χ
	//��˹�õ���������
	double pumbFlow;			//��˹�õĶ����
	double pureMineDrainGas;	//�󾮳����˹������
	double mineDrainGasDencity;	//����˹���Ũ��
	double llReserveFactor;		//��˹����������ϵ��
	double pumbEfficiency;		//��˹�õĻ�еЧ��

	//��ѹ�ι�·ȫ��������ʧ
	double groundZyAllLoss;		//��ѹ��·��ȫ��������ʧ
	double groundZyMcLoss;		//��ѹ��·��Ħ��������ʧ
	double groundZyJbLoss;	    //��ѹ��·�ľֲ�������ʧ
	double zyNeed;      	    //�û�������ѹ

	//��ѹ�ι�·ȫ��������ʧ
	double underFyAllLoss;	     //��ѹ��·��ȫ��������ʧ
	double drainDrillFy;          //���������踺ѹ
	double underFyMcLoss;	     //��ѹ��·��Ħ��������ʧ
	double underFyJbLoss;	     //��ѹ�ľֲ�������ʧ

	//��˹�õ�ѹ��
	double pumbPress;               //��˹�õ�ѹ��
	double ylReserveFactor;	     //��˹��ѹ������ϵ��


	//�׿ڸ�ѹ����
	double reserveFactor;	     //����ϵ��

	//�տ���Ƹ߶Ȳο�ֵ
	double zkDbHeight;             //�տ�λ�þ�ú��װ�Ĵ�ֱ����
	double maoluoHeight;            //ð����ĸ߶�
	double liexiHeight;             //��϶���ĸ߶�

	//ƫ��
	double drillDeflection;         //���ƫ��
	double zkTrendKkDis;         //�տ׵�������λ��ˮƽ����
	double zkTrendcyHfDis;        //�տ׵����ط��ﴹ��

	//����
	double drillElevation;          //�������
	double kkHeight;                //���׸߶�


	//��׳���
	double drillLength;             //��׳���

	//�����Ч����
	double drillEffectiveLenth;   //�����Ч����

	//�������ƽ���Ч����
	double faceEffectiveLenth;    //�������ƽ���Ч����

	//��λ����곡���
	double highDrillDis;           //��λ����곡���

	AcDbObjectId m_objId;		//�ֶ�����ID
};

#endif //CACUL_FUNC_H