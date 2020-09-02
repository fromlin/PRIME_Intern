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
	denso_control dc;	
	
	int key;
	while (1) {
		printf("\n>> ");
		key = _getch();
		if (key == 27)			 break;
		switch (key) {
		case 'I':
			cout << "Init" << endl;
			dc.Initialize();	 break;
		case 'O':
			//cout << "Motor On";G
			dc.MotorOn();	 break;
		case 'G':
			cout << "Get Position" << endl;
			dc.Getpos();		 break;
		case 'M':
			cout << "Move" << endl;
			dc.Movepos();	 break;
		case 'R':
			cout << "MOVE" << endl;
			dc.OutRange();	 break;
		case 'E':
			cout << "End" << endl;
			dc.End();			 break;
		default:				 break;
		}
	}

	printf("# Exit Program\n");
	return 0;
}