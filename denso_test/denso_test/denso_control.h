#pragma once
#include "adsClient.h"
#include <stdio.h>
#include <conio.h>
#include <atlbase.h>
#include <stdint.h>
#include <iostream>
#include <assert.h>
#include <WinNT.h>
#include <bitset>
#include <math.h>
#include <sstream>
#include <atlconv.h>
#include <thread>
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
	void Getparam();
	void Movepos();
	void ChangeJointAngle();
	void OutRange();
	void KillAll();
	void End();
	void background();


	
	void print(string str, int i);
	stringstream makeString(double str[], char type);



	// DENSO
	int					  		   fd;
	HRESULT			  			   hr;
	uint32_t	  		  hCtrl, hRob;
	BSTR   bstr1, bstr2, bstr3, bstr4;

	VARIANT		     vntParam, vntRet;
	long				      *plData;
	double *pdData, *getData, data[7];



private:
	stringstream command;

	double initial_pos[7] = { 400.0, 0.0, 400.0, -180.0, 90.0, 180.0 };
	double		 limit[6] = { 350.0, 350.0, 250.0, 180.0, 0, 180.0 };

	double		  pos1[6] = { 200.0, 0.0, 400.0, -120.0, 90.0, 180.0 };
};

