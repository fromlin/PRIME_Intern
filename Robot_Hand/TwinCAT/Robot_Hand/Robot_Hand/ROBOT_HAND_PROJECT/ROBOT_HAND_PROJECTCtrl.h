///////////////////////////////////////////////////////////////////////////////
// ROBOT_HAND_PROJECTCtrl.h

#ifndef __ROBOT_HAND_PROJECTCTRL_H__
#define __ROBOT_HAND_PROJECTCTRL_H__

#include <atlbase.h>
#include <atlcom.h>


#include "resource.h"       // main symbols
#include "ROBOT_HAND_PROJECTW32.h"
#include "TcBase.h"
#include "ROBOT_HAND_PROJECTClassFactory.h"
#include "TcOCFCtrlImpl.h"

class CROBOT_HAND_PROJECTCtrl 
	: public CComObjectRootEx<CComMultiThreadModel>
	, public CComCoClass<CROBOT_HAND_PROJECTCtrl, &CLSID_ROBOT_HAND_PROJECTCtrl>
	, public IROBOT_HAND_PROJECTCtrl
	, public ITcOCFCtrlImpl<CROBOT_HAND_PROJECTCtrl, CROBOT_HAND_PROJECTClassFactory>
{
public:
	CROBOT_HAND_PROJECTCtrl();
	virtual ~CROBOT_HAND_PROJECTCtrl();

DECLARE_REGISTRY_RESOURCEID(IDR_ROBOT_HAND_PROJECTCTRL)
DECLARE_NOT_AGGREGATABLE(CROBOT_HAND_PROJECTCtrl)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CROBOT_HAND_PROJECTCtrl)
	COM_INTERFACE_ENTRY(IROBOT_HAND_PROJECTCtrl)
	COM_INTERFACE_ENTRY(ITcCtrl)
	COM_INTERFACE_ENTRY(ITcCtrl2)
END_COM_MAP()

};

#endif // #ifndef __ROBOT_HAND_PROJECTCTRL_H__
