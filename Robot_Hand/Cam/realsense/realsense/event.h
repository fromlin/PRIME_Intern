#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <opencv2/opencv.hpp>
#include <string>

using namespace std;



void keybd_event(int& inKey, double& event_val1, int& cell_flag) {
	//event_val1 : alpha

	switch (inKey) {
	case 32:       	// SpaceBar: 32
		cout << "\n               -----< ReStart >-----\n\n";
		break;

	default:
		cout << "get key : " << inKey << endl;
		break;

	}
}

#endif