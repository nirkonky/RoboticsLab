/*
 * behavior.cpp
 *
 *  Created on: Jan 30, 2016
 *      Author: colman
 */

#include "behavior.h"

behavior::behavior(robot *robot):_robot(robot), _multiplier(1) {}

behavior::~behavior() {}

behavior* behavior::addNext(behavior *next){
	_nextBehaviors.push_back(next);
	return this;
}

behavior *behavior::selectNext(realPosition targetWaypoint, double angle){
	for (unsigned int i = 0; i < _nextBehaviors.size(); ++i) {
		if (_nextBehaviors[i]->startCond(targetWaypoint, angle))
		{
			return _nextBehaviors[i];
		}

	}
	return NULL;
}



