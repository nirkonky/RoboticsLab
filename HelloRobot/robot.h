/*
 * robot.h
 *
 *  Created on: Jan 30, 2016
 *      Author: colman
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <iostream>
#include <libplayerc++/playerc++.h>
#include <math.h>
#include <cmath>
#include "Map.h"

using namespace std;
using namespace PlayerCc;

class robot {
private:
	PlayerClient *pc;
	Position2dProxy *pp;
	LaserProxy *lp;

public:
	robot(string ip, int port, double x, double y, double yaw);
	void setSpeed(double linearSpeed, double angularSpeed);
	double getXPos();
	double getYPos();
	double getYaw();
	void read();
	virtual ~robot();
};

#endif /* ROBOT_H_ */
