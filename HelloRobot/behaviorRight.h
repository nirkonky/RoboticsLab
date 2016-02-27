
#ifndef TURNRIGHT_H_
#define TURNRIGHT_H_

#include "robot.h"
#include "behavior.h"

using namespace std;

class behaviorRight: public behavior {
public:
	behaviorRight(robot* robot):behavior(robot){};
	realPosition currentWaypoint;
	string name()
	{
		return "behavior Right";
	}
	bool isBehaviorForWard()
	{
		return false;
	}
	bool start(realPosition targetWaypoint, double degree);
	bool stop(realPosition targetWaypoint, double degree);
	void moving()
	{
		robotObject->setSpeed(0.0,-1 * TURN_SPEED * factor);
		robotObject->read();

	}
	void pause();
	virtual ~behaviorRight();
};
#endif /* TURNRIGHT_H_ */
