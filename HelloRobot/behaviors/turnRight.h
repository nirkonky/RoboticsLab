
#ifndef TURNRIGHT_H_
#define TURNRIGHT_H_

#include "../robot.h"
#include "behavior.h"

using namespace std;

class turnRight: public behavior {
public:
	turnRight(robot* robot):behavior(robot){};
	bool start(realPosition targetWaypoint, double angle);
	bool stop(realPosition targetWaypoint, double angle);
	void moving(){robotObject->setSpeed(0.0,-1 * TURN_SPEED * factor);}
	void pause();
	string name(){cout << "Turn Right:" << endl; return "TurnRight";}
};
#endif /* TURNRIGHT_H_ */
