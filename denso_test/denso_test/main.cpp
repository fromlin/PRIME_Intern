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
			case 'I'://************************ ��Ʈ�ѷ� ���� �ٷ� ���� �Ұ�
				cout << "Init" << endl;
				dc.Initialize(); break;	
			case 'O':
				dc.MotorOn();	 break;
			case 'G'://************************ �ΰ� ���� ����
				cout << "Get Param" << endl;
				dc.getParam();
				dc.print("CurPos", 1);	dc.print("CurJnt", 2);
				break;
			case 'J':							// Max: [170, 120, +151/-120, 270, 120, 360]			�޾ƿ��� ���̶� �� �ٶ� ���� �̻�
				cout << "Change Joint Angle" << endl;
				dc.ChangeJointAngle();
				break;
			case 'M':
				cout << "Move" << endl;
				dc.Movepos();	 break;
			case 'R'://********************* �μ��� ����
				cout << "MOVE" << endl;
				dc.OutRange();	 break;
			case 'E':
				cout << "End" << endl;
				dc.End();		 break;
			case 'Q'://**************************** �ȸ���
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