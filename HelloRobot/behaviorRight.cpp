
#include "behaviorRight.h"

using namespace std;

bool behaviorRight::start(realPosition targetWaypoint, double angle){
	return (canRight(robotObject->getYaw(), angle));
}
behaviorRight::~behaviorRight(){}

bool behaviorRight::stop(realPosition targetWaypoint, double angle){
	currentWaypoint.first = robotObject->getYPos();
	currentWaypoint.second = robotObject->getXPos();
	double yaw = (robotObject->getYaw()*180 / M_PI);
	angle = (atan2((targetWaypoint.first - currentWaypoint.first),targetWaypoint.second - currentWaypoint.second)* 180 / M_PI);
	if (abs(yaw - angle) < ANGLE_TOLERANCE)
	{
		return true; // VERY CLOSE;
	}
	else if (abs(yaw - angle) < ANGLE_TOLERANCE * 40)
	{
		factor = 0.15;
		return false; // CLOSE! CAREFUL!
	}
	factor = 1.2;
	return false; // FAR AWAY;
}
