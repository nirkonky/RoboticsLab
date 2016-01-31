
#include "turnRight.h"

using namespace std;

bool turnRight::start(realPosition targetWaypoint, double angle){
	return (canRight(robotObject->getYaw(), angle));
}

bool turnRight::stop(realPosition targetWaypoint, double angle){
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
