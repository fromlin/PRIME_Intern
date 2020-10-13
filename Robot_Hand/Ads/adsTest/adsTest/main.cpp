#include <iostream>
#include <conio.h>
#include <windows.h>
#include "adsClient.h"

using namespace std;

CadsClient ads;

int main()
{
	ads.Close();

	long nErr;
	nErr = ads.Connect(350);
	if (nErr)	cerr << "Error : " << nErr << endl;
	else cout << nErr << endl << "S" << endl;

	short readData = 0;
	while (1) {
		AdsSyncReadReq(ads.pSrvAddr, 0x1010010, 0x80000000, sizeof(readData), &readData);
		cout << readData << endl;
	}
}