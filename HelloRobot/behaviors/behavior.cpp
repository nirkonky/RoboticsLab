
#include "behavior.h"

behavior::behavior(robot * robotb) : robotObject(robotb), factor(1) {}

behavior::~behavior() {}

behavior* behavior::pushNext(behavior *next){
	getNextBehavior.push_back(next);
	return this;
}

behavior *behavior::getNext(realPosition targetWaypoint, double angle){
	for (int i = 0; i < getNextBehavior.size(); ++i) {
		if (getNextBehavior[i]->start(targetWaypoint, angle))
		{
			return getNextBehavior[i];
		}

	}
	return NULL;
}



