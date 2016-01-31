
#ifndef TURNLEFT_H_
#define TURNLEFT_H_

#include "../robot.h"
#include "behavior.h"

using namespace std;

class turnLeft: public behavior {
public:
	turnLeft(robot* robot): behavior(robot){};
	bool start(realPosition targetWaypoint, double angle);
	bool stop(realPosition targetWaypoint, double angle);
	void moving(){robotObject->setSpeed(0.0,TURN_SPEED * factor);}
	void pause();
	string name(){cout << "Turn Left:" << endl; return "TurnLeft";}
};
#endif /* TURNLEFT_H_ */
