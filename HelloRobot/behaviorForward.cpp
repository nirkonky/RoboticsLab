
#include "behaviorForward.h"

bool behaviorForward::start(realPosition targetWaypoint, double degree)
	{
		return true;
	}
bool behaviorForward::stop(realPosition targetWaypoint, double angle){
	currentWaypoint.first = robotObject->getYPos();
	currentWaypoint.second = robotObject->getXPos();
	if ((abs(currentWaypoint.first - targetWaypoint.first) <  LINEAR_TOLERANCE) &&(abs(currentWaypoint.second - targetWaypoint.second) < LINEAR_TOLERANCE))
	{
		cout<<"stop!"<<endl;
		return true; // VERY CLOSE
	}
	else if ((abs(currentWaypoint.second - targetWaypoint.second) < 10 * LINEAR_TOLERANCE)&& (abs(currentWaypoint.first - targetWaypoint.first) <  10 * LINEAR_TOLERANCE))
	{
		cout<<"almost there!"<<endl;
		factor = 0.3;
		return false; // CLOSE! CAREFUL!
	}
	cout<<"far away."<<endl;
	factor = 1;
	return false; // FAR AWAY.
}
behaviorForward::~behaviorForward(){}

