#include "StdAfx.h"
#include "resource.h"

#include "../MineGE/HelperClass.h"

#include "DrawCmd.h"

// ����ע���������
#ifndef DRAW_VENT_CMD_SERVICE_NAME
#define DRAW_VENT_CMD_SERVICE_NAME _T("DRAWVENTCMD_SERVICE_NAME")
#endif

class CDrawVentCmdApp : public AcRxArxApp
{

public:
    CDrawVentCmdApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // ע�����
        acrxRegisterService( DRAW_VENT_CMD_SERVICE_NAME );

        acutPrintf( _T( "\nDrawVentCmd::On_kLoadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // ɾ������
        delete acrxServiceDictionary->remove( DRAW_VENT_CMD_SERVICE_NAME );

        acutPrintf( _T( "\nDrawVentCmd::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kLoadDwgMsg ( pkt ) ;

        acutPrintf( _T( "\nDrawVentCmd::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadDwgMsg ( pkt ) ;

        acutPrintf( _T( "\nDrawVentCmd::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

    // �������
    static void JL_DrawTunnel( void )
    {
        DrawCmd::DrawTunnel();        // ʹ����ͨ�����н����������
		//DrawCmd::testHD();
    }
	// ���ƻ������
	static void JL_DrawArcTunnel( void )
	{
		DrawCmd::DrawArcTunnel();        // ʹ����ͨ�����н����������
	}


    // ���ƾ��������
    static void JL_DrawTTunnel()
    {
        DrawCmd::DrawTTunnel();  // ���ƾ��������
    }

    static void JL_ResetInTunnel()
    {
        DrawCmd::ResetInTunnel();  // �������þ��������ĵ������
    }

    static void JL_ShowInTunnel()
    {
        DrawCmd::ShowInTunnel();  // ������ʾ���������ĵ������
    }

    // ���Ʒ�������
    static void JL_DrawDirection()
    {
        DrawCmd::DrawDirection();       // ʹ����ͨ�����н������Ʒ�������
    }

    // ���ƾֲ��ȷ��
    static void JL_DrawLocalFan()
    {
        DrawCmd::DrawLocalFan();       // ʹ����ͨ�����н������Ʒ��
    }

	static void JL_DrawLocalFanTag()
	{
		DrawCmd::DrawFanTag(); 
	}

	static void JL_DrawChimney()
	{
		DrawCmd::DrawChimney();
	}

	//���Ʒ��
	static void JL_DrawWindLibrary(void)
	{
		DrawCmd::DrawWindLibrary();
	}

    static void JL_AutoDraw( void )
    {
        DrawCmd::AutoDraw();
    }

	//�ڵ����
	static void JL_DrawJoint(void)
	{
		DrawCmd::DrawJoint();
	}

    static void JL_AutoDirection( void )
    {
        DrawCmd::AutoDirection();
    }

	static void JL_AutoGasFlow( void )
	{
		DrawCmd::AutoGasFlow();
		//PropertyDataUpdater::UpdateAllGasFlow();
	}

    static void JL_TunnelJunctionEnclose()
    {
        DrawCmd::JunctionEnclose();
    }

	//�������ó���˹��ɹ�·
	static void JL_DrawMainTube(void)
	{
		DrawCmd::DrawGasTube(PD_GAS_TUBE);
	}

	//�����ƶ�������˹��ɹ�·
	static void JL_DrawTrunkTube(void)
	{
		DrawCmd::DrawGasTube(MPE_GAS_TUBE);
	}

	//�����ƶ�������˹��ɹ�·
	static void JL_DrawBranchTube(void)
	{
		DrawCmd::DrawGasTube(MPD_GAS_TUBE);
	}

	//������˹��
	static void JL_DrawGasPump(void)
	{
		DrawCmd::DrawGasPump();
	}

	//������˹��·��
	static void JL_DrawValve(void)
	{
		DrawCmd::DrawValve();
	}

	//���Ʒ�ˮ��
	static void JL_DrawTailrace(void)
	{
		DrawCmd::DrawTailrace();
	}

	//����������
	static void JL_DrawFlowmeter(void)
	{
		DrawCmd::DrawFlowmeter();
	}

	//���������
	static void JL_DrawBackFire(void)
	{
		DrawCmd::DrawBackFire();
	}

	//���Ʋⶨ��
	static void JL_DrawDetermineHole(void)
	{
		DrawCmd::DrawDetermineHole();
	}

	//������˹����
	static void JL_DrawGasFlow(void)
	{
		DrawCmd::DrawGasFlow();
		//PropertyDataUpdater::UpdateAllGasFlow();
	}

	static void JL_DrawDrill(void)
	{
		DrawCmd::DrawDrill();
	}

	//�����¶ȴ�����
	static void JL_DrawTempeSensor(void)
	{
		DrawCmd::DrawTempeSensor();
	}

	//��������������
	static void JL_DrawFlowSensor(void)
	{
		DrawCmd::DrawFlowSensor();
	}

	//����ѹ�����
	static void JL_DrawDifferPressSensor(void)
	{
		DrawCmd::DrawDifferPressSensor();
	}

	//������˹������
	static void JL_DrawGasSensor(void)
	{
		DrawCmd::DrawGasSensor();
	}

	//����CO������
	static void JL_DrawCOSensor(void)
	{
		DrawCmd::DrawCOSensor();
	}

    static void JL_ReverseDirection( void )
    {
        DrawCmd::ReverseDirection();
    }

    static void JL_ReBindTagGE( void )
    {
        DrawCmd::ReBindEdge();
    }

    static void JL_SplitByPoint( void )
    {
        DrawCmd::SplitDeal();
    }

	static void JL_MergeChimney( void )
	{
		DrawCmd::MergeChimney();
	}

	static void JL_DrawedQTagGE(void)
	{
		DrawCmd::DrawQTagGE();
	}

	static void JL_DrawedEDTagGE(void)
	{
		DrawCmd::DrawEffectRanGE();
	}

	static void JL_DeleteDirections( void )
	{
		DrawCmd::DeleteDirection();
	}

	static void JL_DeleteGasFlow( void )
	{
		DrawCmd::DeleteAllGasFlow();
	}

	static void JL_DeleteFanTags( void )
	{
		DrawCmd::DeleteFanTag();
	}

	static void JL_DeleteQTags( void )
	{
		DrawCmd::DeleteQTag();
	}

	static void JL_DeleteEDTags( void )
	{
		DrawCmd::DeleteEDTag();
	}


} ;


IMPLEMENT_ARX_ENTRYPOINT( CDrawVentCmdApp )

//ACED_ARXCOMMAND_ENTRY_AUTO(CDrawVentCmdApp, JL, _DrawJoint, DrawJoint, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawTunnel, DrawTunnel, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawArcTunnel, DrawArcTunnel, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawTTunnel, DrawTTunnel, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _ResetInTunnel, ResetInTunnel, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _ShowInTunnel, ShowInTunnel, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawLocalFan, DrawLocalFan, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawLocalFanTag, DrawLocalFanTag, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawChimney, DrawChimney, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawWindLibrary, DrawWindLibrary, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawDirection, DrawDirection, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawJoint, DrawJoint, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _AutoDraw, AutoDraw, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _AutoDirection, AutoDirection, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _AutoGasFlow, AutoGasFlow, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DeleteGasFlow, DeleteGasFlow, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _TunnelJunctionEnclose, TunnelJunctionEnclose, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _ReverseDirection, ReverseDirection, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _ReBindTagGE, ReBindTagGE, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _SplitByPoint, SplitByPoint, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _MergeChimney, MergeChimney, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DeleteDirections, DeleteDirections, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DeleteFanTags, DeleteFanTags, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawedQTagGE, DrawedQTagGE, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DeleteQTags, DeleteQTages, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawedEDTagGE, DrawedEDTagGE, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DeleteEDTags, DeleteEDTages, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawMainTube, DrawMainTube, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawTrunkTube, DrawTrunkTube, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawBranchTube, DrawBranchTube, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawGasPump, DrawGasPump, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawValve, DrawValve, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawTailrace, DrawTailrace, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawGasFlow, DrawGasFlow, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawFlowmeter, DrawFlowmeter, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawBackFire, DrawBackFire, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawDetermineHole, DrawDetermineHole, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawDrill, DrawDrill, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawTempeSensor, DrawTempeSensor, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawFlowSensor, DrawFlowSensor, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawCOSensor, DrawCOSensor, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawDifferPressSensor, DrawDifferPressSensor, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawVentCmdApp, JL, _DrawGasSensor, DrawGasSensor, ACRX_CMD_TRANSPARENT, NULL )
