
#include "behavior.h"

behavior::behavior(robot * robotb) : robotObject(robotb), factor(1) {}

behavior::~behavior() {}

behavior* behavior::pushNext(behavior *next){
	behaviorList.push_back(next);
	return this;
}

behavior *behavior::getNext(realPosition targetWaypoint, double angle){
	for (int i = 0; i < behaviorList.size(); ++i) {
		if (behaviorList[i]->start(targetWaypoint, angle))
		{
			cout << behaviorList[i]->name()<<endl;
			return behaviorList[i];
		}

	}
	return NULL;
}



