#include "StdAfx.h"
#include "DefGEPlugin.h"

// ˫�����
#include "DoubleTunnelDraw.h"

// �������
#include "SingleTunnelDraw.h"

// ��������
#include "PolyLineTunnelDraw.h"

// ����ͷ
#include "SimpelDirectionDraw.h"

// ����߼�ͷ
#include "PolyLineDirectionDraw.h"

// �򵥷��
#include "SimpleLocalFanDraw.h"
#include "FanTagGEDraw.h"

// �򵥷�Ͳ
#include "SimpleChimneyDraw.h"


// ���������
#include "DoubleTTunnelDraw.h"
#include "PolyLineTTunnelDraw.h"

//�ڵ�
#include "SimpleJointDraw.h"

// �򵥷��
#include "SimpleWindLibraryDraw.h"

//��Ͳ�ĳ���ڷ���
#include "QTagGEDraw.h"

//��Ч���
#include "EffectRanTagGEDraw.h"

//��˹��ɹ�·
#include "GasTubeGEDraw.h"

//��˹��
#include "GasPumpGEDraw.h"

//��
#include "ValveGEDraw.h"

//��ˮ��
#include "TailraceDraw.h"

//������
#include "FlowmeterGEDraw.h"

//�����
#include "BackFireGEDraw.h"

//�ⶨ��
#include "DetermineHoleGEDraw.h"

//��˹����
#include "GasFlowTagGEDraw.h"

//���
#include "DrillGEDraw.h"

//�¶ȴ�����
#include "TempeSensorGEDraw.h"

//����������
#include "FlowSensorGEDraw.h"

//ѹ�����
#include "DifferPressSensorGEDraw.h"

//��˹������
#include "GasSensorGEDraw.h"

//CO������
#include "COSensorGEDraw.h"

//���
#include "SimplePoreDraw.h"
//�곡
#include "SimpleDrillSiteDraw.h"
//ú����
#include "SimpleCoalSurfaceDraw.h"
//ʯ��
#include "SimpleRockGateDraw.h"

// ��Ͳ
//#include "DoubleShaftDraw.h"

// ָ����
//#include "CompassBlockDraw.h"

// CircularJointDraw����Ч����Jigʵ��
//#include "CircularJointDraw_Jig.h"

// SingleTunnelDraw����Ч����Jigʵ��
//#include "SingleTunnelDraw_Jig.h"

// DoubleTunnelDraw����Ч����Jigʵ��
//#include "DoubleTunnelDraw_Jig.h"

// DoubleArcTunnelDraw����Ч����Jigʵ��
//#include "SingleArcTunnelDraw_Jig.h"

// DoubleArcTunnelDraw����Ч����jigʵ��
//#include "DoubleArcTunnelDraw_Jig.h"

// DoubleWorkSurfaceDraw����Ч����jigʵ��
//#include "DoubleWorkSurfaceDraw_Jig.h"

void DefGEPlugin::initialize( MineGEDrawManager* pGEDrawManager )
{
    BEGIN_REGISTER_MINEGE_DRAW( MineGEDrawManager, pGEDrawManager )
    // ���
    REGISTER_MINEGE_DRAW( Tunnel, DoubleTunnelDraw )
    REGISTER_MINEGE_DRAW( Tunnel, PolyLineTunnelDraw )
    REGISTER_MINEGE_DRAW( Tunnel, SingleTunnelDraw )
    //REGISTER_MINEGE_DRAW_JIG(Tunnel, DoubleTunnelDraw, DoubleTunnelDraw_Jig)
    //REGISTER_MINEGE_DRAW_JIG(Tunnel, SingleTunnelDraw, SingleTunnelDraw_Jig)

	// �������
	REGISTER_MINEGE_DRAW( ArcTunnel, DoubleArcTunnelDraw )

    // ���������
    REGISTER_MINEGE_DRAW( TTunnel, DoubleTTunnelDraw )
    REGISTER_MINEGE_DRAW( TTunnel, PolyLineTTunnelDraw )

	//��������
    REGISTER_MINEGE_DRAW( WindDirection, PolyLineDirectionDraw )
    REGISTER_MINEGE_DRAW( WindDirection, SimpelDirectionDraw )

    // ���
    REGISTER_MINEGE_DRAW( LocalFan, SimpleLocalFanDraw )
	REGISTER_MINEGE_DRAW( FanTagGE, FanTagGEDraw )

	//�ڵ�
	REGISTER_MINEGE_DRAW( Joint, SimpleJointDraw )


    // ��Ͳ
    REGISTER_MINEGE_DRAW( Chimney, SimpleChimneyDraw )

	// ���
	REGISTER_MINEGE_DRAW( WindLibrary, SimpleWindLibraryDraw )
	
	//��Ͳ����ڷ���
	REGISTER_MINEGE_DRAW( QTagGE, QTagGEDraw )

	REGISTER_MINEGE_DRAW( EffectRanTagGE, EffectRanTagGEDraw )

    //REGISTER_MINEGE_DRAW(Shaft, DoubleShaftDraw)

    // ָ����
    //REGISTER_MINEGE_DRAW(Compass, CompassBlockDraw)

	//��˹��ɹ�·
	REGISTER_MINEGE_DRAW( GasTube, GasTubeGEDraw )

	//��˹��
	REGISTER_MINEGE_DRAW( GasPumpGE, GasPumpGEDraw )

	//��
	REGISTER_MINEGE_DRAW( ValveGE, ValveGEDraw )

	//��ˮ��
	REGISTER_MINEGE_DRAW( TailraceGE, TailraceGEDraw )

	//������
	REGISTER_MINEGE_DRAW( FlowmeterGE, FlowmeterGEDraw )

	//�����
	REGISTER_MINEGE_DRAW( BackFireGE, BackFireGEDraw )

	//�ⶨ��
	REGISTER_MINEGE_DRAW( DetermineHoleGE, DetermineHoleGEDraw )

	//��˹����
	REGISTER_MINEGE_DRAW( GasFlowTagGE , GasFlowTagGEDraw )

	//���
	REGISTER_MINEGE_DRAW( DrillGE, DrillGEDraw );

	//�¶ȴ�����
	REGISTER_MINEGE_DRAW( TempeSensorGE, TempeSensorGEDraw );

	//����������
	REGISTER_MINEGE_DRAW( FlowSensorGE, FlowSensorGEDraw );

	//ѹ�����
	REGISTER_MINEGE_DRAW( DifferPressSensorGE, DifferPressSensorGEDraw );

	//��˹������
	REGISTER_MINEGE_DRAW( GasSensorGE, GasSensorGEDraw );

	//CO������
	REGISTER_MINEGE_DRAW(COSensorGE,COSensorGEDraw);

	//���
	REGISTER_MINEGE_DRAW( Pore, SimplePoreDraw );
	//�곡
	REGISTER_MINEGE_DRAW( DrillSite, SimpleDrillSiteDraw );
	//ú����
	REGISTER_MINEGE_DRAW( CoalSurface, SimpleCoalSurfaceDraw );
	//ʯ��
	REGISTER_MINEGE_DRAW( RockGate, SimpleRockGateDraw );

    END_REGISTER_MINEGE_DRAW
}

void DefGEPlugin::unInitialize( MineGEDrawManager* pGEDrawManager )
{
    BEGIN_UNREGISTER_MINEGE_DRAW( MineGEDrawManager, pGEDrawManager )
    // ���
    UNREGISTER_MINEGE_DRAW( Tunnel, DoubleTunnelDraw )
    UNREGISTER_MINEGE_DRAW( Tunnel, PolyLineTunnelDraw )
    UNREGISTER_MINEGE_DRAW( Tunnel, SingleTunnelDraw )

	// �������
	UNREGISTER_MINEGE_DRAW( ArcTunnel, DoubleArcTunnelDraw )

    // ���������
    UNREGISTER_MINEGE_DRAW( TTunnel, DoubleTTunnelDraw )
    UNREGISTER_MINEGE_DRAW( TTunnel, PolyLineTTunnelDraw )

    // ��������
    UNREGISTER_MINEGE_DRAW( WindDirection, PolyLineDirectionDraw )
    UNREGISTER_MINEGE_DRAW( WindDirection, SimpelDirectionDraw )

    // ���
    UNREGISTER_MINEGE_DRAW( LocalFan, SimpleLocalFanDraw )
	UNREGISTER_MINEGE_DRAW( FanTagGE, FanTagGEDraw )

	// �ڵ�
	UNREGISTER_MINEGE_DRAW( Joint, SimpleJointDraw )

    // ��Ͳ
    UNREGISTER_MINEGE_DRAW( Chimney, SimpleChimneyDraw )

	// ���
	UNREGISTER_MINEGE_DRAW( WindLibrary, SimpleWindLibraryDraw )

	//��Ͳ����ڷ���
	UNREGISTER_MINEGE_DRAW( QTagGE, QTagGEDraw )

	UNREGISTER_MINEGE_DRAW( EffectRanTagGE, EffectRanTagGEDraw )

    //UNREGISTER_MINEGE_DRAW(Shaft, DoubleShaftDraw)

    // ָ����
    //UNREGISTER_MINEGE_DRAW(Compass, CompassBlockDraw)

	//��˹��ɹ�·
	UNREGISTER_MINEGE_DRAW( GasTube, GasTubeGEDraw )

	//��˹��
	UNREGISTER_MINEGE_DRAW( GasPumpGE, GasPumpGEDraw )

	//��
	UNREGISTER_MINEGE_DRAW( ValveGE, ValveGEDraw )

	//��ˮ��
	UNREGISTER_MINEGE_DRAW( TailraceGE, TailraceGEDraw )

	//������
	UNREGISTER_MINEGE_DRAW( FlowmeterGE, FlowmeterGEDraw )

	//�����
	UNREGISTER_MINEGE_DRAW( BackFireGE, BackFireGEDraw )

	//�ⶨ��
	UNREGISTER_MINEGE_DRAW( DetermineHoleGE, DetermineHoleGEDraw )

	//�ⶨ��
	UNREGISTER_MINEGE_DRAW( GasFlowTagGE, GasFlowTagGEDraw )

	//���
	UNREGISTER_MINEGE_DRAW( DrillGE, DrillGEDraw );

	//�¶ȴ�����
	UNREGISTER_MINEGE_DRAW( TempeSensorGE, TempeSensorGEDraw );

	//����������
	UNREGISTER_MINEGE_DRAW( FlowSensorGE, FlowSensorGEDraw );

	//ѹ�����
	UNREGISTER_MINEGE_DRAW( DifferPressSensorGE, DifferPressSensorGEDraw );

	//��˹������
	UNREGISTER_MINEGE_DRAW( GasSensorGE, GasSensorGEDraw );

	//CO������
	UNREGISTER_MINEGE_DRAW(COSensorGE,COSensorGEDraw);

	//���
	UNREGISTER_MINEGE_DRAW( Pore, SimplePoreDraw );
	//�곡
	UNREGISTER_MINEGE_DRAW( DrillSite, SimpleDrillSiteDraw );
	//ú����
	UNREGISTER_MINEGE_DRAW( CoalSurface, SimpleCoalSurfaceDraw );
	//ʯ��
	UNREGISTER_MINEGE_DRAW( RockGate, SimpleRockGateDraw );
	
    END_UNREGISTER_MINEGE_DRAW
}

// ���������������ʵ�ֲ�����
MINEGE_PLUGIN_CREATE_FUNC_IMPL( DefGEPlugin )

//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(Joint, CircularJointDraw, CircularJointDraw_Jig)

//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(Tunnel, SingleTunnelDraw, SingleTunnelDraw_Jig)
//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(Tunnel, DoubleTunnelDraw, DoubleTunnelDraw_Jig)
//
//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(ArcTunnel, DoubleArcTunnelDraw, SingleArcTunnelDraw_Jig)
//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(ArcTunnel, DoubleArcTunnelDraw, DoubleArcTunnelDraw_Jig)
//
//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(WorkSurface, DoubleWorkSurfaceDraw, DoubleWorkSurfaceDraw_Jig)

// draw��������ʵ�ֺ�(ɾ���ˣ���Ϊ������ȷ��draw�Ĺ��췽ʽ��Ŀǰ��draw��ͨ��"����+ARX RTTI"��������)
