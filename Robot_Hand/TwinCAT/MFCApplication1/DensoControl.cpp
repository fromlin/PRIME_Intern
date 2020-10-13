#include "stdafx.h"
#include "DensoControl.h"


DensoControl::DensoControl()
{

}


DensoControl::~DensoControl()
{
}

void DensoControl::GetAxisPosition(PAmsAddr adsport) {
	UINT OPstatus;
	AdsSyncReadReq(adsport, 0x1010010, 0x80000039, 2, &OPstatus);
	cout<< "CLASS = " << OPstatus << endl;
}