/*
 * moveForward.cpp
 *
 *  Created on: Jan 30, 2016
 *      Author: colman
 */

#include "moveForward.h"

bool moveForward::stopCond(realPosition targetWaypoint, double angle){
	int m2 = (angle < 180) ? 1 : -1;
	realPosition currentWaypoint;
	currentWaypoint.first = _robot->getYPos();
	currentWaypoint.second = _robot->getXPos();

	int rank = isCloseEnough(targetWaypoint,currentWaypoint);
		if (rank == 0){
			return true;
		}
		else if (rank == 1){
			_multiplier = 0.3;
			return false;
		}
		_multiplier = 1;
		return false;
	_multiplier = 1;
	return false;
}
