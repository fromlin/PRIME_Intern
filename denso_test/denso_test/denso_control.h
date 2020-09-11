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
#include "../include/bCAPClient/bcap_client.h"


#define _USE_MATH_DEFINES
#define TARGET_RC8_IP "192.168.0.5"
#define SERVER_PORT_NUM 5007


class denso_control
{
public:
	denso_control();
	~denso_control();

	void Connect();
	void Initialize();
	void MotorOn();
	void Speed();
	void getParam();

	void Movepos();
	void Movejoint();
	void ChangeJointAngle();

	void KillAll();
	void End();
	void ClearError();

	void Task();


	


	stringstream makeString(double str[], char type);
	void print(string str, int i);
	void background();





	// DENSO
	int					  		   fd;
	HRESULT			  			   hr;
	uint32_t	  		  hCtrl, hRob;
	BSTR   bstr1, bstr2, bstr3, bstr4;

	VARIANT		     vntParam, vntRet;
	long				      *plData;
	double *pdData, *getData, data[7];
	int							speed;





private:
	stringstream command;

	double initial_pos[6] = { 400.0, 0.0, 400.0, -180.0, 90.0, 180.0 };
	double	    pos[3][6] = { {200.0, 100.0, 500.0, -180.0, 90.0, 180.0},
							  {100.0, -100.0, 600.0, -180.0, 90.0, 180.0},
							  {400.0, 200.0, 200.0, -180.0, 90.0, 180.0} };
	double jointpos[3][6] = { {90.0, -20.0, 50.0, 0.0, 60.0, 0.0},
							  {0.0, 60.0, 120.0, 135.0, 0.0, 180.0},
							  {-75.0, 30.0, -40.0, -100.0, -60.0, 270.0} };
	double  taskpos[5][6] = { {240.0, 0.0, 300.0, -180.0, 90.0, 180.0},
							  {240.0, 320.0, 300.0, -180.0, 90.0, 180.0},
							  {240.0, -320.0, 300.0, -180.0, 90.0, 180.0},
							  {300.0, 0.0, 200.0, -180.0, 90.0, 180.0},
							  {300.0, 0.0, 400.0, -180.0, 90.0, 180.0} };
	// Joint Max: [170, 120, +151 / -120, 270, 120, 360]
};

