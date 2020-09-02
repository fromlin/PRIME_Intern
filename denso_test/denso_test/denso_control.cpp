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
		bstr1 = SysAllocString(L"TakeArm");

		// Populate parameter option
		VariantInit(&vntParam);
		vntParam.vt = (VT_I4 | VT_ARRAY);
		vntParam.parray = SafeArrayCreateVector(VT_I4, 0, 2);
		SafeArrayAccessData(vntParam.parray, (void **)&plData);
		plData[0] = 0;		// Arm Group Number
		plData[1] = 3;		// Mode
		SafeArrayUnaccessData(vntParam.parray);

		/* Take Arm */
		printf("TakeArm\n");
		hr = bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);

		SysFreeString(bstr1);
		VariantClear(&vntParam);
		VariantClear(&vntRet);



		if (SUCCEEDED(hr)) {
			bstr1 = SysAllocString(L"ExtSpeed");
			VariantInit(&vntParam);
			vntParam.vt = (VT_R8 | VT_ARRAY);
			vntParam.parray = SafeArrayCreateVector(VT_R8, 0, 1);
			SafeArrayAccessData(vntParam.parray, (void **)&pdData);
			/* EXTSPEED 100% & ACCELERATION 50% & DECELERATION 50% */
			pdData[0] = 10;
			pdData[1] = 50;
			pdData[2] = 50;
			SafeArrayUnaccessData(vntParam.parray);

			printf("External Speed = %.f\n", pdData[0]);
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

		

			// Move to P(x,y,z,rx,ry,rz,fig) / First Position
			VariantInit(&vntParam);
			command = makeString(initial_pos, 'P');
			vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
			vntParam.vt = VT_BSTR;
			bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
			VariantClear(&vntParam);
			print(command.str(), 0);
		}
	}
}


void denso_control::Getparam()
{
	while (1) {
		/* Get current position (x,y,z,rx,ry,rz,fig) */
		VariantInit(&vntParam);
		bstr1 = SysAllocString(L"CurPos");
		bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
		SafeArrayAccessData(vntRet.parray, (void **)&getData);
		memcpy(data, getData, 7 * sizeof(double));
		SafeArrayUnaccessData(vntRet.parray);

		SysFreeString(bstr1);
		VariantClear(&vntParam);
		VariantClear(&vntRet);


		
		/* Get current joint position  */
		VariantInit(&vntParam);
		bstr1 = SysAllocString(L"CurJnt");
		bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
		SafeArrayAccessData(vntRet.parray, (void **)&getData);
		memcpy(data, getData, 7 * sizeof(double));
		SafeArrayUnaccessData(vntRet.parray);

		SysFreeString(bstr1);
		VariantClear(&vntParam);
		VariantClear(&vntRet);
	}
}


void denso_control::Movepos()
{
	/* ACCELERATION 100% & DECELERATION 50% */
	hr = bCap_RobotAccelerate(fd, hRob, 0, 100, 50);		// 0:ALL axes   /   1:TOOL accel


	if (SUCCEEDED(hr)) {
		//// Move to P(x,y,z,rx,ry,rz,fig)
		VariantInit(&vntParam);
		command = makeString(pos1, 'P');
		vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
		vntParam.vt = VT_BSTR;
		bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
		VariantClear(&vntParam);
		print(command.str(), 0);

		print("\t     to", 0);

		//// Move to P(x,y,z,rx,ry,rz,fig) / First Position
		VariantInit(&vntParam);
		command = makeString(initial_pos, 'P');
		vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
		vntParam.vt = VT_BSTR;
		bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
		VariantClear(&vntParam);
		print(command.str(), 0);
	}
}


void denso_control::ChangeJointAngle()
{
	VariantInit(&vntParam);
	bstr1 = SysAllocString(L"CurJnt");
	hr = bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SafeArrayAccessData(vntRet.parray, (void **)&getData);
	memcpy(data, getData, 7 * sizeof(double));
	SafeArrayUnaccessData(vntRet.parray);

	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);
	print("CurJnt", 2);

	if (SUCCEEDED(hr)) {
		int j;	double ang;
		cout << "which joint?(0~5) : "; cin >> j;
		cout << "Angle : ";		   cin >> ang;
		data[j] = ang;

		VariantInit(&vntParam);
		command = makeString(data, 'J');
		vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
		vntParam.vt = VT_BSTR;
		bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
		VariantClear(&vntParam);
	}



	
	/* Destination Joint */
	VariantInit(&vntParam);
	bstr1 = SysAllocString(L"DestJnt");
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SafeArrayAccessData(vntRet.parray, (void **)&getData);
	memcpy(data, getData, 7 * sizeof(double));
	SafeArrayUnaccessData(vntRet.parray);

	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);
	printf("DestJnt", 2);
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
	command = makeString(limit, 0);
	VariantInit(&vntParam);
	vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
	vntParam.vt = VT_BSTR;
	bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
	VariantClear(&vntParam);
}


void denso_control::KillAll()
{
	bstr1 = SysAllocString(L"KillAll");
	VariantInit(&vntParam);
	hr = bCap_ControllerExecute(fd, hRob, bstr1, vntParam, &vntRet);

	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);
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










stringstream denso_control::makeString(double str[], char type)
{
	stringstream temp;
	if ((pow(str[0], 2) + pow(str[1], 2) + pow(str[2], 2)) > 499)
		str[1] = 0.0;
	switch (type) {
	case 'P':
		temp << "@E P(" << str[0] << "," << str[1] << "," << str[2] << ","
			<< str[3] << "," << str[4] << "," << str[5] << ", 1)";	break;
	case 'J':
		temp << "@E J(" << str[0] << "," << str[1] << "," << str[2] << ","
			<< str[3] << "," << str[4] << "," << str[5] << ", 1)";	break;
	default:														break;
	}
	return temp;
}

void denso_control::print(string str, int i)
{
	cout << str;
	switch (i) {
	case 1:
		printf(" = P(%4.3f, %4.3f, %4.3f, %4.3f, %4.3f, %4.3f, %1.f)\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
		break;
	case 2:
		printf(" = J(%4.3f, %4.3f, %4.3f, %4.3f, %4.3f, %4.3f, %1.f)\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
		break;
	default:
		printf("\n");
		break;
	}
}




void denso_control::background() {
	int key;
	while (1) {
		printf("\n>> ");
		key = _getch();
		if (key == 27)		 break;
		switch (key) {
		case 'I':
			cout << "Init" << endl;
			Initialize(); break;
		case 'O':
			MotorOn();	 break;
		case 'G':
			cout << "Get Param" << endl;
			Getparam();
			print("CurPos", 1); print("CurJnt", 2);
			break;
		case 'J':							// Max: [170, 120, +151/-120, 270, 120, 360]
			cout << "Change Joint Angle" << endl;
			ChangeJointAngle();
			break;
		case 'M':
			cout << "Move" << endl;
			Movepos();	 break;
		case 'R':
			cout << "MOVE" << endl;
			OutRange();	 break;
		case 'E':
			cout << "End" << endl;
			End();		 break;
		case 'Q':
			cout << "Kill All" << endl;
			KillAll();	 break;
		default:			 break;
		}
	}
}