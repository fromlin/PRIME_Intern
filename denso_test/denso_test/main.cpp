#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <windows.h>

#include "denso_control.h"
#include "adsClient.h"

using namespace std;

int main()
{	

	int key;
	printf("# ");
	string start;					cin >> start;
	
	denso_control dc;
	//dc.Connect();

	//thread test1([&] {dc.getParam(); });
	//thread test2([&] {
		while (1) {
			printf("\n>> ");
			key = _getch();
			if (key == 27)		 break;
			switch (key) {
			case 'C':
				cout << "Connect" << endl;
				dc.Connect();	 break;
			case 'I'://************************ 컨트롤러 없이 바로 연결 불가
				cout << "Init" << endl;
				dc.Initialize(); break;	
			case 'O':
				dc.MotorOn();	 break;
			case 'G'://************************ 두개 값이 같음
				cout << "Get Param" << endl;
				dc.getParam();
				dc.print("CurPos", 1);	dc.print("CurJnt", 2);
				break;
			case 'J':							// Max: [170, 120, +151/-120, 270, 120, 360]			받아오는 값이랑 값 줄때 뭔가 이상
				cout << "Change Joint Angle" << endl;
				dc.ChangeJointAngle();
				break;
			case 'M':
				cout << "Move" << endl;
				dc.Movepos();	 break;
			case 'R'://********************* 인수가 부정
				cout << "MOVE" << endl;
				dc.OutRange();	 break;
			case 'E':
				cout << "End" << endl;
				dc.End();		 break;
			case 'Q'://**************************** 안먹음
				cout << "Kill All" << endl;
				dc.KillAll();	 break;
			default:			 break;
			}
		}
	//});
	

	//test1.join();
	//test2.join();

	printf("\n\n# Exit Program\n");
	return 0;
}