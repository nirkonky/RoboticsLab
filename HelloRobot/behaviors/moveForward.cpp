
#include "moveForward.h"

bool moveForward::stop(realPosition targetWaypoint, double angle){
	realPosition currentWaypoint;
	currentWaypoint.first = robotObject->getYPos();
	currentWaypoint.second = robotObject->getXPos();
	double currect_X = currentWaypoint.first;
	double currect_Y = currentWaypoint.second;
	double target_X = targetWaypoint.first;
	double target_Y = targetWaypoint.second;

	if ((abs(currect_X - target_X) <  LINEAR_TOLERANCE) &&(abs(currect_Y - target_Y) < LINEAR_TOLERANCE))
	{
		cout<<"stop!"<<endl;
		return true; // VERY CLOSE
	}
	else if ((abs(currect_X - target_X) <  10 * LINEAR_TOLERANCE) &&(abs(currect_Y - target_Y) < 10 * LINEAR_TOLERANCE))
	{
		cout<<"almost there!"<<endl;
		factor = 0.3;
		return false; // CLOSE! CAREFUL!
	}
	cout<<"far away."<<endl;
	factor = 1;
	return false; // FAR AWAY.
}
