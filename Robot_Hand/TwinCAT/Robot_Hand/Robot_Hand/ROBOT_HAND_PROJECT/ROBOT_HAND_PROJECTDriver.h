///////////////////////////////////////////////////////////////////////////////
// ROBOT_HAND_PROJECTDriver.h

#ifndef __ROBOT_HAND_PROJECTDRIVER_H__
#define __ROBOT_HAND_PROJECTDRIVER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TcBase.h"

#define ROBOT_HAND_PROJECTDRV_NAME        "ROBOT_HAND_PROJECT"
#define ROBOT_HAND_PROJECTDRV_Major       1
#define ROBOT_HAND_PROJECTDRV_Minor       0

#define DEVICE_CLASS CROBOT_HAND_PROJECTDriver

#include "ObjDriver.h"

class CROBOT_HAND_PROJECTDriver : public CObjDriver
{
public:
	virtual IOSTATUS	OnLoad();
	virtual VOID		OnUnLoad();

	//////////////////////////////////////////////////////
	// VxD-Services exported by this driver
	static unsigned long	_cdecl ROBOT_HAND_PROJECTDRV_GetVersion();
	//////////////////////////////////////////////////////
	
};

Begin_VxD_Service_Table(ROBOT_HAND_PROJECTDRV)
	VxD_Service( ROBOT_HAND_PROJECTDRV_GetVersion )
End_VxD_Service_Table


#endif // ifndef __ROBOT_HAND_PROJECTDRIVER_H__