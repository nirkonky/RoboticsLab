
#ifndef MOVEFORWARD_H_
#define MOVEFORWARD_H_


#include "../robot.h"
#include "behavior.h"
#include "../defines.h"

using namespace std;

class moveForward: public behavior {

public:
	moveForward(robot* robot): behavior(robot){};
	bool start(realPosition targetWaypoint, double angle){return true;}
	bool stop(realPosition targetWaypoint, double angle);
	void moving(){robotObject->setSpeed(LINEAR_SPEED * factor,0.0);}
	string name(){cout << "Move Forward:" << endl; return "Move Forward";}
};


#endif /* MOVEFORWARD_H_ */
