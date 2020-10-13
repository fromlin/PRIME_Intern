///////////////////////////////////////////////////////////////////////////////
// ROBOT_HAND_PROJECTDriver.cpp
#include "TcPch.h"
#pragma hdrstop

#include "ROBOT_HAND_PROJECTDriver.h"
#include "ROBOT_HAND_PROJECTClassFactory.h"

DECLARE_GENERIC_DEVICE(ROBOT_HAND_PROJECTDRV)

IOSTATUS CROBOT_HAND_PROJECTDriver::OnLoad( )
{
	TRACE(_T("CObjClassFactory::OnLoad()\n") );
	m_pObjClassFactory = new CROBOT_HAND_PROJECTClassFactory();

	return IOSTATUS_SUCCESS;
}

VOID CROBOT_HAND_PROJECTDriver::OnUnLoad( )
{
	delete m_pObjClassFactory;
}

unsigned long _cdecl CROBOT_HAND_PROJECTDriver::ROBOT_HAND_PROJECTDRV_GetVersion( )
{
	return( (ROBOT_HAND_PROJECTDRV_Major << 8) | ROBOT_HAND_PROJECTDRV_Minor );
}

