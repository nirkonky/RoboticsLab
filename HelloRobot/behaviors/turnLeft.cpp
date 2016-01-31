
#include "turnLeft.h"
#include "../defines.h"
#include "../robot.h"
#include <iostream>
#include <math.h>
#include <cmath>
//#include "../Helper.h"

using namespace std;

bool turnLeft::start(realPosition targetWaypoint, double angle){
	return (!canRight(robotObject->getYaw(), angle));
}

bool turnLeft::stop(realPosition targetWaypoint, double angle){
	realPosition currentWaypoint;
	currentWaypoint.first = robotObject->getYPos();
	currentWaypoint.second = robotObject->getXPos();
	double yaw = (robotObject->getYaw()*180 / M_PI);
	double currect_X = currentWaypoint.first;
	double currect_Y = currentWaypoint.second;
	double target_X = targetWaypoint.first;
	double target_Y = targetWaypoint.second;
	angle = (atan2((target_X - currect_X),target_Y - currect_Y)* 180 / M_PI);
	if (abs(yaw - angle) < ANGLE_TOLERANCE)
	{
		return true; // VERY CLOSE;
	}
	else if (abs(yaw - angle) < ANGLE_TOLERANCE * 40)
	{
		factor = 0.1;
		return false; // CLOSE! CAREFUL!
	}
	factor = 1;
	return false; // FAR AWAY;
}
