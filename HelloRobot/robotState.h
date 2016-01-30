/*
 * robotState.h
 *
 *  Created on: Jan 30, 2016
 *      Author: colman
 */

#ifndef ROBOTSTATE_H_
#define ROBOTSTATE_H_


#include "defines.h"
using namespace std;

class robotState {
private:
	double x, y, yaw;
public:
	robotState(double x, double y, double yaw);
	double getX();
	double getY();
	double getYaw();
	virtual ~robotState();
};

#endif /* ROBOTPOSITION_H_ */
