/*
 * turnRight.cpp
 *
 *  Created on: Jan 30, 2016
 *      Author: colman
 */

#include "turnRight.h"

using namespace std;

bool turnRight::startCond(realPosition targetWaypoint, double angle){
	return (isRight(_robot->getYaw(), angle));
}

bool turnRight::stopCond(realPosition targetWaypoint, double angle){
	realPosition currentWaypoint;
	currentWaypoint.first = _robot->getYPos();
	currentWaypoint.second = _robot->getXPos();
	double yaw = radiansToDegrees(_robot->getYaw());

	int rank = isAngleCloseEnough(currentWaypoint, yaw, targetWaypoint);

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
