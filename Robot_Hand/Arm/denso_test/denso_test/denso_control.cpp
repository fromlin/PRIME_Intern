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
		if (SUCCEEDED(hr))	print("Connect  Succeeded...", 0);
		else				print("Connect  Failed...", 0);
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
			SysFreeString(bstr1);
			SysFreeString(bstr2);

			if (SUCCEEDED(hr)) {
				bstr1 = SysAllocString(L"TakeArm");
				VariantInit(&vntParam);
				vntParam.vt = (VT_I4 | VT_ARRAY);
				vntParam.parray = SafeArrayCreateVector(VT_I4, 0, 2);
				SafeArrayAccessData(vntParam.parray, (void **)&plData);
				plData[0] = 0;		// Arm Group Number
				plData[1] = 1;		// Keep Value
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
					vntParam.parray = SafeArrayCreateVector(VT_R8, 0, 3);
					SafeArrayAccessData(vntParam.parray, (void **)&pdData);
					/* EXTSPEED 20% & ACCELERATION 100% & DECELERATION 50% */
					pdData[0] = 20;
					pdData[1] = 100;
					pdData[2] = 100;
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
			SysFreeString(bstr1);
			SysFreeString(bstr2);

			if (SUCCEEDED(hr)) {
				bstr1 = SysAllocString(L"TakeArm");
				VariantInit(&vntParam);
				vntParam.vt = (VT_I4 | VT_ARRAY);
				vntParam.parray = SafeArrayCreateVector(VT_I4, 0, 2);
				SafeArrayAccessData(vntParam.parray, (void **)&plData);
				plData[0] = 0;		// Arm Group Number
				plData[1] = 1;		// Keep Value
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
					vntParam.parray = SafeArrayCreateVector(VT_R8, 0, 3);
					SafeArrayAccessData(vntParam.parray, (void **)&pdData);
					/* EXTSPEED 20% & ACCELERATION 100% & DECELERATION 50% */
					pdData[0] = 20;
					pdData[1] = 100;
					pdData[2] = 100;
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
	bstr1 = SysAllocString(L"ExtSpeed");
	VariantInit(&vntParam);
	vntParam.vt = (VT_R8 | VT_ARRAY);
	vntParam.parray = SafeArrayCreateVector(VT_R8, 0, 3);
	SafeArrayAccessData(vntParam.parray, (void **)&pdData);
	/* EXTSPEED 20% & ACCELERATION 100% & DECELERATION 50% */
	pdData[0] = 30;
	pdData[1] = 100;
	pdData[2] = 80;
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
	print("Motor ON", 0);
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);



	// Move to P(x,y,z,rx,ry,rz,fig) / First Position
	VariantInit(&vntParam);
	command = makeString(initialpos[0], 'P');
	vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
	vntParam.vt = VT_BSTR;
	bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
	VariantClear(&vntParam);
	print(command.str(), 0);
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

void denso_control::GetParam()
{
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
}





void denso_control::Speed(bool ch)
{
	bstr1 = SysAllocString(L"ExtSpeed");
	VariantInit(&vntParam);
	vntParam.vt = (VT_R8 | VT_ARRAY);
	vntParam.parray = SafeArrayCreateVector(VT_R8, 0, 1);
	SafeArrayAccessData(vntParam.parray, (void **)&pdData);

	if (ch)		speed = 20;
	else {
		printf("0~100 : ");					cin >> speed;
		if (speed > 100)	return;
	}
	pdData[0] = speed;
	SafeArrayUnaccessData(vntParam.parray);

	printf("External Speed = %.f\n", pdData[0]);
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SysFreeString(bstr1);
	VariantClear(&vntParam);
	VariantClear(&vntRet);
}

void denso_control::MovePos()
{
	/* ACCELERATION 100% & DECELERATION 50% */
	//hr = bCap_RobotAccelerate(fd, hRob, 0, 100, 50);		// 0:ALL axes   /   1:TOOL accel

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
	//Speed(0);
	//bstr1 = SysAllocString(L"MotionSkip");
	//VariantInit(&vntParam);
	//vntParam.vt = (VT_I4 | VT_ARRAY);
	//vntParam.parray = SafeArrayCreateVector(VT_I4, 0, 2);
	//SafeArrayAccessData(vntParam.parray, (void **)&pdData);
	//pdData[0] = 0;
	//pdData[1] = 1;
	//SafeArrayUnaccessData(vntParam.parray);

	//print("Motion Skip complete", 0);
	//hr = bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);


	for (int i = 0; i < 3; i++) {
		VariantInit(&vntParam);
		command = makeString(pos[i], 'P');
		vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
		vntParam.vt = VT_BSTR;
		bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
		VariantClear(&vntParam);
		print(command.str(), 0);
	}
}

void denso_control::MoveJoint()
{
	Speed(0);
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
	Speed(0);

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





void denso_control::Rotate()
{
	VariantInit(&vntParam);
	command = makeString(initialpos[1], 'P');
	vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
	vntParam.vt = VT_BSTR;
	bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
	VariantClear(&vntParam);
	print(command.str(), 0);
	float rot_angle;


	//// 속도 점점 빨리, 대신 끊김
	//for (int i = 1; i <= 7; i++) {
	//	vntParam.bstrVal = SysAllocString(L"V(1, 0, 0), V(1, 1, 0), V(1, 0, 1)");
	//	vntParam.vt = VT_BSTR;
	//	rot_angle = 360.0;// *COUNT;
	//	vntRet.bstrVal = SysAllocString(L"V(0, 0, 310)");
	//	vntRet.vt = VT_BSTR;

	//	sp[0] = (i + 5) * 10;
	//	if (sp[0] > 100)	sp[0] = 100;
	//	command = makeString(sp, 'R');
	//	bstr1 = SysAllocString(CA2W(command.str().c_str()));
	//	bCap_RobotRotate(fd, hRob, vntParam, rot_angle, vntRet, bstr1);
	//}


	// 같은 속도 여러번
	cout << "> Max speed : 40" << endl;
	Speed(0);			// Max : 45

	vntParam.bstrVal = SysAllocString(L"V(1, 0, 0), V(1, 1, 0), V(1, 0, 1)");			// 세 점으로 이루어진 평면 : yz
	vntParam.vt = VT_BSTR;
	rot_angle = 360.0 * COUNT;
	vntRet.bstrVal = SysAllocString(L"V(0, 0, 310)");									// 기준위치 z:340으로 부터 310 -> 30mm 반경으로 원 그리기
	vntRet.vt = VT_BSTR;
	bstr1 = SysAllocString(L"Pose=2, NEXT");
	bCap_RobotRotate(fd, hRob, vntParam, rot_angle, vntRet, bstr1);
}

bool denso_control::GrabPos()
{
	char yn;
	cout << "> ";	 cin >> yn;
	VariantInit(&vntParam);

	switch (yn) {
	case '1':		command = makeString(taskpos[0], 'P');		break;
	case '2':		command = makeString(grabpos[0], 'P');		break;
	case '3':		command = makeString(grabpos[1], 'P');		break;
	case '4':		command = makeString(grabpos[0], 'P');		break;
									//
	case '5':		command = makeString(taskpos[0], 'P');		break;
									//
	case '6':		command = makeString(grabpos[2], 'P');		break;
	case '7':		command = makeString(grabpos[3], 'P');		break;
	case '8':		command = makeString(grabpos[2], 'P');		break;
	case '9':		command = makeString(taskpos[0], 'P');		break;
									//
	default:		return 1;
	}
	
	vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
	vntParam.vt = VT_BSTR;
	bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
	VariantClear(&vntParam);
	return 0;
}

void denso_control::Task()
{
	cout << "Step 1~9" << endl;

	Speed(1);
	for (int i = 0; i < 5; i++) {
		if (gb = GrabPos())		break;
	};

	if (gb) return;		char yn;
	cout << "Task start?(Y/N)  ";  cin >> yn;
	if (yn == 'Y')		Rotate();
	else				return;

	Speed(1);
	for (int i = 0; i < 4; i++) {
		if (GrabPos())	break;
	};
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
	case 'R':
		temp << "S=" << str[0] << "," << "Pose=2, NEXT";	break;
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















//void denso_control::Task()
//{
//	Speed(1);
//	GrabPos();	GrabPos();	GrabPos();
//
//
//
//	int j;	 char yn;
//	cout << "Task start?(Y/N)  ";  cin >> yn;
//
//	//bstr1 = SysAllocString(L"ExtSpeed");
//	//VariantInit(&vntParam);
//	//vntParam.vt = (VT_R8 | VT_ARRAY);
//	//vntParam.parray = SafeArrayCreateVector(VT_R8, 0, 1);
//	//SafeArrayAccessData(vntParam.parray, (void **)&pdData);
//	///* EXTSPEED 100% & ACCELERATION 50% & DECELERATION 100% */
//
//	//pdData[0] = 100;
//	//SafeArrayUnaccessData(vntParam.parray);
//	//bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
//	//SysFreeString(bstr1);
//	//VariantClear(&vntParam);
//	//VariantClear(&vntRet);
//
//	if (yn == 'Y') {
//		cout << "which Task?(1,2) : "; cin >> j;
//		for (int i = 1; i <= 10; i++) {
//			bstr1 = SysAllocString(L"ExtSpeed");
//			VariantInit(&vntParam);
//			vntParam.vt = (VT_R8 | VT_ARRAY);
//			vntParam.parray = SafeArrayCreateVector(VT_R8, 0, 1);
//			SafeArrayAccessData(vntParam.parray, (void **)&pdData);
//			/* EXTSPEED 50~100% & ACCELERATION 50% & DECELERATION 100% */
//
//			pdData[0] = i * 20;
//			if (pdData[0] > 100)	pdData[0] = 100;
//			SafeArrayUnaccessData(vntParam.parray);
//
//			printf("External Speed = %.f\n", pdData[0]);
//			bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
//			SysFreeString(bstr1);
//			VariantClear(&vntParam);
//			VariantClear(&vntRet);
//
//
//
//			switch (j) {
//			case 1:
//				VariantInit(&vntParam);
//				command = makeString(taskpos[i % 2 + 1], 'P');
//				vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
//				vntParam.vt = VT_BSTR;
//				hr = bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
//				//VariantClear(&vntParam);
//				//print(command.str(), 0);
//				break;
//			case 2:
//				VariantInit(&vntParam);
//				command = makeString(taskpos[i % 2 + 3], 'P');
//				vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
//				vntParam.vt = VT_BSTR;
//				hr = bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
//				//VariantClear(&vntParam);
//				//print(command.str(), 0);
//				break;
//			default:
//				break;
//			}
//
//			if (SUCCEEDED(hr)) {
//				bstr1 = SysAllocString(L"MotionSkip");
//				VariantInit(&vntParam);
//				vntParam.vt = (VT_I4 | VT_ARRAY);
//				vntParam.parray = SafeArrayCreateVector(VT_I4, 0, 2);
//				SafeArrayAccessData(vntParam.parray, (void **)&pdData);
//				pdData[0] = 0;
//				pdData[1] = 1;
//				SafeArrayUnaccessData(vntParam.parray);
//
//				hr = bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
//				SysFreeString(bstr1);
//				VariantClear(&vntParam);
//				VariantClear(&vntRet);
//			}
//		}
//	}
//	else
//		return;
//
//	VariantInit(&vntParam);
//	command = makeString(taskpos[0], 'P');
//	vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
//	vntParam.vt = VT_BSTR;
//	bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
//	VariantClear(&vntParam);
//	print(command.str(), 0);
//
//	Speed(1);
//	GrabPos();	GrabPos();	GrabPos();
//}