/*
 * moveForward.h
 *
 *  Created on: Jan 30, 2016
 *      Author: colman
 */

#ifndef MOVEFORWARD_H_
#define MOVEFORWARD_H_


#include "../robot.h"
#include "behavior.h"
#include "../defines.h"

using namespace std;

class moveForward: public behavior {

public:
	moveForward(robot* robot): behavior(robot){};
	bool startCond(realPosition targetWaypoint, double angle){return true;}
	bool stopCond(realPosition targetWaypoint, double angle);
	void action(){_robot->setSpeed(LINEAR_SPEED * _multiplier,0.0);}
	string name(){cout << "Move Forward:" << endl; return "Move Forward";}
};


#endif /* MOVEFORWARD_H_ */
