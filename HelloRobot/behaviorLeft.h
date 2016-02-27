
#ifndef TURNLEFT_H_
#define TURNLEFT_H_

#include "robot.h"
#include "behavior.h"

using namespace std;

class behaviorLeft: public behavior {
public:
	behaviorLeft(robot* robot): behavior(robot){};
	realPosition currentWaypoint;
	string name()
	{
		return "behavior Left";
	}

	void moving()
	{
		robotObject->setSpeed(0.0,TURN_SPEED * factor);
		robotObject->read();

	}
	bool start(realPosition targetWaypoint, double degree);
	bool stop(realPosition targetWaypoint, double degree);
	void pause();
	virtual ~behaviorLeft();

};
#endif /* TURNLEFT_H_ */
