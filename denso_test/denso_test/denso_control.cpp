#include "denso_control.h"

denso_control::denso_control() { }

denso_control::~denso_control()
{
	bstr1 = SysAllocString(L"GiveArm");
	VariantInit(&vntParam);
	vntParam.vt = VT_EMPTY;

	/* Release arm control authority */
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);



	/* Release robot handle */
	bCap_RobotRelease(fd, &hRob);

	/* Disconnect RC8 */
	bCap_ControllerDisconnect(fd, &hCtrl);

	/* Send SERVICE_STOP packet */
	bCap_ServiceStop(fd);

	/* Close connection */
	bCap_Close_Client(&fd);
}





void denso_control::Connect()
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
		if (SUCCEEDED(hr))	print("Connect  Succeeded...", 0);
		else				print("Connect  Failed...", 0);

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

			if (SUCCEEDED(hr))	print("GetRobot Succeeded...", 0);
			else				print("GetRobot Failed...", 0);

			//Release Variables
			SysFreeString(bstr1);
			SysFreeString(bstr2);

			if (SUCCEEDED(hr)) {
				bstr1 = SysAllocString(L"TakeArm");

				// Populate parameter option
				VariantInit(&vntParam);
				vntParam.vt = (VT_I4 | VT_ARRAY);
				vntParam.parray = SafeArrayCreateVector(VT_I4, 0, 2);
				SafeArrayAccessData(vntParam.parray, (void **)&plData);
				plData[0] = 0;		// Arm Group Number
				plData[1] = 0;		// Keep Value
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
					/* EXTSPEED 100% & ACCELERATION 0% & DECELERATION 50% */
					pdData[0] = 10;
					pdData[1] = 50;
					pdData[2] = 50;
					SafeArrayUnaccessData(vntParam.parray);

					printf("External Speed = %.f\n", pdData[0]);
					bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
					SysFreeString(bstr1);
					VariantClear(&vntParam);
					VariantClear(&vntRet);
				}
			}
		}
	}
}

void denso_control::Initialize()
{
	/* MOTOR ON */
	bstr1 = SysAllocString(L"Motor");
	VariantInit(&vntParam);
	vntParam.vt = VT_I4;
	vntParam.lVal = 1;

	/* Motor on */
	print("Motor ON", 0);
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

void denso_control::Speed()
{
	bstr1 = SysAllocString(L"ExtSpeed");
	VariantInit(&vntParam);
	vntParam.vt = (VT_R8 | VT_ARRAY);
	vntParam.parray = SafeArrayCreateVector(VT_R8, 0, 1);
	SafeArrayAccessData(vntParam.parray, (void **)&pdData);
	/* EXTSPEED 100% & ACCELERATION 50% & DECELERATION 50% */

	printf("0~100 : ");					cin >> speed;
	if (speed > 100)	return;
	pdData[0] = speed;
	SafeArrayUnaccessData(vntParam.parray);

	printf("External Speed = %.f\n", pdData[0]);
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);
}

void denso_control::getParam()
{
	//int cnt = 0;
	//while (1) {
		//if (cnt++ > 1000) {
			/* Get current position (x,y,z,rx,ry,rz,fig) */
			bstr1 = SysAllocString(L"CurPos");
			VariantInit(&vntParam);
			VariantInit(&vntRet);
			bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
			SafeArrayAccessData(vntRet.parray, (void **)&getData);
			memcpy(data, getData, 7 * sizeof(double));
			SafeArrayUnaccessData(vntRet.parray);

			SysFreeString(bstr1);
			VariantClear(&vntParam);
			VariantClear(&vntRet);
			print("CurPos", 1);


			/* Get current joint position  */
			bstr1 = SysAllocString(L"CurJnt");
			VariantInit(&vntParam);
			VariantInit(&vntRet);
			bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
			SafeArrayAccessData(vntRet.parray, (void **)&getData);
			memcpy(data, getData, 7 * sizeof(double));
			SafeArrayUnaccessData(vntRet.parray);

			SysFreeString(bstr1);
			VariantClear(&vntParam);
			VariantClear(&vntRet);
			print("CurJnt", 2);

			//cnt = 0;
		//}
	//}
}





void denso_control::Movepos()
{
	/* ACCELERATION 100% & DECELERATION 50% */
	hr = bCap_RobotAccelerate(fd, hRob, 0, 100, 50);		// 0:ALL axes   /   1:TOOL accel

	if (SUCCEEDED(hr)) {
		int i;
		print("Which pos?(0~2)", 0);		cin >> i;
		if (i > 2)		return;

		VariantInit(&vntParam);
		command = makeString(pos[i], 'P');
		vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
		vntParam.vt = VT_BSTR;
		bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
		VariantClear(&vntParam);
		print(command.str(), 0);
	}
}

void denso_control::Movejoint()
{
	if (SUCCEEDED(hr)) {
		int i;
		print("Which jointpos?(0~2)", 0);		cin >> i;
		if (i > 2)		return;

		VariantInit(&vntParam);
		command = makeString(jointpos[i], 'J');
		vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
		vntParam.vt = VT_BSTR;
		bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
		VariantClear(&vntParam);
	}
}

void denso_control::ChangeJointAngle()
{
	bstr1 = SysAllocString(L"CurJnt");
	VariantInit(&vntParam);
	VariantInit(&vntRet);
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
		cout << "Angle : ";				cin >> ang;
		data[j] = ang;

		VariantInit(&vntParam);
		command = makeString(data, 'J');
		vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
		vntParam.vt = VT_BSTR;
		bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
		VariantClear(&vntParam);
	}
}





void denso_control::KillAll()
{
	/*
	bstr1 = SysAllocString(L"Stop");
	VariantInit(&vntParam);
	VariantInit(&vntRet);
	bCap_ExtensionExecute(fd, hCtrl, bstr1, vntParam, &vntRet);

	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);



	bstr1 = SysAllocString(L"SuspendAll");
	VariantInit(&vntParam);
	bCap_ControllerExecute(fd, hCtrl, bstr1, vntParam, &vntRet);

	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);
	*/
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
}

void denso_control::ClearError()
{
	bstr1 = SysAllocString(L"ClearError");
	VariantInit(&vntParam);
	bCap_ControllerExecute(fd, hCtrl, bstr1, vntParam, &vntRet);

	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);
}



void denso_control::Task()
{
	VariantInit(&vntParam);
	command = makeString(taskpos[0], 'P');
	vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
	vntParam.vt = VT_BSTR;
	bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
	VariantClear(&vntParam);
	print(command.str(), 0);

	int j;	 char yn;
	cout << "which Task?(1,2) : "; cin >> j;
	cout << "Task start?(Y/n)  "; cin >> yn;

	if (yn = 'Y') {
		for (int i = 1; i <= 10; i++) {
			bstr1 = SysAllocString(L"ExtSpeed");
			VariantInit(&vntParam);
			vntParam.vt = (VT_R8 | VT_ARRAY);
			vntParam.parray = SafeArrayCreateVector(VT_R8, 0, 1);
			SafeArrayAccessData(vntParam.parray, (void **)&pdData);
			/* EXTSPEED 100% & ACCELERATION 50% & DECELERATION 50% */

			pdData[0] = (i + 1) * 10;
			if (pdData[0] > 100)	pdData[0] = 100;
			SafeArrayUnaccessData(vntParam.parray);

			printf("External Speed = %.f\n", pdData[0]);
			bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
			SysFreeString(bstr1);
			VariantClear(&vntParam);
			VariantClear(&vntRet);



			i %= 2;
			switch (j) {
			case 1:
				VariantInit(&vntParam);
				command = makeString(taskpos[i + 1], 'P');
				vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
				vntParam.vt = VT_BSTR;
				bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
				VariantClear(&vntParam);
				print(command.str(), 0);
				break;
			case 2:
				VariantInit(&vntParam);
				command = makeString(taskpos[i + 3], 'P');
				vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
				vntParam.vt = VT_BSTR;
				bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
				VariantClear(&vntParam);
				print(command.str(), 0);
				break;
			default:
				break;
			}
		}
	}
}










stringstream denso_control::makeString(double str[], char type)
{
	stringstream temp;
	if (sqrt((pow(str[0], 2) + pow(str[1], 2) + pow(str[2], 2))) > 1499) {
		str[1] = 0.0;	print("Range Over 1500", 0);
	}
	switch (type) {
	case 'P':
		temp << "@E P(" << str[0] << "," << str[1] << "," << str[2] << ","
			<< str[3] << "," << str[4] << "," << str[5] << ", 1)";	break;
	case 'J':
		temp << "@E J(" << str[0] << "," << str[1] << "," << str[2] << ","
			<< str[3] << "," << str[4] << "," << str[5] << ", 1)";	break;
	default:
		temp << "@E J(" << str[0] << "," << str[1] << "," << str[2] << ","
			<< str[3] << "," << str[4] << "," << str[5] << ", 1)";	break;
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
			getParam();
			print("CurPos", 1); print("CurJnt", 2);
			break;
		case 'J':							// Max: [170, 120, +151/-120, 270, 120, 360]
			cout << "Change Joint Angle" << endl;
			ChangeJointAngle();
			break;
		case 'M':
			cout << "Move" << endl;
			Movepos();	 break;
		case 'E':
			cout << "End" << endl;
			End();		 break;
		case 'Q':
			cout << "Kill All" << endl;
			KillAll();	 break;
		case 'N':
			cout << "Clear Error" << endl;
			ClearError(); break;
		default:			 break;
		}
	}
}