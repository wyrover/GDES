#include "StdAfx.h"
#include "Rcu_DbReactor.h"
#include "UIHelper.h"

#include "../MineGE/MineGE.h"
#include "../MineGE/TagGE.h"
#include "../MineGE/DataObject.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

#include "../DefGE/RcuGE.h"
#include "../DefGE/Pore.h"
#include "../DefGE/DrillSite.h"
//#include "../DefGE/RockGate.h"
#include "../DefGE/CoalSurface.h"

Rcu_DbReactor::Rcu_DbReactor ( AcDbDatabase* pDb ) : AcDbDatabaseReactor(), mpDatabase( pDb )
{
    if ( pDb )
    {
        //acutPrintf(_T("\nMineGEErase_DbReactor : %ld"), (long)pDb);
        pDb->addReactor ( this ) ;
    }
}

Rcu_DbReactor::~Rcu_DbReactor ()
{
    Detach () ;
}

void Rcu_DbReactor::Attach ( AcDbDatabase* pDb )
{
    Detach () ;
    if ( mpDatabase == NULL )
    {
        if ( ( mpDatabase = pDb ) != NULL )
            pDb->addReactor ( this ) ;
    }
}

void Rcu_DbReactor::Detach ()
{
    if ( mpDatabase )
    {
        mpDatabase->removeReactor ( this ) ;
        mpDatabase = NULL ;
    }
}

AcDbDatabase* Rcu_DbReactor::Subject () const
{
    return ( mpDatabase ) ;
}

bool Rcu_DbReactor::IsAttached () const
{
    return ( mpDatabase != NULL ) ;
}

void Rcu_DbReactor::objectAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
	AcDbDatabaseReactor::objectAppended(dwg, dbObj);
// 	if(dbObj->isKindOf(RockGate::desc()) || dbObj->isKindOf(RcuGE::desc()))
// 	{
// 		//acutPrintf(_T("\nRcu_DbReactor::objectAppended..."));
// 		ArxMsg msgParam = { dbObj->isA()->name(), dbObj->objectId() };
// 		UIHelper::SendMessage(WM_RCU_ADD, &msgParam);
// 	}
}

//void Rcu_DbReactor::objectUnAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj)
//{
//	AcDbDatabaseReactor::objectAppended(dwg, dbObj);
//	if(dbObj->isKindOf(RockGate::desc()) || dbObj->isKindOf(RcuGE::desc()))
//	{
//		acutPrintf(_T("\nRcu_DbReactor::objectUnAppended..."));
//		ArxMsg msgParam = { dbObj->isA()->name(), dbObj->objectId() };
//		UIHelper::SendMessage(WM_DEL_GE, &msgParam);
//	}
//}
//
//void Rcu_DbReactor::objectReAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj)
//{
//	AcDbDatabaseReactor::objectAppended(dwg, dbObj);
//	if(dbObj->isKindOf(RockGate::desc()) || dbObj->isKindOf(RcuGE::desc()))
//	{
//		acutPrintf(_T("\nRcu_DbReactor::objectReAppended..."));
//		ArxMsg msgParam = { dbObj->isA()->name(), dbObj->objectId() };
//		UIHelper::SendMessage(WM_ADD_GE, &msgParam);
//	}
//}

// ��objectModified���޷���������
void Rcu_DbReactor::objectModified( const AcDbDatabase* dwg, const AcDbObject* dbObj )
{
    AcDbDatabaseReactor::objectModified ( dwg, dbObj );
	//if(dbObj->isKindOf(RockGate::desc()) || dbObj->isKindOf(RcuGE::desc()))
	//{
	//	acutPrintf(_T("\nRcu_DbReactor::objectModified..."));
	//	ArxMsg msgParam = { dbObj->isA()->name(), dbObj->objectId() };
	//	UIHelper::SendMessage(WM_RCU_MODIFY, &msgParam);
	//}
}

void Rcu_DbReactor::objectErased( const AcDbDatabase* dwg, const AcDbObject* dbObj, Adesk::Boolean pErased )
{
	AcDbDatabaseReactor::objectErased ( dwg, dbObj, pErased );
// 	if(dbObj->isKindOf(RockGate::desc()) || dbObj->isKindOf(RcuGE::desc()))
// 	{
// 		//acutPrintf(_T("\nRcu_DbReactor::objectErased..."));
// 		ArxMsg msgParam = { dbObj->isA()->name(), dbObj->objectId() };
// 		if(pErased == Adesk::kTrue)
// 		{
// 			UIHelper::SendMessage(WM_RCU_DEL, &msgParam);
// 		}
// 		else
// 		{
// 			UIHelper::SendMessage(WM_RCU_ADD, &msgParam);
// 		}
// 	}
}
