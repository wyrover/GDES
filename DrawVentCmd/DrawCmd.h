#pragma once

enum GASTUBETYPE
{
	PD_GAS_TUBE	 = 0,	//���ó����˹��·
	MPE_GAS_TUBE = 1,	//�ƶ�������˹��·
	MPD_GAS_TUBE = 2	//�ƶ��ó���˹��·
};
class DrawCmd
{
public:
	/* ʵ����DrawVent.cpp�� */
	//static void DrawJoint(void);
	static void DrawTunnel(void);
	static void DrawArcTunnel(void);
	static void DrawWS(void);

	/* ʵ����DrawTTunnel.cpp�� */
	static void DrawTTunnel();
	static void ResetInTunnel();
	static void ShowInTunnel();

	static void DrawMainFan();
	static void DrawLocalFan();
	static void DrawFanTag();
	static void DrawGate(int flag);
	static void DrawCasement(int flag);
	static void DrawWindBridge();
	static void DrawJoint();
	static void DrawWall(int flag);
	static void DrawStorage(int flag);
	static void DrawDirection(void);
	static void DrawWindLibrary(void);

	//��˹��ɹ�·����
	static void DrawGasTube(GASTUBETYPE GEName);

	//��˹�û���
	static void DrawGasPump( void );

	//��
	static void DrawValve( void );

	//��ˮ��
	static void DrawTailrace( void );

	//������
	static void DrawFlowmeter( void );

	//�����
	static void DrawBackFire( void );

	//�ⶨ��
	static void DrawDetermineHole( void );

	//���
	static void DrawDrill( void );

	//�¶ȴ�����
	static void DrawTempeSensor( void );

	//����������
	static void DrawFlowSensor( void );

	//ѹ�����
	static void DrawDifferPressSensor( void );

	//��˹������
	static void DrawGasSensor( void );

	//CO������
	static void DrawCOSensor(void);

	static void DrawChimney(void);
	//static void DrawCompass();

	/* ʵ����AutoDraw.cpp�� */
	static void AutoDraw();
	static void AutoDraw2();

	/* �Զ���ע�������� */
	static void AutoDirection();

	static double ControlDirByMethods(AcDbObjectId objId,double angle);

	/* �Զ�������˹�������� */
	//static void UpdateAllGasFlow();
	// ͨ�ñ༭����
	static void JunctionEnclose(void);
	static void ReverseDirection();
	static void ReBindEdge();
	static void ZoomMineGE();
	static void SplitByPoint(AcDbObjectId objId,AcGePoint3d spt,AcGePoint3d ept,AcGePoint3d pt);
	static void SplitDeal();
	static void MergeChimney();
	static void Merging(AcDbObjectId sourceId,AcDbObjectId mergeId);
	static void DrawQTagGE();
	static void DrawEffectRanGE();

	static void DrawGasFlow();
	static void AutoGasFlow();

	static void DeleteDirection();
	static void DeleteAllGasFlow();
	static void DeleteFanTag();
	static void DeleteQTag();
	static void DeleteEDTag();

	//static AcDbObjectId GetRelatedTW(AcDbObjectId objId);

	static void testHD();
};