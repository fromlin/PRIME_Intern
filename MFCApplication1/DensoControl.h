#pragma once
#include "adsClient.h"
class DensoControl
{
private:
	AmsAddr SrvAddr, ModuleAddr;
	PAmsAddr pSrvAddr, pModuleAddr;

public:
	DensoControl();
	~DensoControl();
	void GetAxisPosition(PAmsAddr adsport);

};

