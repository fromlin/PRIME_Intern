#include "denso_control.h"

denso_control::denso_control()
{
	//Open Client Socket
	hr = bCap_Open_Client("tcp:" TARGET_RC8_IP, 1000, 3, &fd);
	if (SUCCEEDED(hr))
	{
		/* Send SERVICE_START Packet */
		bCap_ServiceStart(fd, NULL);
		//Get Controller Handler
		bstr1 = SysAllocString(L"");					// Name
		bstr2 = SysAllocString(L"CaoProv.DENSO.VRC");   // Provider
		bstr3 = SysAllocString(L"localhost");			// Machine
		bstr4 = SysAllocString(L"");					// Option

		/* Connect to RC8 */
		hr = bCap_ControllerConnect(fd, bstr1, bstr2, bstr3, bstr4, &hCtrl);
		if (SUCCEEDED(hr))	printf("bCap_ControllerConnect Succeeded...\n");
		else				printf("bCap_ControllerConnect Failed...\n");

		//Release Variables
		SysFreeString(bstr1);
		SysFreeString(bstr2);
		SysFreeString(bstr3);
		SysFreeString(bstr4);









		if (SUCCEEDED(hr))
		{
			/* Get robot handle */
			bstr1 = SysAllocString(L"Robot");	// Name
			bstr2 = SysAllocString(L"");		// Option

			hr = bCap_ControllerGetRobot(fd, hCtrl, bstr1, bstr2, &hRob);

			if (SUCCEEDED(hr))	printf("bCap_ControllerGetRobot Succeeded...\n");
			else				printf("bCap_ControllerGetRobot Failed...\n");
			
			//Release Variables
			SysFreeString(bstr1);
			SysFreeString(bstr2	);
		}
	}
}


denso_control::~denso_control()
{
	/* Release robot handle */
	bCap_RobotRelease(fd, &hRob);

	/* Disconnect RC8 */
	bCap_ControllerDisconnect(fd, &hCtrl);

	/* Send SERVICE_STOP packet */
	bCap_ServiceStop(fd);

	/* Close connection */
	bCap_Close_Client(&fd);
}


void denso_control::Initialize()
{
	if (SUCCEEDED(hr)) {
		bstr1 = SysAllocString(L"TakeArm");		// Name

		// Populate parameter option
		VariantInit(&vntParam);
		vntParam.vt = (VT_I4 | VT_ARRAY);
		vntParam.parray = SafeArrayCreateVector(VT_I4, 0, 2);
		SafeArrayAccessData(vntParam.parray, (void **)&plData);
		plData[0] = 0;		// Arm Group Number
		plData[1] = 1;
		SafeArrayUnaccessData(vntParam.parray);

		/* Take Arm */
		printf("TakeArm\n");
		hr = bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);

		SysFreeString(bstr1);
		VariantClear(&vntParam);
		VariantClear(&vntRet);









		if (SUCCEEDED(hr)) {
			/* EXTSPEED 100% */
			bstr1 = SysAllocString(L"ExtSpeed");
			VariantInit(&vntParam);
			vntParam.vt = (VT_R8 | VT_ARRAY);
			vntParam.parray = SafeArrayCreateVector(VT_R8, 0, 1);
			SafeArrayAccessData(vntParam.parray, (void **)&pdData);
			pdData[0] = 100;
			SafeArrayUnaccessData(vntParam.parray);

			printf("External Speed = 100%\n");
			bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
			SysFreeString(bstr1);
			VariantClear(&vntParam);
			VariantClear(&vntRet);



			/* MOTOR ON */
			bstr1 = SysAllocString(L"Motor");
			VariantInit(&vntParam);
			vntParam.vt = VT_I4;
			vntParam.lVal = 1;

			/* Motor on */
			printf("Motor ON\n");
			bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
			SysFreeString(bstr1);
			VariantClear(&vntParam);
			VariantClear(&vntRet);


			/* ACCELERATION 60% */
			hr = bCap_RobotAccelerate(fd, hRob, 0, 100, -1);
			//hr = bCap_RobotAccelerate(fd, hRob, 0, -1, 50);


			//// Move to P(x,y,z,rx,ry,rz,fig) / First Position
			VariantInit(&vntParam);
			vntParam.bstrVal = SysAllocString(L"@E P(400.0, 0.0, 400.0, -180.0, 90.0, 180.0, 1)");
			vntParam.vt = VT_BSTR;
			printf("@E P(400.0, 0.0, 400.0, -180.0, 90.0, 180.0, 1) \n");
			bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
			VariantClear(&vntParam);
		}
	}
}


void denso_control::Getpos()
{
	/* Get current position (x,y,z,rx,ry,rz,fig) */
	VariantInit(&vntParam);
	bstr1 = SysAllocString(L"CurPos");
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SafeArrayAccessData(vntRet.parray, (void **)&pdData);
	memcpy(dPos, pdData, 7 * sizeof(double));
	SafeArrayUnaccessData(vntRet.parray);

	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);

	printf("CurPos=P(%4.3f, %4.3f, %4.3f, %4.3f, %4.3f, %4.3f, %1.f)\n", dPos[0], dPos[1], dPos[2], dPos[3], dPos[4], dPos[5], dPos[6]);





	/* Get current joint position  */
	VariantInit(&vntParam);
	bstr1 = SysAllocString(L"CurJnt");
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SafeArrayAccessData(vntRet.parray, (void **)&pdData);
	memcpy(dPos, pdData, 7 * sizeof(double));
	SafeArrayUnaccessData(vntRet.parray);

	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);

	printf("CurJnt=(%4.3f, %4.3f, %4.3f, %4.3f, %4.3f, %4.3f, %1.f)\n", dPos[0], dPos[1], dPos[2], dPos[3], dPos[4], dPos[5], dPos[6]);
}


void denso_control::Movepos()
{
	//// Move to P(x,y,z,rx,ry,rz,fig)
	VariantInit(&vntParam);
	vntParam.bstrVal = SysAllocString(L"@E P(200.0, 0.0, 400.0, -120.0, 90.0, 180.0, 1)");
	//vntParam.bstrVal = SysAllocString(L"@E P(-100.0, -100.0, 800.0, 0.0, 0.0, 0.0, 1)");
	vntParam.vt = VT_BSTR;
	printf("@E P(200.0, 100.0, 400.0, -120.0, 90.0, 180.0, 1) \n");
	bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
	VariantClear(&vntParam);

	//// Move to P(x,y,z,rx,ry,rz,fig) / First Position
	VariantInit(&vntParam);
	vntParam.bstrVal = SysAllocString(L"@E P(400.0, 0.0, 400.0, -180.0, 90.0, 180.0, 1)");
	vntParam.vt = VT_BSTR;
	printf("@E P(400.0, 0.0, 400.0, -180.0, 90.0, 180.0, 1) \n");
	bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
	VariantClear(&vntParam);
}


void denso_control::OutRange()
{
	/* EXTSPEED 10% */
	bstr1 = SysAllocString(L"ExtSpeed");
	VariantInit(&vntParam);
	vntParam.vt = (VT_R8 | VT_ARRAY);
	vntParam.parray = SafeArrayCreateVector(VT_R8, 0, 1);
	SafeArrayAccessData(vntParam.parray, (void **)&pdData);
	pdData[0] = 10;
	SafeArrayUnaccessData(vntParam.parray);

	printf("External Speed = 10%\n");
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);


	printf("OutRange\n");
	double limit[6] = { 350.0, 350.0, 250.0, 180.0, 0, 180.0 };
	stringstream command;
	command << "@E P(" << PosCmd[0] << "," << PosCmd[1] << "," << PosCmd[2] << ","
		<< PosCmd[3] << "," << PosCmd[4] << "," << PosCmd[5] << ", 1)";


	VariantInit(&vntParam);
	vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
	vntParam.vt = VT_BSTR;
	bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
	VariantClear(&vntParam);
}


void denso_control::MotorOn()
{
	/* MOTOR ON */
	bstr1 = SysAllocString(L"Motor");
	VariantInit(&vntParam);
	vntParam.vt = VT_I4;
	vntParam.lVal = 1;

	/* Motor on */
	printf("Motor ON\n");
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);
}


void denso_control::End()
{
	bstr1 = SysAllocString(L"Motor");
	VariantInit(&vntParam);
	vntParam.vt = VT_I4;
	vntParam.lVal = 0;

	/* Motor off */
	printf("Motor OFF\n");
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);



	bstr1 = SysAllocString(L"GiveArm");
	VariantInit(&vntParam);
	vntParam.vt = VT_EMPTY;

	/* Release arm control authority */
	printf("GiveArm\n");
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);
}