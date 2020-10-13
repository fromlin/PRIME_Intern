#pragma once
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
#include <atlconv.h>3
#include <thread>
#include "../include/bCAPClient/bcap_client.h"

using namespace std;

#define _USE_MATH_DEFINES
#define TARGET_RC8_IP		"192.168.0.5"
#define SERVER_PORT_NUM			5007

#define COUNT					 10


class denso_control
{
public:
	denso_control();
	~denso_control();

	void Connect();
	void Initialize();
	void MotorOn();
	void End();
	void ClearError();
	void GetParam();

	void Speed(bool ch);
	void MovePos();
	void MoveJoint();
	void ChangeJointAngle();

	void Rotate();
	bool GrabPos();
	void Task();



	stringstream makeString(double str[], char type);
	void print(string str, int i);





	// Denso
	int					  		   fd;
	HRESULT			  			   hr;
	uint32_t	  		  hCtrl, hRob;
	BSTR   bstr1, bstr2, bstr3, bstr4;
	BSTR				  *param_data;

	VARIANT		     vntParam, vntRet;
	long				      *plData;
	double *pdData, *getData, data[7];
	int						speed, gb;





private:
	stringstream command;

	double			  sp[1] = { 0.0 };

	double initialpos[3][6] = { {400.0, 0.0, 400.0, -180.0, 90.0, 180.0},
								{340.0, 0.0, 340.0, 0, 90.0, 0.0} ,				// ** TASK INIT POSE	
								{250.0, 0.0, 200.0, 180.0, 0.0, 180.0} };

	double		  pos[3][6] = { {200.0, 100.0, 500.0, -180.0, 90.0, 180.0},
								{100.0, -100.0, 600.0, -180.0, 90.0, 180.0},
								{400.0, 200.0, 200.0, -180.0, 90.0, 180.0} };

	double	 jointpos[3][6] = { {90.0, -20.0, 50.0, 0.0, 60.0, 0.0},
								{0.0, 60.0, 120.0, 135.0, 0.0, 180.0},
								{-75.0, 30.0, -40.0, -100.0, -60.0, 270.0} };

	double	  taskpos[5][6] = { {300.0, 0.0, 300.0, -180.0, 90.0, 180.0},
												  //
								{300.0, 50.0, 300.0, -180.0, 90.0, 195.0},
								{300.0, -50.0, 300.0, -180.0, 90.0, 165.0},
												  //
								{300.0, 0.0, 225.0, -180.0, 70.0, 180.0},
								{300.0, 0.0, 375.0, -180.0, 110.0, 180.0} };

	double	  grabpos[4][6] = { {200.0, -200.0, 150.0, -180.0, 90.0, 180.0}, 
								{300.0, -200.0, 150.0, -180.0, 90.0, 180.0},
								{200.0, 200.0, 150.0, -180.0, 90.0, 180.0},
								{300.0, 200.0, 150.0, -180.0, 90.0, 180.0} };
	// Joint Max: [170, 120, +151 / -120, 270, 120, 360]
};

