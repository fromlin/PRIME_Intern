#pragma once
#include "adsClient.h"
#include <stdio.h>
#include <atlbase.h>
#include <stdint.h>
#include <iostream>
#include <assert.h>
#include <WinNT.h>
#include <bitset>
#include <math.h>
#include <sstream>
#include <atlconv.h>
#include "bCAPClient/bcap_client.h"


#define _USE_MATH_DEFINES
#define TARGET_RC8_IP "192.168.0.5"
#define SERVER_PORT_NUM 5007


class denso_control
{
public:
	denso_control();
	~denso_control();

	void Initialize();
	void MotorOn();
	void Getpos();
	void Movepos();
	void OutRange();
	void End();


	// DENSO
	int fd;
	HRESULT hr;
	uint32_t hCtrl, hRob;
	BSTR bstr1, bstr2, bstr3, bstr4;

	VARIANT vntParam, vntRet;
	long *plData;
	double *pdData, dPos[7], PosCmd[7];
};

