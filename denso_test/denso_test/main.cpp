#include <iostream>
#include <conio.h>
#include <windows.h>

#include "denso_control.h"
#include "adsClient.h"

using namespace std;

int main()
{
	printf("# ");
	int num;
	string start;
	cin >> start;


	thread test([] {
		denso_control dc;
		int key;
		while (1) {
			printf("\n>> ");
			key = _getch();
			if (key == 27)		 break;
			switch (key) {
			case 'I':
				cout << "Init" << endl;
				dc.Initialize(); break;
			case 'O':
				dc.MotorOn();	 break;
			case 'G':
				cout << "Get Param" << endl;
				dc.Getparam();
				dc.print("CurPos", 1);	dc.print("CurJnt", 2);
				break;
			case 'J':							// Max: [170, 120, +151/-120, 270, 120, 360]
				cout << "Change Joint Angle" << endl;
				dc.ChangeJointAngle();
				break;
			case 'M':
				cout << "Move" << endl;
				dc.Movepos();	 break;
			case 'R':
				cout << "MOVE" << endl;
				dc.OutRange();	 break;
			case 'E':
				cout << "End" << endl;
				dc.End();		 break;
			case 'Q':
				cout << "Kill All" << endl;
				dc.KillAll();	 break;
			default:			 break;
			}
		}
	});

	test.join();

	printf("\n\n# Exit Program\n");
	return 0;
}