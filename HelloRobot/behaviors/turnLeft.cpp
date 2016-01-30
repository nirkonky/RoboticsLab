/*
 * turnLeft.cpp
 *
 *  Created on: Jan 30, 2016
 *      Author: colman
 */

#include "turnLeft.h"
//#include "../Helper.h"

using namespace std;

bool turnLeft::startCond(realPosition targetWaypoint, double angle){
	return (!isRight(_robot->getYaw(), angle));
}

bool turnLeft::stopCond(realPosition targetWaypoint, double angle){
	realPosition currentWaypoint;
	currentWaypoint.first = _robot->getYPos();
	currentWaypoint.second = _robot->getXPos();
	double yaw = radiansToDegrees(_robot->getYaw());

	int rank = isAngleCloseEnough(currentWaypoint, yaw, targetWaypoint);
	cout<<"currect Yaw: "<< yaw << " To --> " << angle<<endl;
	if (rank == 0){
		return true;
	}
	else if (rank == 1){
		_multiplier = 0.2;
		return false;
	}
	_multiplier = 1;
	return false;
}
