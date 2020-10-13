#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <windows.h>

#include "denso_control.h"
//#include "adsClient.h"

using namespace std;


void Menu() {
	system("title Denso_Arm");
	printf("\n\n\n");
	printf("\t -------------------   Menu   ------------------- \n");
	printf("\t��                                                ��\n");
	printf("\t��   I : Initialization     O : Motor On          ��\n");
	printf("\t��   G : Get Pos & Joint                          ��\n");
	printf("\t��   N : Clear Error        E :End                ��\n");
	printf("\t��  --------------------------------------------  ��\n");
	printf("\t��   S : Change Speed                             ��\n");
	printf("\t��   M : Choose 3 Pos       J : Choose 3 Pos      ��\n");
	printf("\t��   A : 1 Joint Angle                            ��\n");
	printf("\t��  --------------------------------------------  ��\n");
	printf("\t��   R : Rotate             T : Task              ��\n");
	printf("\t��                                                ��\n");
	printf("\t -------------------------------------------------- \n");
	printf("\t                        Exit                        \n");
	printf("\n\n\n");
}



int main()
{	
	int key;
	denso_control dc;

	while (1) {
		Menu();
		printf("\n>> ");
		key = _getch();
		if (key == 27)		 break;

		switch (key) {
		case 'C':
			cout << "Connect" << endl;
			dc.Connect();	 break;
		case 'I':
			cout << "Init" << endl;
			dc.Initialize(); break;
		case 'O':
			dc.MotorOn();	 break;
		case 'E':
			cout << "End" << endl;
			dc.End();		 break;
		case 'N':
			cout << "Clear Error" << endl;
			dc.ClearError(); break;
		case 'G':
			cout << "Get Param" << endl;
			dc.GetParam();   break;



		case 'S':
			cout << "Change Speed" << endl;
			dc.Speed(0);	 break;
		case 'M':
			cout << "Move Pos" << endl;
			dc.MovePos();	 break;
		case 'J':
			cout << "Move Joint Pos" << endl;
			dc.MoveJoint();	 break;
		case 'A':					 // Max: [170, 120, +151/-120, 270, 120, 360]
			cout << "Change Joint Angle" << endl;
			dc.ChangeJointAngle();
			break;



		case 'R':
			cout << "Rotate" << endl;
			dc.Rotate();	 break;
		case 'T':
			cout << "Task" << endl;
			dc.Task(); break;

		default:			 break;
		}
		printf("\n\n\n");
	}



	dc.End();
	printf("\n\n# Exit Program\n");
	return 0;
}